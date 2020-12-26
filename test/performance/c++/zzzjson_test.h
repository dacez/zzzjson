#include "../../test.h"

#define zj_ALLOCATORINITMEMSIZE 4096 * 128
#define zj_STRINGINITMEMSIZE 4096 * 2
#define zj_DELTA 2
#define zj_SHORT_API 0
#include "../../../zzzjson.h"

using namespace zj;

class zzzjsonTest : public Test {
public:
  zzzjsonTest(const std::string &path) : Test("zzzjson", path){};
  zzzjsonTest(const std::vector<std::string> &jsons) : Test("zzzjson", jsons){};
  virtual bool Parse(const char *json, unsigned long long *ms) {
    zj_Allocator *A = zj_NewAllocator();
    Value *v = zj_NewValue(A);
    uint64_t now = Now();
    bool ret = zj_ParseFast(v, json);
    *ms = Now() - now;
    zj_ReleaseAllocator(A);
    if (ret == false)
      return false;
    return true;
  }
  virtual bool Stringify(const char *json, unsigned long long *ms) {
    zj_Allocator *A = zj_NewAllocator();
    Value *v = zj_NewValue(A);

    bool ret = zj_ParseFast(v, json);
    if (ret == true) {
      uint64_t now = Now();
      const char *ret_json = zj_Stringify(v);
      *ms = Now() - now;
      ret = (ret_json != 0);
    }
    zj_ReleaseAllocator(A);
    if (ret == false)
      return false;
    return true;
  }
  virtual bool All(const char *json, unsigned long long *ms) {
    bool ret = true;
    const char *ret_json = 0;
    uint64_t now = Now();
    {
      zj_Allocator *A = zj_NewAllocator();
      zj_Value *v = zj_NewValue(A);
      ret = zj_ParseFast(v, json);
      if (ret == true) {
        ret_json = zj_Stringify(v);
      }
      zj_ReleaseAllocator(A);
    }
    *ms = Now() - now;
    if (ret == false || ret_json == 0)
      return false;
    return true;
  }
};
