#include "zzzjson.h"

namespace zzzjson
{
class Allocator : public zzz_Allocator
{
  public:
    Allocator()
    {
        zzz_AllocatorNew(this);
    }
    ~Allocator()
    {
        zzz_AllocatorRelease(this);
    }
};

class Value : public zzz_Value
{
  public:
  Value *NewValue(Allocator *alloc) {
      zzz_Value * v = zzz_ValueNew(alloc);
      N = v->N;
      A = v->A;
      CacheStr = v->CacheStr;
      Cache = v->Cache;
      CacheKey = v->CacheKey;
      CacheUnEscapeKey = v->CacheUnEscapeKey;
  }
  private:
    Value() {}
};
} // namespace zzzjson