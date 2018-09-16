#include "../../zzzjson.hpp"

using namespace zzzjson;

// 深复制函数，这里命名为GetAndSet更直观
void GetAndSet(Allocator *A, Value *srcv, Value *desv)
{
    // 获取值的类型
    const JSONType *t;
    t = srcv->Type();
    if (t == 0)
        return;
    switch (*t)
    {
    case JSONTypeArray:
    {
        // 如果是数组，则把当前值设为数组，然后遍历并复制数组中的每个值
        desv->SetArray();
        Value *next = srcv->Begin();
        while (next != 0)
        {
            Value *v = Value::NewValue(A);
            GetAndSet(A, next, v);
            if (desv->ArrayAddFast(v) != true)
                return;
            next = next->Next();
        }
        break;
    }
    case JSONTypeObject:
    {
        // 如果是对象，则把当前值设为对象，然后遍历并复制对象中的每个值
        desv->SetObj();
        Value *next = srcv->Begin();
        while (next != 0)
        {
            Value *v = Value::NewValue(A);
            SIZE kl = 0;
            const char *key = next->GetKeyFast(&kl);
            if (key == 0) return;
            v->SetKeyLenFast(key, kl);
            GetAndSet(A, next, v);
            if (desv->ObjAddFast(v) != true)
                return;
            next = next->Next();
        }
        break;
    }
    case JSONTypeBool:
    {
        // 如果是布尔值，则获取并复制该值
        const bool *b = srcv->GetBool();
        if (b == 0)
            return;
        desv->SetBool(*b);
        break;
    }
    case JSONTypeNull:
    {
        // 如果是空，则复制该值
        if (srcv->IsNull() == false)
            return;
        desv->SetNull();
        break;
    }

    case JSONTypeString:
    {
        // 如果是字符串，则获取并复制该字符串
        SIZE sl = 0;
        const char *str = srcv->GetStrFast(&sl);
        if (str == 0)
            return;
        // 如果需要拷贝字符串，则需要使用SetStr
        if (desv->SetStrLenFast(str, sl) != true)
            return;
        break;
    }
    case JSONTypeNumber:
    {
        // 如果是数字，则获取并复制该数字
        SIZE nl = 0;
        const char *str = srcv->GetNumFast(&nl);
        if (str == 0)
            return;
        // 如果需要拷贝数字，则需要使用SetNumStr
        if (desv->SetNumStrLenFast(str, nl) != true)
            return;
        break;
    }
    }
}
int main()
{
    const char *src_json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"],null]";
    // 创建一个内存分配器
    Allocator A;
    // 创建两个值
    Value *src_v = Value::NewValue(&A);
    Value *des_v = Value::NewValue(&A);
    // 解析JSON文本
    bool ret = src_v->ParseFast(src_json);
    if (ret != true)
    {
        printf("ParseFast Fail!\n");
        return 1;
    }
    // 深复制
    GetAndSet(&A, src_v, des_v);
    // 序列化
    const char *des_json = des_v->Stringify();
    printf("src_json:%s\n", src_json);
    if (des_json != 0)
        printf("des_json:%s\n", des_json);
    return 0;
}