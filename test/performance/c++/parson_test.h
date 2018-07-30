#include "../../test.h"
#include "parson/parson.h"

class parsonTest : public Test
{
  public:
    parsonTest(const std::string &path) : Test("parson", path){};
    parsonTest(const std::vector<std::string> &jsons) : Test("parson", jsons){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        JSON_Value *v;
        v = json_parse_string(json);
        *ms = Now() - now;
        if (v == 0)
            return false;
        json_value_free(v);
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        JSON_Value *v;
        v = json_parse_string(json);
        if (v == 0)
            return false;
        uint64_t now = Now();
        char *s = json_serialize_to_string(v);
        *ms = Now() - now;
        json_free_serialized_string(s);
        json_value_free(v);
        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        {
            JSON_Value *v;
            v = json_parse_string(json);
            if (v == 0)
                return false;

            char *s = json_serialize_to_string(v);
            json_free_serialized_string(s);
            json_value_free(v);
        }
        *ms = Now() - now;
        return true;
    }
};