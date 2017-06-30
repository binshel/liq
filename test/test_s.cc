#include "pb/test_s.h"

namespace test {
    class Search : public SearchService {


    };

    extern "C" {
        CommonService* create_module() {
            return new Search();
        }
    }
}

