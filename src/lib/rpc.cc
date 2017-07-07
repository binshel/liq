
#include "shm_channel.h"
#include "rpc.h"


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
        IPCShm(const char *remote) {
            shm_channel_open(&this->channel, "", remote, 100*1000);
        }

        virtual int32_t send(const uint8_t *data, int32_t len) {
            shm_channel_send(this->channel, data, len);
        }
        virtual int32_t recv(uint8_t *buff) {
            const char *data = NULL;
            int len = 0;
            shm_channel_peek_recv(this->channel, &data, &len);
            memcpy(buff, data, len);
            shm_channel_recv_peek(this->channel);
            return len;
        }
        
    private:
        LPCHANNEL channel;
    };


    /** RPC **/
    RPC::RPC(RPCManager *manager, IPCBase *ipc, const std::string &name)
            :manager(manager), ipc(ipc), name(name) {
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
    RPCManager::RPCManager(LiqState *liq) 
            : liq(liq) {
    }
    RPC* RPCManager::create_rpc(IPC_TYPE type, const char *name, const char *remote) {
        IPCShm *ipc = new IPCShm(remote);
        this->shms[remote] = ipc;
        return new RPC(this, ipc, name);
    }

    int32_t RPCManager::ontick() {
        IPCData data;
        for (auto it = this->shms.begin(); it != this->shms.end(); ++it) {
            int32_t len = it->second->recv(this->buff);
            RPCHeader *header = (RPCHeader*)this->buff;
            if (header->type == 1) {        // becall
            } else if (header->type == 2) {    // call return
                int32_t id = *(int32_t*)this->buff;
                data.len = len;
                data.data = this->buff;
                liq->thread_pool->notify(ThreadPool::EVENT_RPC, id, &data);
            }

        }
    }
}
