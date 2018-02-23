#pragma once

#include <google/protobuf/message.h>

#include "liq.h"
#include "thread_pool.h"
// #include "shm_channel.h"

class tagChannel;
typedef tagChannel *LPCHANNEL;

namespace liq {

    enum RPC_TYPE {
        RPC_TWOWAY = 0,
        RPC_RETURN = 1,
        RPC_ERROR  = 2,
        RPC_ONEWAY = 3
    };      

    class IPCBase 
    {
        public:
            virtual int32_t send(const uint8_t *data, int32_t len) = 0;
            virtual int32_t recv(uint8_t *buff) = 0;
    };
    class IPCShm : public IPCBase 
    {
        public:
            IPCShm() = delete;
            IPCShm(const std::string &from, const std::string &remote,
                    const int channelsize = 100*1000);

            virtual int32_t send(const uint8_t *data, int32_t len);
            virtual int32_t recv(uint8_t *buff);

        private:
            LPCHANNEL channel;
    };


    class RPC 
    {
        public:
            struct BecallArg {
                IPCBase *ipc;
                uint8_t *data;
                int32_t data_len;
            };
            struct ReturnArg {
                int32_t ecode;
                uint8_t *data;
                int32_t data_len;
            };
        public:
            RPC() = delete;
            RPC(const std::string &name, IPCBase *ipc);

            virtual void call(RPC_TYPE type, const char *method, google::protobuf::Message &message,  uint8_t **resBuff, int32_t *resLen);

        private:
            std::string name;
            IPCBase *ipc;
            static int64_t last_id;
    };




    class RPCManager 
    {
        public:
            RPCManager() = delete;
            RPCManager(const std::string &node_name);

            RPC* create_rpc(const char *name, const char *remote);
            IPCBase* create_ipc(const char *remote);
            int32_t handle_ipc(IPCBase *ipc);
            int32_t ontick();

        private:
            void on_becall(RPC::BecallArg *arg);

        protected:
            std::string node_name;
            std::map<std::string, IPCBase*> ipcs;
            uint8_t buff[2018];
    };


}
