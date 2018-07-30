#include "../../test.h"
#include "picojson/picojson.h"

class picojsonTest : public Test
{
  public:
    picojsonTest(const std::string &path) : Test("picojson", path){};
    picojsonTest(const std::vector<std::string> &jsons) : Test("picojson", jsons){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        uint32_t len = strlen(json);
        uint64_t now = Now();
        std::string err;
        picojson::value v;
        picojson::parse(v, json, json + len, &err);
        *ms = Now() - now;
        if (err.empty())
        {
            return true;
        }
        return false;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        std::string err;
        picojson::value v;
        picojson::parse(v, json, json + strlen(json), &err);
        if (err.empty())
        {
            uint64_t now = Now();
            v.serialize();
            *ms = Now() - now;
            return true;
        }
        return false;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        {
            std::string err;
            picojson::value v;
            picojson::parse(v, json, json + strlen(json), &err);
            if (err.empty())
            {
                v.serialize();
            } else {
                return false;
            }
        }
        *ms = Now() - now;
        return true;
    }
};