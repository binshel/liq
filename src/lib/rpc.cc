
#include "errorcode.h"
#include "shm_channel.h"
#include "rpc.h"
#include "service.h"


namespace liq {

#pragma pack(1)
    struct RPCHeader {
        int8_t  type;           // req:1 ; res:2
        int32_t rid;            // 调用ID
        int8_t name_len;        // 服务名字长度
        int8_t method_len;      // 方法名字长度
        int32_t data_len;       // 数据长度
    };
#pragma pack()


    /** IPCShm **/
    class IPCShm : public IPCBase {
    public:
        IPCShm() = delete;
        IPCShm(const std::string &from, const std::string &remote) {
            shm_channel_open(&this->channel, from.c_str(), remote.c_str(), 100*1000);
        }

        virtual int32_t send(const uint8_t *data, int32_t len) {
            shm_channel_send(this->channel, data, len);
        }
        virtual int32_t recv(uint8_t *buff) {
            const char *data = NULL;
            int len = 0;
            int r = shm_channel_peek_recv(this->channel, &data, &len);
            if (r == ERRORCODE_SUCESS) {
                printf("channel recv len %d:%d\n", r, len);
                memcpy(buff, data, len);
                shm_channel_recv_peek(this->channel);
            } else if (r != ERRORCODE_CHANNEL_EMPTY) {
                printf("channel recv error %d\n", r);
            }
            return len;
        }
        
    private:
        LPCHANNEL channel;
    };


    /** RPC **/
    RPC::RPC(RPCManager *manager, const std::string &name, IPCBase *ipc)
            :manager(manager), name(name), ipc(ipc) {
    }

    void RPC::call(const char *method, google::protobuf::Message &message,  uint8_t **res_buff, int32_t *res_len) {
        RPCHeader header;
        header.type = 1;
        header.rid = ++this->last_id;
        header.data_len = message.ByteSize();
        header.name_len = this->name.length();
        header.method_len = strlen(method);


        uint8_t *buff = manager->buff;
        memcpy(buff, &header, sizeof(header)); buff += sizeof(header);
        memcpy(buff, this->name.c_str(), header.name_len); buff += header.name_len;
        memcpy(buff, method, header.method_len); buff += header.method_len;
        message.SerializeWithCachedSizesToArray(buff); buff += header.data_len;

        printf("rpc call[%d] %s::%s %d:%d\n", header.rid, this->name.c_str(), method, header.data_len, buff - manager->buff);
        int code = this->ipc->send(manager->buff, buff - manager->buff);
        printf("ipc code[%d]\n", code);
        manager->rid2tid[header.rid] = liq->thread_pool->self();
        RPCManager::ReturnArg *res = (RPCManager::ReturnArg*)liq->thread_pool->yield(ThreadPool::EVENT_RPC, header.rid);
        *res_buff = res->data;
        *res_len = res->data_len;
        delete res;
        return;
    }

    int64_t RPC::last_id = 0;



    /** RPCManager **/
    RPCManager::RPCManager(const char *node_name) 
            : node_name(node_name) {
    }
    RPC* RPCManager::create_rpc(IPC_TYPE type, const char *name, const char *remote) {
        IPCShm *ipc = new IPCShm(node_name, remote);
        this->shms[remote] = ipc;
        return new RPC(this, name, ipc);
    }
    IPCBase* RPCManager::create_ipc(IPC_TYPE type, const char *remote) {
        IPCShm *ipc = new IPCShm(node_name, remote);
        this->shms[remote] = ipc;
        return ipc;
    }

    int32_t RPCManager::ontick() {
        int count = 0;
        for (auto it = this->shms.begin(); it != this->shms.end(); ++it) {
            int32_t len = it->second->recv(this->buff);
            if (len <= 0) continue;
            ++count;
            printf("shm len: %d\n", len);
            RPCHeader *header = (RPCHeader*)this->buff;
            if (header->type == 1) {        // becall
                BecallArg arg = {
                    .manager    = this,
                    .ipc        = it->second,
                    .data       = this->buff,
                    .data_len   = len
                };
                liq->thread_pool->spawn((ThreadBase::fun_enter)RPCManager::on_becall, &arg);
            } else if (header->type == 2) {    // call return
                uint8_t *p = this->buff + sizeof(*header);
                RPCManager::ReturnArg *res = new ReturnArg();
                res->type = ThreadPool::EVENT_RPC;
                res->value = header->rid;
                res->data = p + header->name_len + header->method_len;
                res->data_len = header->data_len;
                uint64_t tid = rid2tid[header->rid];
                printf("call return[%d:%d] %d:%d\n", header->rid, len - (res->data - this->buff), res->data_len);
                liq->thread_pool->notify(tid, res);
            }
        }
        return count;
    }

    void RPCManager::on_becall(BecallArg *p_arg) {
        BecallArg arg = *p_arg;
        RPCHeader header = *(RPCHeader*)arg.data;
        char service_name[256];
        char method_name[256];
        uint8_t *p = arg.data + sizeof(header);
        strncpy(service_name, (const char*)p, header.name_len); p += header.name_len;
        strncpy(method_name, (const char*)p, header.method_len); p += header.method_len;
        printf("call method[%d] %s::%s[%d:%d]\n", header.rid, service_name, method_name, arg.data_len - (p - arg.data), header.data_len);

        CommonSkeleton *skeleton = liq->service_manager->get_skeleton(service_name);
        if (!skeleton) {
            printf("ERROR no service for %s::%s[%d]\n", service_name, method_name, header.data_len);
            return;
        } 
        auto res = skeleton->handle(method_name, p, header.data_len);
        header.type = 2;
        header.data_len = res->ByteSize();
        uint8_t *buff = arg.data;
        memcpy(buff, &header, sizeof(header)); buff += sizeof(header);
        memcpy(buff, service_name, header.name_len); buff += header.name_len;
        memcpy(buff, method_name, header.method_len); buff += header.method_len;
        res->SerializeWithCachedSizesToArray(buff); buff += header.data_len;
        delete res;

        printf("call return[%d] %d:%d\n", header.rid, header.data_len, buff - arg.data);
        arg.ipc->send(arg.data, buff - arg.data);
    }
}
