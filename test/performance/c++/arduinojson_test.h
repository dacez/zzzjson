#include "../../test.h"
#include "ArduinoJson/src/ArduinoJson.h"

class arduinojsonTest : public Test
{
  public:
    arduinojsonTest(const std::string &path) : Test("arduinojson", path){};
    arduinojsonTest(const std::vector<std::string> &jsons) : Test("arduinojson", jsons){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        DynamicJsonBuffer jsonBuffer(4096);
        for (size_t i = 0; i < strlen(json); i++)
        {
            switch (json[i])
            {
            case '{':
            {
                uint64_t now = Now();
                JsonObject &o = jsonBuffer.parseObject(json + i);
                *ms = Now() - now;
                if (!o.success())
                {
                    return false;
                }
                else
                {
                    break;
                }
            }
            case '[':
            {
                uint64_t now = Now();
                JsonArray &o = jsonBuffer.parseArray(json + i);
                *ms = Now() - now;
                if (!o.success())
                {
                    return false;
                }
                else
                {
                    break;
                }
            }
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                continue;
            }
            break;
        }
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        std::stringstream os;
        ArduinoJson::Internals::StreamPrintAdapter adapter(os);
        DynamicJsonBuffer jsonBuffer(4096);
        for (size_t i = 0; i < strlen(json); i++)
        {
            switch (json[i])
            {
            case '{':
            {
                JsonObject &o = jsonBuffer.parseObject(json + i);
                if (!o.success())
                {
                    return false;
                }
                else
                {
                    uint64_t now = Now();
                    o.printTo(adapter);
                    *ms = Now() - now;
                    break;
                }
            }
            case '[':
            {
                JsonArray &o = jsonBuffer.parseArray(json + i);
                if (!o.success())
                {
                    return false;
                }
                else
                {
                    uint64_t now = Now();
                    o.printTo(adapter);
                    *ms = Now() - now;
                    break;
                }
            }
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                continue;
            }
            break;
        }
        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        {
            std::stringstream os;
            ArduinoJson::Internals::StreamPrintAdapter adapter(os);
            DynamicJsonBuffer jsonBuffer(4096);
            for (size_t i = 0; i < strlen(json); i++)
            {
                switch (json[i])
                {
                case '{':
                {
                    JsonObject &o = jsonBuffer.parseObject(json + i);
                    if (!o.success())
                    {
                        return false;
                    }
                    else
                    {
                        o.printTo(adapter);
                        break;
                    }
                }
                case '[':
                {
                    JsonArray &o = jsonBuffer.parseArray(json + i);
                    if (!o.success())
                    {
                        return false;
                    }
                    else
                    {
                        uint64_t now = Now();
                        o.printTo(adapter);
                        break;
                    }
                }
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    continue;
                }
                break;
            }
        }
        *ms = Now() - now;
        return true;
    }
};