#include "../../zzzjson.h"
#include <stdio.h>

// 深复制函数，这里命名为GetAndSet更直观
void GetAndSet(Value *srcv, Value *desv) {
  // 获取值的类型
  const JSONType *t;
  t = Type(srcv);
  if (t == 0)
    return;
  switch (*t) {
  case JSONTypeArray: {
    // 如果是数组，则把当前值设为数组，然后遍历并复制数组中的每个值
    SetArray(desv);
    Value *next = Begin(srcv);
    while (next != 0) {
      Value *v = NewValue(desv->A);
      GetAndSet(next, v);
      if (ArrayAddFast(desv, v) != true)
        return;
      next = Next(next);
    }
    break;
  }
  case JSONTypeObject: {
    // 如果是对象，则把当前值设为对象，然后遍历并复制对象中的每个值
    SetObj(desv);
    Value *next = Begin(srcv);
    while (next != 0) {
      Value *v = NewValue(desv->A);
      SetKeyFast(v, GetKey(next));
      GetAndSet(next, v);
      if (ObjAddFast(desv, v) != true)
        return;
      next = Next(next);
    }
    break;
  }
  case JSONTypeBool: {
    // 如果是布尔值，则获取并复制该值
    const bool *b = GetBool(srcv);
    if (b == 0)
      return;
    SetBool(desv, *b);
    break;
  }
  case JSONTypeNull: {
    // 如果是空，则复制该值
    if (IsNull(srcv) == false)
      return;
    SetNull(desv);
    break;
  }

  case JSONTypeString: {
    // 如果是字符串，则获取并复制该字符串
    const char *str = GetStr(srcv);
    if (str == 0)
      return;
    // 如果需要拷贝字符串，则需要使用SetStr
    if (SetStrFast(desv, str) != true)
      return;
    break;
  }
  case JSONTypeNumber: {
    // 如果是数字，则获取并复制该数字
    const char *str = GetNumStr(srcv);
    if (str == 0)
      return;
    // 如果需要拷贝数字，则需要使用SetNumStr
    if (SetNumStrFast(desv, str) != true)
      return;
    break;
  }
  }
}
int main() {
  const char *src_json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],"
                         "true],[\"\",123,\"str\"],null]";
  // 创建一个内存分配器
  Allocator *A = NewAllocator();
  // 创建两个值
  Value *src_v = NewValue(A);
  Value *des_v = NewValue(A);
  // 解析JSON文本
  bool ret = ParseFast(src_v, src_json);
  if (ret != true) {
    printf("ParseFast Fail!\n");
    return 1;
  }
  // 深复制
  GetAndSet(src_v, des_v);
  // 序列化
  const char *des_json = Stringify(des_v);
  printf("src_json:%s\n", src_json);
  if (des_json != 0)
    printf("des_json:%s\n", des_json);
  // 释放内存
  ReleaseAllocator(A);
  return 0;
}
