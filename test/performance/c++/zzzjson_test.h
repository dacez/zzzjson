#include "../../test.h"

#define ZZZ_DELTA 4                     
#define ZZZ_SHORT_API 0
#include "../../../zzzjson.h"

class zzzjsonTest : public Test
{
  public:
    zzzjsonTest(const std::string &path) : Test("zzzjson", path){};
    zzzjsonTest(const std::vector<std::string> &jsons) : Test("zzzjson", jsons){};
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        zzz_Allocator *A = zzz_AllocatorNew();
        zzz_Value *v = zzz_ValueNew(A);
        uint64_t now = Now();
        zzz_BOOL ret = zzz_ValueParseFast(v, json);
        *ms = Now() - now;
        zzz_AllocatorRelease(A);
        if (ret == zzz_False) return false;
        return true;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        zzz_Allocator *A = zzz_AllocatorNew();
        zzz_Value *v = zzz_ValueNew(A);

        zzz_BOOL ret = zzz_ValueParseFast(v, json);
        if (ret == zzz_True) {
            uint64_t now = Now();
            const char *ret_json = zzz_ValueStringify(v);
            *ms = Now() - now;
            ret = (ret_json != 0);
        }
        zzz_AllocatorRelease(A);
        if (ret == zzz_False) return false;
        return true;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        zzz_BOOL ret = zzz_True;
        const char *ret_json = 0;
        uint64_t now = Now();
        {
            zzz_Allocator *A = zzz_AllocatorNew();
            zzz_Value *v = zzz_ValueNew(A);
            ret = zzz_ValueParseFast(v, json);
            if (ret == zzz_True)
            {
                ret_json = zzz_ValueStringify(v);
            }
            zzz_AllocatorRelease(A);
        }
        *ms = Now() - now;
        if (ret == zzz_False || ret_json == 0)
            return false;
        return true;
    }
};