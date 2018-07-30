#include "../../test.h"
#define RAPIDJSON_SSE2

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/prettywriter.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
#include "rapidjson/include/rapidjson/writer.h"

class rapidjsonfpTest : public Test
{
  public:
    rapidjsonfpTest(const std::string &path) : Test("rapidjsonfp", path){};
    rapidjsonfpTest(const std::vector<std::string> &jsons) : Test("rapidjsonfp", jsons){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        rapidjson::Document doc;
        uint64_t now = Now();
        doc.Parse<rapidjson::kParseFullPrecisionFlag>(json);
        *ms = Now() - now;
        if (doc.HasParseError())
        {
            return false;
        }
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseFullPrecisionFlag>(json);
        if (doc.HasParseError())
        {
            return false;
        }
        rapidjson::StringBuffer buf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
        uint64_t now = Now();
        doc.Accept(writer);
        *ms = Now() - now;
        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        {
            rapidjson::Document doc;
            doc.Parse<rapidjson::kParseFullPrecisionFlag>(json);
            if (doc.HasParseError() == false)
            {
                rapidjson::StringBuffer buf;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
                doc.Accept(writer);
            } else {
                return false;
            }
        }
        *ms = Now() - now;
        return true;
    }
};