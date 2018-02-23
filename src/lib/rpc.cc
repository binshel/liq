#include "errorcode.h"
#include "liq/rpc.h"
#include "liq/service.h"
#include "liq/logger.h"
#include "shm_channel.h"


namespace liq {

#pragma pack(1)
    struct RPCHeader {
        int8_t  type;           // req:1 ; res:2
        int32_t cid;            // 调用ID
        int32_t tid;            // 线程ID
        int8_t name_len;        // 服务名字长度
        int8_t method_len;      // 方法名字长度
        int32_t data_len;       // 数据长度
    };
#pragma pack()

    static uint8_t msg_buff[64000];


    /** IPCShm **/
    IPCShm::IPCShm(const std::string &from, const std::string &remote, const int channelsize) {
            shm_channel_open(&this->channel, from.c_str(), remote.c_str(), channelsize);
        }

    int32_t IPCShm::send(const uint8_t *data, int32_t len) {
        return shm_channel_send(this->channel, data, len);
    }
    int32_t IPCShm::recv(uint8_t *buff) {
        const char *data = NULL;
        int len = 0;
        int r = shm_channel_peek_recv(this->channel, &data, &len);
        if (r == ERRORCODE_SUCESS) {
            //DEBUGF("channel recv len %d:%d", r, len);
            memcpy(buff, data, len);
            shm_channel_recv_peek(this->channel);
        } else if (r != ERRORCODE_CHANNEL_EMPTY) {
            ERRORF("channel recv error %d", r);
        }
        return len;
    }
    


    /** RPC **/
    int64_t RPC::last_id = 0;

    RPC::RPC(const std::string &name, IPCBase *ipc)
            :name(name), ipc(ipc) {
    }

    void RPC::call(RPC_TYPE type, const char *method, google::protobuf::Message &message,  uint8_t **res_buff, int32_t *res_len)
    {
        RPCHeader header;
        header.type = type;
        header.cid = ++this->last_id;
        header.tid = Liq::instance().thread_pool->self();
        header.data_len = message.ByteSize();
        header.name_len = this->name.length();
        header.method_len = strlen(method);

        uint8_t *buff = msg_buff;
        memcpy(buff, &header, sizeof(header)); buff += sizeof(header);
        memcpy(buff, this->name.c_str(), header.name_len); buff += header.name_len;
        memcpy(buff, method, header.method_len); buff += header.method_len;
        message.SerializeWithCachedSizesToArray(buff); buff += header.data_len;

        DEBUGF("rpc call[%d] %s::%s %d:%ld", header.cid, this->name.c_str(), method, header.data_len, buff - msg_buff);
        int code = this->ipc->send(msg_buff, buff - msg_buff);
        DEBUGF("ipc code[%d]", code);
        if (type == RPC_TWOWAY) {
            ThreadEvent &event = Liq::instance().thread_pool->yield();
            ReturnArg *res = static_cast<ReturnArg*>(event.data);
            if (event.err) {
                throw "rpc call error [" + (*event.err) + "] " + this->name + "." + method;
            } else if (res->ecode != 0) {
                delete res;
                throw "rpc call error return " + this->name + "." + method;
            } else {
                *res_buff = res->data;
                *res_len = res->data_len;
                delete res;
            }
        }
        return;
    }


    /** RPCManager **/
    RPCManager::RPCManager(const std::string &node_name) 
        : node_name(node_name)
    {
    }
    RPC* RPCManager::create_rpc(const char *name, const char *remote) {
        IPCShm *ipc = new IPCShm(node_name, remote);
        this->ipcs[remote] = ipc;
        return new RPC(name, ipc);
    }
    IPCBase* RPCManager::create_ipc(const char *remote) {
        IPCShm *ipc = new IPCShm(node_name, remote);
        this->ipcs[remote] = ipc;
        return ipc;
    }

    int32_t RPCManager::handle_ipc(IPCBase *ipc)
    {
        int32_t count = 0;
        while (true) {
            int32_t len = ipc->recv(msg_buff);
            if (len <= 0) return count;
            ++count;
            DEBUGF("recv shm len: %d", len);
            RPCHeader *header = (RPCHeader*)msg_buff;
            switch (header->type) {
                case RPC_TWOWAY: 
                case RPC_ONEWAY: 
                    {
                        RPC::BecallArg arg = {
                            .ipc        = ipc,
                            .data       = msg_buff,
                            .data_len   = len
                        };
                        Liq::instance().thread_pool->spawn([&arg]() {
                                Liq::instance().rpc->on_becall(&arg);
                                });
                        break;
                    }
                case RPC_ERROR:
                    {
                        RPC::ReturnArg *ret = new RPC::ReturnArg();
                        ret->ecode = -1;
                        DEBUGF("call error return[%d]", header->cid);
                        uint64_t tid = header->tid;
                        Liq::instance().thread_pool->notify(tid, ret);
                        break;
                    }
                case RPC_RETURN:
                    {
                        uint8_t *p = msg_buff + sizeof(*header);
                        RPC::ReturnArg *ret = new RPC::ReturnArg();
                        ret->ecode = 0;
                        ret->data = p + header->name_len + header->method_len;
                        ret->data_len = header->data_len;
                        DEBUGF("call success return[%d] %ld:%d", header->cid, len - (ret->data - msg_buff), ret->data_len);
                        uint64_t tid = header->tid;
                        Liq::instance().thread_pool->notify(tid, ret);
                        break;
                    }
                default:
                    ERRORF("error msg type %d", header->type);
                    break;
            }
        }
    }


    int32_t RPCManager::ontick()
    {
        int count = 0;
        for (auto it = this->ipcs.begin(); it != this->ipcs.end(); ++it) {
            count += this->handle_ipc(it->second);
        }
        return count;
    }

    void RPCManager::on_becall(RPC::BecallArg *p_arg) {
        RPC::BecallArg arg = *p_arg;
        RPCHeader header = *(RPCHeader*)arg.data;
        char service_name[256]; service_name[header.name_len] = '\0';
        char method_name[256]; method_name[header.method_len] = '\0';
        uint8_t *p = arg.data + sizeof(header);
        memcpy(service_name, (const char*)p, header.name_len); p += header.name_len;
        memcpy(method_name, (const char*)p, header.method_len); p += header.method_len;
        DEBUGF("call method[%d] %s::%s(%d)[%ld:%d]", 
                header.cid, service_name, method_name, header.method_len, 
                arg.data_len - (p - arg.data), header.data_len);

        CommonSkeleton *skeleton = Liq::instance().service->get_skeleton(service_name);
        if (!skeleton) {
            ERRORF("no service for %s::%s[%d]", service_name, method_name, header.data_len);
            return;
        } 
        auto res = skeleton->handle(method_name, p, header.data_len);
        if (header.type == RPC_TWOWAY) {
            uint8_t *buff = arg.data;
            header.type = RPC_RETURN;
            header.data_len = res ? res->ByteSize() : 0;

            memcpy(buff, &header, sizeof(header)); buff += sizeof(header);
            memcpy(buff, service_name, header.name_len); buff += header.name_len;
            memcpy(buff, method_name, header.method_len); buff += header.method_len;
            if (res) {
                res->SerializeWithCachedSizesToArray(buff); buff += header.data_len;
            }

            DEBUGF("call return[%d] %d:%ld", header.cid, header.data_len, buff - arg.data);
            arg.ipc->send(arg.data, buff - arg.data);
        }
        if (res) delete res;
    }
}
