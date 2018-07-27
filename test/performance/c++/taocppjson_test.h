#include "../../test.h"
#include "taocppjson/include/tao/json.hpp"

class taocppJSONTest : public Test
{
  public:
    taocppJSONTest(const std::vector<std::string> &paths) : Test("taocppJSON", paths){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
            uint64_t now = Now();
            const tao::json::value v = tao::json::from_string(json);
            *ms = Now() - now;
            return true;

    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
            const tao::json::value v = tao::json::from_string(json);
uint64_t now = Now();
            std::string ret = tao::json::to_string(v);
            *ms = Now() - now;
            return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        {
            const tao::json::value v = tao::json::from_string(json);
            std::string ret = tao::json::to_string(v);
        }
        *ms = Now() - now;
        return true;
    }
};