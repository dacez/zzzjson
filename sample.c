#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "{\"key123\":123}";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ObjGet(v, "key123");
        SetKeyLenFast(vv, "key234", 6);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}