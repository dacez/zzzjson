#include "../../test.h"

class strlenTest : public Test
{
  public:
    strlenTest(const std::string &path) : Test("strlen", path){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        if (strlen(json) == 0) return false;
        *ms = Now() - now;
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        if (strlen(json) == 0) return false;
        *ms = Now() - now;
        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        if (strlen(json) == 0) return false;
        else {
            if (strlen(json) == 0) return false;
        }
        *ms = Now() - now;
        return true;
    }
};