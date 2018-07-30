#include "../../test.h"

#include <iomanip>

#include "gason/src/gason.h"

class gasonTest : public Test
{
  public:
    gasonTest(const std::string &path) : Test("gason", path){};
    gasonTest(const std::vector<std::string> &jsons) : Test("gason", jsons){};
    
    void dumpString(std::ostringstream &os, const char *s)
    {
        os.put('"');
        while (*s)
        {
            char c = *s++;
            switch (c)
            {
            case '\b':
                os << "\\b";
                break;
            case '\f':
                os << "\\f";
                break;
            case '\n':
                os << "\\n";
                break;
            case '\r':
                os << "\\r";
                break;
            case '\t':
                os << "\\t";
                break;
            case '\\':
                os << "\\\\";
                break;
            case '"':
                os << "\\\"";
                break;
            default:
                os.put(c);
            }
        }
        os << s << "\"";
    }
    void dumpValue(std::ostringstream &os, const JsonValue &o, int shiftWidth, const std::string &linefeed = "", int indent = 0)
    {
        switch (o.getTag())
        {
        case JSON_NUMBER:
            char buffer[32];
            sprintf(buffer, "%f", o.toNumber());
            os << buffer;
            break;
        case JSON_TRUE:
            os << "true";
            break;
        case JSON_FALSE:
            os << "false";
            break;
        case JSON_STRING:
            dumpString(os, o.toString());
            break;
        case JSON_ARRAY:
            // It is not necessary to use o.toNode() to check if an array or object
            // is empty before iterating over its members, we do it here to allow
            // nicer pretty printing.
            if (!o.toNode())
            {
                os << "[]";
                break;
            }
            os << "[" << linefeed;
            for (auto i : o)
            {
                if (shiftWidth > 0)
                    os << std::setw(indent + shiftWidth) << " " << std::setw(0);
                dumpValue(os, i->value, shiftWidth, linefeed, indent + shiftWidth);
                if (i->next)
                    os << ",";
                os << linefeed;
            }
            if (indent > 0)
                os << std::setw(indent) << " " << std::setw(0);
            os.put(']');
            break;
        case JSON_OBJECT:
            if (!o.toNode())
            {
                os << "{}";
                break;
            }
            os << "{" << linefeed;
            for (auto i : o)
            {
                if (shiftWidth > 0)
                    os << std::setw(indent + shiftWidth) << " " << std::setw(0);
                dumpString(os, i->key);
                os << ":";
                dumpValue(os, i->value, shiftWidth, linefeed, indent + shiftWidth);
                if (i->next)
                    os << ",";
                os << linefeed;
            }
            if (indent > 0)
                os << std::setw(indent) << " " << std::setw(0);
            os.put('}');
            break;
        case JSON_NULL:
            os << "null";
            break;
        }
    }
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        char *endptr;
        JsonValue value;
        JsonAllocator allocator;
        char *s = new char[strlen(json) + 1];
        s[strlen(json)] = 0;
        memcpy(s, json, strlen(json));

        uint64_t now = Now();
        int status = jsonParse(s, &endptr, &value, allocator);
        *ms = Now() - now;

        if (status != JSON_OK)
            return false;
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        char *endptr;
        JsonValue value;
        JsonAllocator allocator;
        char *s = new char[strlen(json) + 1];
        s[strlen(json)] = 0;
        memcpy(s, json, strlen(json));
        int status = jsonParse(s, &endptr, &value, allocator);
        if (status != JSON_OK)
            return false;
        std::ostringstream os;

        uint64_t now = Now();
        dumpValue(os, value, 0);
        *ms = Now() - now;

        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        uint64_t now = Now();
        char *s = new char[strlen(json) + 1];
        s[strlen(json)] = 0;
        memcpy(s, json, strlen(json));
        {
            char *endptr;
            JsonValue value;
            JsonAllocator allocator;
            int status = jsonParse(s, &endptr, &value, allocator);
            if (status != JSON_OK)
                return false;
            std::ostringstream os;

            dumpValue(os, value, 0);
        }
        *ms = Now() - now;
        return true;
    }
};