#include "zzzjson.h"

int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "{\"key123\":123}";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ObjGetLen(v, "key123", 6);
        if (vv != 0)
        {
            SetNumStr(vv, "1234");
            const char *ret_json = Stringify(v);
            printf("%s\n", ret_json);
        }
    }
    ReleaseAllocator(A);
}