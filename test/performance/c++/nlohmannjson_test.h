#include "../../test.h"
#include "nlohmannjson/single_include/nlohmann/json.hpp"

class nlohmannjsonTest : public Test
{
  public:
    nlohmannjsonTest(const std::string &path) : Test("nlohmannjson", path){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        try
        {
            uint64_t now = Now();
            nlohmann::json j = nlohmann::json::parse(json);
            *ms = Now() - now;
        }
        catch (...)
        {
            return false;
        }
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        try
        {
            nlohmann::json j = nlohmann::json::parse(json);
            uint64_t now = Now();
            j.dump();
            *ms = Now() - now;
        }
        catch (...)
        {
            return false;
        }
        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        {
            try
            {
                nlohmann::json j = nlohmann::json::parse(json);
                j.dump();
            }
            catch (...)
            {
                return false;
            }
            *ms = Now() - now;
        }
        return true;
    }
};