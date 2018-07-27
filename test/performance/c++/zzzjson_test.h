#include "../../test.h"

#define ZZZ_ALLOCATORINITMEMSIZE 4096 * 16
#define ZZZ_STRINGINITMEMSIZE 4096 * 4
#define ZZZ_DELTA 4
#define ZZZ_SHORT_API 0
#include "../../../zzzjson.h"

class zzzJSONTest : public Test
{
  public:
    zzzJSONTest(const std::vector<std::string> &paths) : Test("zzzJSON", paths){};
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