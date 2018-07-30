#include "../../test.h"

#include "jsoncpp/include/json/json.h"

class jsoncppTest : public Test
{
  public:
    jsoncppTest(const std::string path) : Test("jsoncpp", path){};
    jsoncppTest(const std::vector<std::string> &jsons) : Test("jsoncpp", jsons){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        std::stringstream ss;
        ss << json;
        Json::Value root;
        Json::CharReaderBuilder rbuilder;
        rbuilder["collectComments"] = false;
        std::string errs;
        uint64_t now = Now();
        bool ok = Json::parseFromStream(rbuilder, ss, &root, &errs);
        *ms = Now() - now;
        if (ok)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        Json::Value root;
        Json::CharReaderBuilder rbuilder;
        rbuilder["collectComments"] = false;
        std::string errs;
        std::stringstream ss;
        ss << json;
        bool ok = Json::parseFromStream(rbuilder, ss, &root, &errs);
        if (ok)
        {
            Json::StreamWriterBuilder wbuilder;
            wbuilder["indentation"] = "";
            uint64_t now = Now();
            std::string ret = Json::writeString(wbuilder, root);
            *ms = Now() - now;
            return true;
        }
        else
        {
            return false;
        }
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        std::stringstream ss;
        ss << json;
        uint64_t now = Now();
        {
            Json::Value root;
            Json::CharReaderBuilder rbuilder;
            rbuilder["collectComments"] = false;
            std::string errs;
            bool ok = Json::parseFromStream(rbuilder, ss, &root, &errs);
            if (ok)
            {
                Json::StreamWriterBuilder wbuilder;
                wbuilder["indentation"] = "";
                std::string ret = Json::writeString(wbuilder, root);
            }
            else
            {
                return false;
            }
        }
        *ms = Now() - now;
        return true;
    }
};