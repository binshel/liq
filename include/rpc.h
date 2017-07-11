#pragma once

#include <google/protobuf/message.h>

#include "liq.h"
#include "thread_pool.h"

namespace liq {

    class IPCBase {
    public:
        virtual int32_t send(const uint8_t *data, int32_t len) = 0;
    };


    class RPCManager;
    class RPC {
        public:
            RPC() = delete;
            RPC(RPCManager *manager, const std::string &name, IPCBase *ipc);

            virtual void call(const char *method, google::protobuf::Message &message,  uint8_t **resBuff, int32_t *resLen);

        private:
            RPCManager *manager;
            IPCBase *ipc;
            std::string name;
            static int32_t last_id;
    };
    

    class IPCShm;
    class RPCManager {
        friend class RPC;
    public:
        enum IPC_TYPE {
            IPC_SHM     = 1
        };

        RPCManager() = delete;
        RPCManager(const char *node_name, LiqState *liq);

        RPC* create_rpc(IPC_TYPE type, const char *name, const char *remote);
        IPCBase* create_ipc(IPC_TYPE type, const char *remote);
        int32_t ontick();

    private:
        struct BecallArg {
            RPCManager *manager;
            IPCBase *ipc;
            uint8_t *data;
            int32_t data_len;
        };
        struct ReturnArg {
            uint8_t *data;
            int32_t data_len;
        };
        static void on_becall(BecallArg *arg);

    protected:
        std::string node_name;
        LiqState *liq;
        std::map<std::string, IPCShm*> shms;
        uint8_t buff[2018];
    };


}
