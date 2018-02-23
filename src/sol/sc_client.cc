
#include <jsoncpp/json/reader.h>

#include "liq/logger.h"     // 引用日志功能
#include "liq/service.h"
#include "liq/timer.h"      // 引用定时器功能

#include "sol/sc_client.h"  // 生成的接口定义文件
#include "sc_agent.h"       // 依赖的其他模块

// 命名空间，与protobuf文件的 package 对应
namespace sol {

    // 实现了 ScClient 接口的类
    class ScClientImp : public ScClient
    {
    public:
        // onload 函数在实例被加载是调用
        int onload(Json::Value &cfg) {
            // nothing todo
        }
        // oninit 函数初始化实例状态
        int oninit(
                Json::Value &cfg, 
                std::map<std::string, CommonService*> &deps)
        {
            this->agent = (ScAgent*)deps["sc_agent"];

            liq::Liq::instance().timer->set_timer(1, std::bind(&ScClientImp::get_cfg_from_server, this));

            return 0;
        }
        // sc_client 的功能函数，不对外访问所以不需要在protobuf文件中定义
        void get_cfg_from_server()
        {
            std::string *method = new std::string("container_query");
            std::string *name = new std::string(liq::Liq::instance().node_name());
            String *str = NULL;
            try {
                str = this->agent->call_sc_server(method, name);
            } catch (const char *e) {
                INFOF("get config from server fail [%s]", e);
                liq::Liq::instance().timer->set_timer(10, std::bind(&ScClientImp::get_cfg_from_server, this));
                return;
            } catch (const std::exception &e) {
                INFOF("get config from server fail [%s]", e.what());
                liq::Liq::instance().timer->set_timer(10, std::bind(&ScClientImp::get_cfg_from_server, this));
                return;
            }
            DEBUG << str->str() << EOL;

            Json::Reader reader;
            Json::Value cfgroot;
            if (!reader.parse(str->str(), cfgroot)) {
                ERROR << "parse sc_server config error: " << reader.getFormattedErrorMessages() << EOL;
                exit(-1);
            } else {
                liq::Liq::instance().service->load_cfg(cfgroot);
            }
        }

        // 实现 ScClient 的 stop 接口
        virtual inline Null *stop(int64_t ts) {
            INFO << __FUNCTION__ << ts << EOL;
            exit(0);
            return NULL;
        }
        // 实现 ScClient 的 reload 接口
        virtual inline Null *reload(int64_t ts) {
            DEBUG << __FUNCTION__ << ts << EOL;
            return NULL;
        }
        // 实现 ScClient 的 stat 接口
        virtual inline ScStat *stat() {
            return NULL;
        }
        // 实现 ScClient 的 heartbeat 接口
        virtual inline Null *heartbeat(int64_t ts) {
            DEBUG << __FUNCTION__ << ts << EOL;
            return NULL;
        }
        // 实现 ScClient 的 update_by_server 接口
        virtual inline Null *update_by_server(string* cfg, int64_t ts) {
            DEBUG << __FUNCTION__ << ts << EOL;
            return NULL;
        }
    private:
        // 对依赖项的引用，可能是本地服务也可能是外部服务的代理stub
        ScAgent *agent;
    };

    extern "C" {
        // 固定的 create_service 函数，用于产生该类的实例
        CommonService* create_service() {
            return new ScClientImp();
        }
    }
}

