
#include "errorcode.h"
#include "shm_channel.h"
#include "rpc.h"
#include "service.h"


namespace liq {

    struct IPCData {
        uint32_t len;
        uint8_t *data;
    };
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
                memcpy(buff, data, len);
                shm_channel_recv_peek(this->channel);
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

        this->ipc->send(manager->buff, buff - manager->buff);
        IPCData *res = (IPCData*)manager->liq->thread_pool->yield(ThreadPool::EVENT_RPC, header.rid);
        *res_buff = res->data;
        *res_len = res->len;
        return;
    }

    int32_t RPC::last_id = 0;



    /** RPCManager **/
    RPCManager::RPCManager(const char *node_name, LiqState *liq) 
            : node_name(node_name), liq(liq) {
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
        IPCData data;
        for (auto it = this->shms.begin(); it != this->shms.end(); ++it) {
            int32_t len = it->second->recv(this->buff);
            if (len <= 0) continue;
            printf("shm len: %d\n", len);
            RPCHeader *header = (RPCHeader*)this->buff;
            if (header->type == 1) {        // becall
                this->on_becall(it->second, this->buff, len);
            } else if (header->type == 2) {    // call return
                int32_t id = *(int32_t*)this->buff;
                data.len = len;
                data.data = this->buff;
                liq->thread_pool->notify(ThreadPool::EVENT_RPC, id, &data);
            }

        }
    }

    void RPCManager::on_becall(IPCBase *ipc, uint8_t *data, int32_t len) {
        RPCHeader header = *(RPCHeader*)data;
        char service_name[256];
        char method_name[256];
        uint8_t *p = data + sizeof(header);
        strncpy(service_name, (const char*)p, header.name_len); p += header.name_len;
        strncpy(method_name, (const char*)p, header.method_len); p += header.method_len;
        printf("call method %s::%s[%d:%d]\n", service_name, method_name, len - (p - this->buff), header.data_len);

        CommonSkeleton *skeleton = liq->service_manager->get_skeleton(service_name);
        if (!skeleton) {
            printf("ERROR no service for %s::%s[%d]\n", service_name, method_name, header.data_len);
            return;
        } 
        auto res = skeleton->handle(method_name, p, header.data_len);
        header.type = 2;
        header.data_len = res->ByteSize();
        uint8_t *buff = this->buff;
        memcpy(buff, &header, sizeof(header)); buff += sizeof(header);
        memcpy(buff, service_name, header.name_len); buff += header.name_len;
        memcpy(buff, method_name, header.method_len); buff += header.method_len;
        res->SerializeWithCachedSizesToArray(buff); buff += header.data_len;
        delete res;

        ipc->send(this->buff, buff - this->buff);
    }
}
