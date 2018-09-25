#include "../../zzzjson.h"

int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    long long d = -123;
    SetLongLong(v, d);
    const char *v_str = Stringify(v);
    if (v_str != 0)
        printf("%s\n", v_str);
    ReleaseAllocator(A);
}