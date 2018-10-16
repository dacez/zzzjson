#define zzz_SHORT_API 0
#include "zzzjson.h"

namespace zzzjson
{
typedef zzz_SIZE SIZE;
typedef zzz_JSONType JSONType;
static const JSONType JSONTypeArray = zzz_JSONTYPEARRAY;
static const JSONType JSONTypeObject = zzz_JSONTYPEOBJECT;
static const JSONType JSONTypeString = zzz_JSONTYPESTRING;
static const JSONType JSONTypeNumber = zzz_JSONTYPENUMBER;
static const JSONType JSONTypeBool = zzz_JSONTYPEBOOL;
static const JSONType JSONTypeNull = zzz_JSONTYPENULL;

class Allocator
{
  public:
    Allocator()
    {
        alloc = zzz_AllocatorNew();
    }
    ~Allocator()
    {
        zzz_AllocatorRelease(alloc);
    }
    zzz_Allocator *getAllocator()
    {
        return alloc;
    }

  private:
    Allocator(const Allocator &a) {}
    const Allocator &operator=(const Allocator &a) {}

  private:
    zzz_Allocator *alloc;
};

class Value
{
  public:
    static Value *NewValue(Allocator *alloc)
    {
        zzz_Value *v = zzz_ValueNew(alloc->getAllocator());
        return innerNew(v);
    }
    bool ParseFast(const char *s)
    {
        return zzz_ValueParseFast(val, s);
    }
    bool ParseLen(const char *s, SIZE len)
    {
        return zzz_ValueParseLen(val, s, len);
    }
    bool Parse(const char *s)
    {
        return zzz_ValueParse(val, s);
    }
    const char *Stringify()
    {
        return zzz_ValueStringify(val);
    }
    const char *GetStrFast(SIZE *len)
    {
        return zzz_ValueGetStrFast(val, len);
    }
    const char *GetStr()
    {
        return zzz_ValueGetStr(val);
    }
    const char *GetUnEscapeStr()
    {
        return zzz_ValueGetUnEscapeStr(val);
    }
    const char *GetNumFast(SIZE *len)
    {
        return zzz_ValueGetNumFast(val, len);
    }
    const char *GetNumStr()
    {
        return zzz_ValueGetNumStr(val);
    }
    const double *GetNum()
    {
        return zzz_ValueGetNum(val);
    }
    const double *GetDouble()
    {
        return zzz_ValueGetDouble(val);
    }
    const int *GetInt()
    {
        return zzz_ValueGetInt(val);
    }
    const long *GetLong()
    {
        return zzz_ValueGetLong(val);
    }
    const long long *GetLongLong()
    {
        return zzz_ValueGetLongLong(val);
    }
    const bool *GetBool()
    {
        return (const bool *)zzz_ValueGetBool(val);
    }
    const bool IsNull()
    {
        return zzz_ValueIsNull(val);
    }
    const char *GetKey()
    {
        return zzz_ValueGetKey(val);
    }
    const char *GetUnEscapeKey()
    {
        return zzz_ValueGetUnEscapeKey(val);
    }
    const char *GetKeyFast(SIZE *len)
    {
        return zzz_ValueGetKeyFast(val, len);
    }
    Value *ObjGet(const char *key)
    {
        zzz_Value *v = zzz_ValueObjGet(val, key);
        return innerNew(v);
    }
    const Value *ObjGetLen(const char *key, SIZE len)
    {
        zzz_Value *v = zzz_ValueObjGetLen(val, key, len);
        return innerNew(v);
    }
    const JSONType *Type()
    {
        return zzz_ValueType(val);
    }
    const SIZE Size()
    {
        return zzz_ValueSize(val);
    }
    Value *ArrayGet(SIZE index)
    {
        zzz_Value *v = zzz_ValueArrayGet(val, index);
        return innerNew(v);
    }
    Value *Begin()
    {
        zzz_Value *v = zzz_ValueBegin(val);
        return innerNew(v);
    }
    Value *Next()
    {
        zzz_Value *v = zzz_ValueNext(val);
        return innerNew(v);
    }
    Value *Copy()
    {
        zzz_Value *v = zzz_ValueCopy(val);
        return innerNew(v);
    }
    bool Move()
    {
        return zzz_ValueMove(val);
    }
    bool SetNull()
    {
        return zzz_ValueSetNull(val);
    }
    bool SetBool(bool b)
    {
        return zzz_ValueSetBool(val, b);
    }
    bool SetNumStr(const char *num)
    {
        return zzz_ValueSetNumStr(val, num);
    }
    bool SetNumStrLen(const char *num, SIZE len)
    {
        return zzz_ValueSetNumStrLen(val, num, len);
    }
    bool SetNumStrFast(const char *num)
    {
        return zzz_ValueSetNumStrFast(val, num);
    }
    bool SetNumStrLenFast(const char *num, SIZE len)
    {
        return zzz_ValueSetNumStrLen(val, num, len);
    }
    bool SetNum(const double d)
    {
        return zzz_ValueSetNum(val, d);
    }
    bool SetDouble(const double d)
    {
        return zzz_ValueSetDouble(val, d);
    }
    bool SetInt(int i)
    {
        return zzz_ValueSetInt(val, i);
    }
    bool SetLong(long l)
    {
        return zzz_ValueSetLong(val, l);
    }
    bool SetLongLong(long long ll)
    {
        return zzz_ValueSetLongLong(val, ll);
    }
    bool SetStrEscape(const char *str)
    {
        return zzz_ValueSetStrEscape(val, str);
    }
    bool SetStrLenEscape(const char *str, SIZE len)
    {
        return zzz_ValueSetStrLenEscape(val, str, len);
    }
    bool SetStrFast(const char *s)
    {
        return zzz_ValueSetStrFast(val, s);
    }
    bool SetStrLenFast(const char *s, SIZE len)
    {
        return zzz_ValueSetStrLenFast(val, s, len);
    }
    bool SetStr(const char *s)
    {
        return zzz_ValueSetStr(val, s);
    }
    bool SetStrLen(const char *s, SIZE len)
    {
        return zzz_ValueSetStrLen(val, s, len);
    }
    bool SetKeyEscape(const char *key)
    {
        return zzz_ValueSetKeyEscape(val, key);
    }
    bool SetKeyLenEscape(const char *key, SIZE len)
    {
        return zzz_ValueSetKeyLenEscape(val, key, len);
    }
    bool SetKeyFast(const char *key)
    {
        return zzz_ValueSetKeyFast(val, key);
    }
    bool SetKeyLenFast(const char *key, SIZE len)
    {
        return zzz_ValueSetKeyLenFast(val, key, len);
    }
    bool SetKey(const char *key)
    {
        return zzz_ValueSetKey(val, key);
    }
    bool SetKeyLen(const char *key, SIZE len)
    {
        return zzz_ValueSetKeyLen(val, key, len);
    }
    bool SetArray()
    {
        return zzz_ValueSetArray(val);
    }
    bool SetObj()
    {
        return zzz_ValueSetObj(val);
    }
    bool SetFast(Value *v)
    {
        return zzz_ValueSetFast(val, v->val);
    }
    bool Set(Value *v)
    {
        return zzz_ValueSet(val, v->val);
    }
    bool ObjAddFast(Value *v)
    {
        return zzz_ValueObjAddFast(val, v->val);
    }
    bool ObjAdd(Value *v)
    {
        return zzz_ValueObjAdd(val, v->val);
    }
    bool ArrayAddFast(Value *v)
    {
        return zzz_ValueArrayAddFast(val, v->val);
    }
    bool ArrayAdd(Value *v)
    {
        return zzz_ValueArrayAdd(val, v->val);
    }
    bool ArrayDel(SIZE index)
    {
        return zzz_ValueArrayDel(val, index);
    }
    bool ObjDel(const char *key)
    {
        return zzz_ValueObjDel(val, key);
    }

  private:
    Value() {}
    static Value *innerNew(zzz_Value *v)
    {
        if (zzz_UNLIKELY(v == 0))
            return 0;
        zzz_Allocator *A = v->A;
        Value *ret = (Value *)zzz_AllocatorAlloc(A, sizeof(Value));
        ret->val = v;
        return ret;
    }
    zzz_Value *val;
};
} // namespace zzzjson