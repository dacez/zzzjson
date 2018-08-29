#include "../../test.h"

#define z_ALLOCATORINITMEMSIZE 4096 * 128
#define z_STRINGINITMEMSIZE 4096 * 2
#define z_DELTA 2                     
#define z_SHORT_API 0
#include "../../../beta/zzzjson_beta.h"

class zzzjsonbetaTest : public Test
{
  public:
    zzzjsonbetaTest(const std::string &path) : Test("zzzjsonbeta", path){};
    zzzjsonbetaTest(const std::vector<std::string> &jsons) : Test("zzzjsonbeta", jsons){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        z_Allocator *A = z_AllocatorNew();
        z_Value *v = z_ValueNew(A);
        uint64_t now = Now();
        z_BOOL ret = z_ValueParseFast(v, json);
        *ms = Now() - now;
        z_AllocatorRelease(A);
        if (ret == z_False) return false;
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        z_Allocator *A = z_AllocatorNew();
        z_Value *v = z_ValueNew(A);

        z_BOOL ret = z_ValueParseFast(v, json);
        if (ret == z_True) {
            uint64_t now = Now();
            const char *ret_json = z_ValueStringify(v);
            *ms = Now() - now;
            ret = (ret_json != 0);
        }
        z_AllocatorRelease(A);
        if (ret == z_False) return false;
        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        z_BOOL ret = z_True;
        const char *ret_json = 0;
        uint64_t now = Now();
        {
            z_Allocator *A = z_AllocatorNew();
            z_Value *v = z_ValueNew(A);
            ret = z_ValueParseFast(v, json);
            if (ret == z_True)
            {
                ret_json = z_ValueStringify(v);
            }
            z_AllocatorRelease(A);
        }
        *ms = Now() - now;
        if (ret == z_False || ret_json == 0)
            return false;
        return true;
    }
};