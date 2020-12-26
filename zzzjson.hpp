#define zj_SHORT_API 0
#include "zzzjson.h"

namespace zzzjson {
typedef zj::Size SIZE;
typedef zj::JSONType JSONType;
static const JSONType JSONTypeArray = zj::JSONTypeArray;
static const JSONType JSONTypeObject = zj::JSONTypeObject;
static const JSONType JSONTypeString = zj::JSONTypeString;
static const JSONType JSONTypeNumber = zj::JSONTypeNumber;
static const JSONType JSONTypeBool = zj::JSONTypeBool;
static const JSONType JSONTypeNull = zj::JSONTypeNull;

class Allocator {
public:
  Allocator() { alloc = zj::zj_NewAllocator(); }
  ~Allocator() { zj::zj_ReleaseAllocator(alloc); }
  zj::zj_Allocator *getAllocator() { return alloc; }

private:
  Allocator(const Allocator &a) {}
  const Allocator &operator=(const Allocator &a) { return *this; }

private:
  zj::zj_Allocator *alloc;
};

class Value {
public:
  static Value *NewValue(Allocator *alloc) {
    zj::Value *v = zj::zj_NewValue(alloc->getAllocator());
    return innerNew(v);
  }
  Value *NewValue() {
    zj::Value *v = zj::zj_NewValue(val->A);
    Value *ret = (Value *)zj::zj_allocatorAlloc(val->A, sizeof(Value));
    ret->val = v;
    return ret;
  }
  bool ParseFast(const char *s) { return zj::zj_ParseFast(val, s); }
  bool ParseLen(const char *s, SIZE len) { return zj_ParseLen(val, s, len); }
  bool Parse(const char *s) { return zj_Parse(val, s); }
  const char *Stringify() { return zj_Stringify(val); }
  const char *GetStrFast(SIZE *len) { return zj::GetStrFast(val, len); }
  const char *GetStr() { return zj::GetStr(val); }
  const char *GetUnEscapeStr() { return zj::GetUnEscapeStr(val); }
  const char *GetNumFast(SIZE *len) { return zj::GetNumFast(val, len); }
  const char *GetNumStr() { return zj::GetNumStr(val); }
  const double *GetNum() { return zj::GetNum(val); }
  const double *GetDouble() { return zj::GetDouble(val); }
  const int *GetInt() { return zj::GetInt(val); }
  const long *GetLong() { return zj::GetLong(val); }
  const long long *GetLongLong() { return zj::GetLongLong(val); }
  const bool *GetBool() { return (const bool *)zj::GetBool(val); }
  const bool IsNull() { return zj::IsNull(val); }
  const char *GetKey() { return zj::GetKey(val); }
  const char *GetUnEscapeKey() { return zj::GetUnEscapeKey(val); }
  const char *GetKeyFast(zj::Size *len) { return zj::GetKeyFast(val, len); }
  Value *ObjGet(const char *key) {
    zj::Value *v = zj::ObjGet(val, key);
    return innerNew(v);
  }
  const Value *ObjGetLen(const char *key, SIZE len) {
    zj::Value *v = zj::ObjGetLen(val, key, len);
    return innerNew(v);
  }
  const JSONType *Type() { return zj::Type(val); }
  const SIZE Size() { return zj::SizeOf(val); }
  Value *ArrayGet(SIZE index) {
    zj::Value *v = zj::ArrayGet(val, index);
    return innerNew(v);
  }
  Value *Begin() {
    zj::Value *v = zj::Begin(val);
    return innerNew(v);
  }
  Value *Next() {
    zj::Value *v = zj::Next(val);
    return innerNew(v);
  }
  Value *Copy() {
    zj::Value *v = zj::Copy(val);
    return innerNew(v);
  }
  bool Move() { return zj::Move(val); }
  void SetNull() { return zj::SetNull(val); }
  void SetBool(bool b) { return zj::SetBool(val, b); }
  bool SetNumStr(const char *num) { return zj::SetNumStr(val, num); }
  bool SetNumStrLen(const char *num, SIZE len) {
    return zj::SetNumStrLen(val, num, len);
  }
  bool SetNumStrFast(const char *num) { return zj::SetNumStrFast(val, num); }
  bool SetNumStrLenFast(const char *num, SIZE len) {
    return zj::SetNumStrLen(val, num, len);
  }
  bool SetNum(const double d) { return zj::SetNum(val, d); }
  bool SetDouble(const double d) { return zj::SetDouble(val, d); }
  bool SetInt(int i) { return zj::SetInt(val, i); }
  bool SetLong(long l) { return zj::SetLong(val, l); }
  bool SetLongLong(long long ll) { return zj::SetLongLong(val, ll); }
  bool SetStrEscape(const char *str) { return zj::SetStrEscape(val, str); }
  bool SetStrLenEscape(const char *str, SIZE len) {
    return zj::SetStrLenEscape(val, str, len);
  }
  bool SetStrFast(const char *s) { return zj::SetStrFast(val, s); }
  bool SetStrLenFast(const char *s, SIZE len) {
    return zj::SetStrLenFast(val, s, len);
  }
  bool SetStr(const char *s) { return zj::SetStr(val, s); }
  bool SetStrLen(const char *s, SIZE len) { return zj::SetStrLen(val, s, len); }
  bool SetKeyEscape(const char *key) { return zj::SetKeyEscape(val, key); }
  bool SetKeyLenEscape(const char *key, SIZE len) {
    return zj::SetKeyLenEscape(val, key, len);
  }
  bool SetKeyFast(const char *key) { return zj::SetKeyFast(val, key); }
  bool SetKeyLenFast(const char *key, SIZE len) {
    return zj::SetKeyLenFast(val, key, len);
  }
  bool SetKey(const char *key) { return zj::SetKey(val, key); }
  bool SetKeyLen(const char *key, SIZE len) {
    return zj::SetKeyLen(val, key, len);
  }
  void SetArray() { return zj::SetArray(val); }
  void SetObj() { return zj::SetObj(val); }
  bool SetFast(Value *v) { return zj::SetFast(val, v->val); }
  bool Set(Value *v) { return zj::Set(val, v->val); }
  bool ObjAddFast(Value *v) { return zj::ObjAddFast(val, v->val); }
  bool ObjAdd(Value *v) { return zj::ObjAdd(val, v->val); }
  bool ArrayAddFast(Value *v) { return zj::ArrayAddFast(val, v->val); }
  bool ArrayAdd(Value *v) { return zj::ArrayAdd(val, v->val); }
  bool ArrayDel(SIZE index) { return zj::ArrayDel(val, index); }
  bool ObjDel(const char *key) { return zj::ObjDel(val, key); }

private:
  Value() {}
  static Value *innerNew(zj::Value *v) {
    if (zj_UNLIKELY(v == 0))
      return 0;
    zj::Allocator *A = v->A;
    Value *ret = (Value *)zj::zj_allocatorAlloc(A, sizeof(Value));
    ret->val = v;
    return ret;
  }
  zj::Value *val;
};
} // namespace zzzjson
