#ifndef zzz_JSON_H
#define zzz_JSON_H

#include <string.h> // 使用其 memcpy 函数
#include <stdlib.h> // 使用其 atof malloc free 函数
#include <stdio.h>  // 使用其 sprintf 函数
#include <stdint.h> // 使用其 uint32_t

// 长命名的 类型 & 常量 & 用作常量的宏，详见《数据结构》
#ifndef zzz_SIZETYPE
#define zzz_SIZETYPE uint32_t
#endif
typedef zzz_SIZETYPE zzz_SIZE;

typedef char zzz_BOOL;
static const zzz_BOOL zzz_True = 1;
static const zzz_BOOL zzz_False = 0;

typedef char zzz_JSONType;
#define zzz_JSONTYPEBOOL 1
#define zzz_JSONTYPEARRAY 2
#define zzz_JSONTYPEOBJECT 3
#define zzz_JSONTYPESTRING 4
#define zzz_JSONTYPENULL 5
#define zzz_JSONTYPENUMBER 6
static const zzz_JSONType zzz_JSONTypeArray = zzz_JSONTYPEARRAY;
static const zzz_JSONType zzz_JSONTypeObject = zzz_JSONTYPEOBJECT;
static const zzz_JSONType zzz_JSONTypeString = zzz_JSONTYPESTRING;
static const zzz_JSONType zzz_JSONTypeNumber = zzz_JSONTYPENUMBER;
static const zzz_JSONType zzz_JSONTypeBool = zzz_JSONTYPEBOOL;
static const zzz_JSONType zzz_JSONTypeNull = zzz_JSONTYPENULL;

// 长命名的固字符串，详见《数据结构》
static const char *zzz_StrTrue = "true";
static const char *zzz_StrFalse = "false";
static const char *zzz_StrNull = "null";

// 内存配置，详见《内存配置》
#ifndef zzz_DELTA
#define zzz_DELTA 2
#endif
static const zzz_SIZE zzz_Delta = zzz_DELTA;

#ifndef zzz_ALLOCATORINITMEMSIZE
#define zzz_ALLOCATORINITMEMSIZE 1024 * 4
#endif
static const zzz_SIZE zzz_AllocatorInitMemSize = zzz_ALLOCATORINITMEMSIZE;

// Stringify函数初始化字符串的大小
#ifndef zzz_STRINGINITMEMSIZE
#define zzz_STRINGINITMEMSIZE 1024
#endif
static const zzz_SIZE zzz_StringInitMemSize = zzz_STRINGINITMEMSIZE;

// 用zzz_String作为Cache时初始化内存的大小，后面可能去掉
#ifndef zzz_STRINGCACHEINITMEMSIZE
#define zzz_STRINGCACHEINITMEMSIZE 128
#endif
static const zzz_SIZE zzz_StringCacheInitMemSize = zzz_STRINGCACHEINITMEMSIZE;

// 环境适配

#ifndef zzz_MEMORY_MODE
#define zzz_MEMORY_MODE 1
#endif

#if zzz_MEMORY_MODE == 1

static inline void *zzz_New(zzz_SIZE size)
{
    return malloc(size);
}
static inline void zzz_Free(void *pointer) { free(pointer); }

#elif zzz_MEMORY_MODE == 2
// 测试模式，主要是用来测试内存的分配数和释放数，防止内存泄漏，同时用于观察内存分配次数。
// 通过观察，可以得出较好zzz_DELTA和zzz_ALLOCATORINITMEMSIZE。

static zzz_SIZE AllocMemorySize = 0, AllocMemoryCount = 0, FreeMemoryCount = 0;
static inline void *zzz_New(zzz_SIZE size) { return AllocMemorySize += size, AllocMemoryCount += 1, malloc(size); }
static inline void zzz_Free(void *ptr) { FreeMemoryCount += 1, free(ptr); }

#elif zzz_MEMORY_MODE == 3
// 自定义模式，需要实现zzz_New，zzz_Free和zzz_Copy三个函数
#endif

// 分支预测
#ifndef zzz_EXPECT_MODE
#if defined(__GNUC__) || defined(__clang__)
#define zzz_EXPECT_MODE 1
#else
#define zzz_EXPECT_MODE 2
#endif
#endif

#if zzz_EXPECT_MODE == 1
// gcc和clang，使用__builtin_expect
#define zzz_LIKELY(x) __builtin_expect(x, 1)
#define zzz_UNLIKELY(x) __builtin_expect(x, 0)
#elif zzz_EXPECT_MODE == 2
// msvc 不需要分支优化
#define zzz_LIKELY(x) x
#define zzz_UNLIKELY(x) x
#elif zzz_EXPECT_MODE == 3
// 自定义分支预测
#endif

// 长命名数据结构，详见《数据结构》
struct zzz_Allocator;
struct zzz_Value;

// 长命名数据结构，详见《API》
static inline struct zzz_Allocator *zzz_AllocatorNew();
static inline void zzz_AllocatorRelease(struct zzz_Allocator *root_alloc);
static inline struct zzz_Value *zzz_ValueNew(struct zzz_Allocator *alloc);

static inline zzz_BOOL zzz_ValueParseFast(struct zzz_Value *v, const char *s);
static inline zzz_BOOL zzz_ValueParseLen(struct zzz_Value *v, const char *s, zzz_SIZE len);
static inline zzz_BOOL zzz_ValueParse(struct zzz_Value *v, const char *s);

static inline const char *zzz_ValueStringify(const struct zzz_Value *v);

static inline const char *zzz_ValueGetStrFast(const struct zzz_Value *v, zzz_SIZE *len);
static inline const char *zzz_ValueGetUnEscapeStr(struct zzz_Value *v);
static inline const char *zzz_ValueGetStr(struct zzz_Value *v);

static inline const char *zzz_ValueGetNumFast(const struct zzz_Value *v, zzz_SIZE *len);
static inline const char *zzz_ValueGetNumStr(struct zzz_Value *v);
static inline const double *zzz_ValueGetNum(struct zzz_Value *v);

static inline const zzz_BOOL *zzz_ValueGetBool(const struct zzz_Value *v);

static inline zzz_BOOL zzz_ValueIsNull(const struct zzz_Value *v);

static inline const char *zzz_ValueGetKey(struct zzz_Value *v);
static inline const char *zzz_ValueGetUnEscapeKey(struct zzz_Value *v);
static inline const char *zzz_ValueGetKeyFast(const struct zzz_Value *v, zzz_SIZE *len);

static inline struct zzz_Value *zzz_ValueObjGet(const struct zzz_Value *v, const char *key);
static inline struct zzz_Value *zzz_ValueObjGetLen(const struct zzz_Value *v, const char *key, zzz_SIZE len);

static inline const zzz_JSONType *zzz_ValueType(const struct zzz_Value *v);

static inline zzz_SIZE zzz_ValueSize(const struct zzz_Value *v);

static inline struct zzz_Value *zzz_ValueArrayGet(const struct zzz_Value *v, zzz_SIZE index);

static inline struct zzz_Value *zzz_ValueBegin(const struct zzz_Value *v);
static inline struct zzz_Value *zzz_ValueNext(const struct zzz_Value *v);

static inline struct zzz_Value *zzz_ValueCopy(const struct zzz_Value *v);
static inline zzz_BOOL zzz_ValueMove(struct zzz_Value *v);

static inline void zzz_ValueSetNull(struct zzz_Value *v);

static inline void zzz_ValueSetBool(struct zzz_Value *v, zzz_BOOL b);

static inline zzz_BOOL zzz_ValueSetNumStrFast(struct zzz_Value *v, const char *num);
static inline zzz_BOOL zzz_ValueSetNumStrLenFast(struct zzz_Value *v, const char *num, zzz_SIZE len);
static inline zzz_BOOL zzz_ValueSetNumStr(struct zzz_Value *v, const char *num);
static inline zzz_BOOL zzz_ValueSetNumStrLen(struct zzz_Value *v, const char *num, zzz_SIZE len);
static inline zzz_BOOL zzz_ValueSetNum(struct zzz_Value *v, const double d);

static inline zzz_BOOL zzz_ValueSetStrFast(struct zzz_Value *v, const char *str);
static inline zzz_BOOL zzz_ValueSetStrLenFast(struct zzz_Value *v, const char *str, zzz_SIZE len);
static inline zzz_BOOL zzz_ValueSetStr(struct zzz_Value *v, const char *str);
static inline zzz_BOOL zzz_ValueSetStrLen(struct zzz_Value *v, const char *str, zzz_SIZE len);
static inline zzz_BOOL zzz_ValueSetStrEscape(struct zzz_Value *v, const char *str);
static inline zzz_BOOL zzz_ValueSetStrLenEscape(struct zzz_Value *v, const char *str, zzz_SIZE len);

static inline zzz_BOOL zzz_ValueSetKeyFast(struct zzz_Value *v, const char *key);
static inline zzz_BOOL zzz_ValueSetKeyLenFast(struct zzz_Value *v, const char *key, zzz_SIZE len);
static inline zzz_BOOL zzz_ValueSetKey(struct zzz_Value *v, const char *key);
static inline zzz_BOOL zzz_ValueSetKeyLen(struct zzz_Value *v, const char *key, zzz_SIZE len);
static inline zzz_BOOL zzz_ValueSetKeyEscape(struct zzz_Value *v, const char *key);
static inline zzz_BOOL zzz_ValueSetKeyLenEscape(struct zzz_Value *v, const char *key, zzz_SIZE len);

static inline void zzz_ValueSetArray(struct zzz_Value *v);

static inline void zzz_ValueSetObj(struct zzz_Value *v);

static inline zzz_BOOL zzz_ValueSetFast(struct zzz_Value *v, struct zzz_Value *vv);
static inline zzz_BOOL zzz_ValueSet(struct zzz_Value *v, const struct zzz_Value *vv);

static inline zzz_BOOL zzz_ValueObjAddFast(struct zzz_Value *v, struct zzz_Value *vv);
static inline zzz_BOOL zzz_ValueObjAdd(struct zzz_Value *v, const struct zzz_Value *vv);

static inline zzz_BOOL zzz_ValueArrayAddFast(struct zzz_Value *v, struct zzz_Value *vv);
static inline zzz_BOOL zzz_ValueArrayAdd(struct zzz_Value *v, const struct zzz_Value *vv);

static inline zzz_BOOL zzz_ValueArrayDel(struct zzz_Value *v, zzz_SIZE index);

static inline zzz_BOOL zzz_ValueObjDel(struct zzz_Value *v, const char *key);

// 短命名开关，默认开
#ifndef zzz_SHORT_API
#define zzz_SHORT_API 1
#endif
#if zzz_SHORT_API == 1

// 短命名数据结构，详见《数据结构》
typedef struct zzz_Allocator Allocator;
typedef struct zzz_Value Value;

typedef zzz_SIZE SIZE;

typedef zzz_BOOL BOOL;
static const BOOL True = 1;
static const BOOL False = 0;

typedef zzz_JSONType JSONType;
#define JSONTYPEBOOL zzz_JSONTYPEBOOL
#define JSONTYPEARRAY zzz_JSONTYPEARRAY
#define JSONTYPEOBJECT zzz_JSONTYPEOBJECT
#define JSONTYPESTRING zzz_JSONTYPESTRING
#define JSONTYPENULL zzz_JSONTYPENULL
#define JSONTYPENUMBER zzz_JSONTYPENUMBER
static const JSONType JSONTypeArray = JSONTYPEARRAY;
static const JSONType JSONTypeObject = JSONTYPEOBJECT;
static const JSONType JSONTypeString = JSONTYPESTRING;
static const JSONType JSONTypeNumber = JSONTYPENUMBER;
static const JSONType JSONTypeBool = JSONTYPEBOOL;
static const JSONType JSONTypeNull = JSONTYPENULL;

// 短命名API，详见《API》
static inline Allocator *NewAllocator()
{
    return zzz_AllocatorNew();
}
static inline void ReleaseAllocator(Allocator *root_alloc)
{
    zzz_AllocatorRelease(root_alloc);
}
static inline Value *NewValue(Allocator *alloc)
{
    return zzz_ValueNew(alloc);
}
static inline BOOL ParseFast(Value *v, const char *s)
{
    return zzz_ValueParseFast(v, s);
}
static inline BOOL ParseLen(Value *v, const char *s, SIZE len)
{
    return zzz_ValueParseLen(v, s, len);
}
static inline BOOL Parse(Value *v, const char *s)
{
    return zzz_ValueParse(v, s);
}
static inline const char *Stringify(const Value *v)
{
    return zzz_ValueStringify(v);
}
static inline const char *GetStrFast(const Value *v, SIZE *len)
{
    return zzz_ValueGetStrFast(v, len);
}
static inline const char *GetUnEscapeStr(Value *v)
{
    return zzz_ValueGetUnEscapeStr(v);
}
static inline const char *GetStr(Value *v)
{
    return zzz_ValueGetStr(v);
}
static inline const char *GetNumFast(const Value *v, zzz_SIZE *len)
{
    return zzz_ValueGetNumFast(v, len);
}
static inline const char *GetNumStr(Value *v)
{
    return zzz_ValueGetNumStr(v);
}
static inline const double *GetNum(Value *v)
{
    return zzz_ValueGetNum(v);
}
static inline const BOOL *GetBool(const Value *v)
{
    return zzz_ValueGetBool(v);
}
static inline BOOL IsNull(const Value *v)
{
    return zzz_ValueIsNull(v);
}
static inline const char *GetKey(Value *v)
{
    return zzz_ValueGetKey(v);
}
static inline const char *GetUnEscapeKey(Value *v)
{
    return zzz_ValueGetUnEscapeKey(v);
}
static inline const char *GetKeyFast(const Value *v, SIZE *len)
{
    return zzz_ValueGetKeyFast(v, len);
}
static inline Value *ObjGet(const Value *v, const char *key)
{
    return zzz_ValueObjGet(v, key);
}
static inline Value *ObjGetLen(const Value *v, const char *key, SIZE len)
{
    return zzz_ValueObjGetLen(v, key, len);
}
static inline const JSONType *Type(const Value *v)
{
    return zzz_ValueType(v);
}
static inline SIZE Size(const Value *v)
{
    return zzz_ValueSize(v);
}
static inline Value *ArrayGet(const Value *v, SIZE index)
{
    return zzz_ValueArrayGet(v, index);
}
static inline Value *Begin(const Value *v)
{
    return zzz_ValueBegin(v);
}
static inline Value *Next(const Value *v)
{
    return zzz_ValueNext(v);
}
static inline Value *Copy(const Value *v)
{
    return zzz_ValueCopy(v);
}
static inline BOOL Move(Value *v)
{
    return zzz_ValueMove(v);
}
static inline void SetNull(Value *v)
{
    zzz_ValueSetNull(v);
}
static inline void SetBool(Value *v, BOOL b)
{
    zzz_ValueSetBool(v, b);
}
static inline BOOL SetNumStrFast(Value *v, const char *num)
{
    return zzz_ValueSetNumStrFast(v, num);
}
static inline BOOL SetNumStrLenFast(Value *v, const char *num, SIZE len)
{
    return zzz_ValueSetNumStrLenFast(v, num, len);
}
static inline BOOL SetNumStr(Value *v, const char *num)
{
    return zzz_ValueSetNumStr(v, num);
}
static inline BOOL SetNumStrLen(Value *v, const char *num, SIZE len)
{
    return zzz_ValueSetNumStrLen(v, num, len);
}
static inline BOOL SetNum(Value *v, const double d)
{
    return zzz_ValueSetNum(v, d);
}
static inline BOOL SetStrFast(Value *v, const char *str)
{
    return zzz_ValueSetStrFast(v, str);
}
static inline BOOL SetStrLenFast(Value *v, const char *str, SIZE len)
{
    return zzz_ValueSetStrLenFast(v, str, len);
}
static inline BOOL SetStr(Value *v, const char *str)
{
    return zzz_ValueSetStr(v, str);
}
static inline BOOL SetStrLen(Value *v, const char *str, SIZE len)
{
    return zzz_ValueSetStrLen(v, str, len);
}
static inline BOOL SetStrEscape(Value *v, const char *str)
{
    return zzz_ValueSetStrEscape(v, str);
}
static inline BOOL SetStrLenEscape(Value *v, const char *str, SIZE len)
{
    return zzz_ValueSetStrLenEscape(v, str, len);
}
static inline BOOL SetKeyFast(Value *v, const char *key)
{
    return zzz_ValueSetKeyFast(v, key);
}
static inline BOOL SetKeyLenFast(Value *v, const char *key, SIZE len)
{
    return zzz_ValueSetKeyLenFast(v, key, len);
}
static inline BOOL SetKey(Value *v, const char *key)
{
    return zzz_ValueSetKey(v, key);
}
static inline BOOL SetKeyLen(Value *v, const char *key, SIZE len)
{
    return zzz_ValueSetKeyLen(v, key, len);
}
static inline BOOL SetKeyEscape(Value *v, const char *key) {
    return zzz_ValueSetKeyEscape(v, key);
}
static inline BOOL SetKeyLenEscape(Value *v, const char *key, SIZE len) {
    return zzz_ValueSetKeyLenEscape(v, key, len);
}
static inline void SetArray(Value *v)
{
    zzz_ValueSetArray(v);
}
static inline void SetObj(Value *v)
{
    zzz_ValueSetObj(v);
}
static inline BOOL SetFast(Value *v, Value *vv)
{
    return zzz_ValueSetFast(v, vv);
}
static inline BOOL Set(Value *v, const Value *vv)
{
    return zzz_ValueSet(v, vv);
}
static inline BOOL ObjAddFast(Value *v, Value *vv)
{
    return zzz_ValueObjAddFast(v, vv);
}
static inline BOOL ObjAdd(Value *v, const Value *vv)
{
    return zzz_ValueObjAdd(v, vv);
}
static inline BOOL ArrayAddFast(Value *v, Value *vv)
{
    return zzz_ValueArrayAddFast(v, vv);
}
static inline BOOL ArrayAdd(Value *v, const Value *vv)
{
    return zzz_ValueArrayAdd(v, vv);
}
static inline BOOL ArrayDel(Value *v, SIZE index)
{
    return zzz_ValueArrayDel(v, index);
}
static inline BOOL ObjDel(Value *v, const char *key)
{
    return zzz_ValueObjDel(v, key);
}
#endif

// 内存拷贝函数
static inline void zzz_Copy(const char *src, zzz_SIZE len, char *des)
{
    memcpy(des, src, len);
}

// 字符串长度计算函数
static inline zzz_SIZE zzz_StrLen(const char *str)
{
    return (zzz_SIZE)strlen(str);
}

static inline double zzz_StrToDouble(const char *str)
{
    return atof(str);
}

static inline void zzz_DoubleToStr(double d, char *buff)
{
    sprintf(buff, "%.17g", d);
}

// 字符串比较，a必须以0结束，len为b的长度。
static inline zzz_BOOL zzz_StrIsEqual(const char *a, const char *b, zzz_SIZE len)
{
    zzz_SIZE i;
    for (i = 0; zzz_LIKELY(i < len); ++i)
    {
        if (zzz_LIKELY(a[i] != b[i]))
        {
            return zzz_False;
        }
    }
    // a字符串必须结束才能算相等
    if (zzz_LIKELY(a[i] == 0))
        return zzz_True;
    return zzz_False;
}

// 字符串比较，len为b的长度。
static inline zzz_BOOL zzz_StrIsEqualLen(const char *a, zzz_SIZE a_len, const char *b, zzz_SIZE b_len)
{
    if (zzz_LIKELY(a_len != b_len))
    {
        return zzz_False;
    }
    zzz_SIZE i;
    for (i = 0; zzz_LIKELY(i < a_len); ++i)
    {
        if (zzz_LIKELY(a[i] != b[i]))
        {
            return zzz_False;
        }
    }
    return zzz_True;
}

// 内存分配器节点
struct zzz_ANode
{
    // 数据地址
    char *Data;
    // 数据大小
    zzz_SIZE Size;
    // 使用到的位置
    zzz_SIZE Pos;
    // 下一个节点
    struct zzz_ANode *Next;
};

// 内存分配器
// 内存分配器为由内存分配器节点组成的链表，Root为根节点，End总是指向最后一个节点
struct zzz_Allocator
{
    // 根节点
    struct zzz_ANode *Root;
    // 最后一个节点
    struct zzz_ANode *End;
};

// 函数说明详见《API》
static inline struct zzz_Allocator *zzz_AllocatorNew()
{
    // 分配大块内存
    void *ptr = zzz_New(sizeof(struct zzz_Allocator) + sizeof(struct zzz_ANode) + zzz_AllocatorInitMemSize);
    struct zzz_Allocator *alloc = (struct zzz_Allocator *)ptr;
    alloc->Root = (struct zzz_ANode *)((char *)ptr + sizeof(struct zzz_Allocator));
    alloc->End = alloc->Root;

    alloc->Root->Size = zzz_AllocatorInitMemSize;
    alloc->Root->Data = (char *)ptr + sizeof(struct zzz_Allocator) + sizeof(struct zzz_ANode);
    alloc->Root->Pos = 0;
    alloc->Root->Next = 0;
    return alloc;
}

// 函数说明详见《API》
static inline void zzz_AllocatorRelease(struct zzz_Allocator *alloc)
{
    // 遍历整个链表，每次释放一块内存
    struct zzz_ANode *next = alloc->Root->Next;
    while (zzz_LIKELY(next != 0))
    {
        struct zzz_ANode *nn = next->Next;
        zzz_Free((void *)next);
        next = nn;
    }
    // 最后释放第一块内存
    zzz_Free((void *)alloc);
}

// 追加一个大小为 init_size 的节点。
static inline void zzz_AllocatorAppendChild(zzz_SIZE init_size, struct zzz_Allocator *alloc)
{
    // 每次分配一大块内存，避免多次分配
    void *ptr = zzz_New(sizeof(struct zzz_ANode) + init_size);
    struct zzz_ANode *node = (struct zzz_ANode *)ptr;
    node->Size = init_size;
    node->Data = (char *)ptr + sizeof(struct zzz_ANode);
    node->Pos = 0;
    node->Next = 0;
    // 在ANode组成的链表最后加一个ANode
    alloc->End->Next = node;
    alloc->End = node;
    return;
}

// 分配大小为size的内存
static inline char *zzz_AllocatorAlloc(struct zzz_Allocator *alloc, zzz_SIZE size)
{
    struct zzz_ANode *cur_node = alloc->End;
    zzz_SIZE s = cur_node->Size;
    if (zzz_UNLIKELY(cur_node->Pos + size > s))
    {
        s *= zzz_Delta;
        // 通过循环计算最终需要的空间大小
        // 这里应该有更好的方法，就是直接通过计算所得
        while (zzz_UNLIKELY(size > s))
            s *= zzz_Delta; // 每次分配内存的大小是上次的zzz_Delta倍
        zzz_AllocatorAppendChild(s, alloc);
        cur_node = alloc->End;
    }
    char *ret = cur_node->Data + cur_node->Pos;
    cur_node->Pos += size;
    return ret;
}

// 字符串
// 字符串由内存分配器分配内存，当追加操作导致内存不够时，直接分配 zzz_Delta 倍内存，并把旧内存拷贝到新内存中
struct zzz_String
{
    // 数据
    char *Data;
    // 位置
    zzz_SIZE Pos;
    // 使用内存大小
    zzz_SIZE Size;
    // 分配器
    struct zzz_Allocator *A;
};

static struct zzz_String *zzz_StringCache = 0;

// 新建一个字符串
static inline struct zzz_String *zzz_StringNew(struct zzz_Allocator *alloc, zzz_SIZE init_size)
{
    struct zzz_String *str = (struct zzz_String *)zzz_AllocatorAlloc(alloc, sizeof(struct zzz_String) + init_size);
    str->Size = init_size;
    str->Data = (char *)str + sizeof(struct zzz_String);
    str->Pos = 0;
    str->A = alloc;
    return str;
}

// 清空一个字符串
static inline void zzz_StringReset(struct zzz_String *str)
{
    str->Pos = 0;
}

// 追加字符串
static inline void zzz_StringAppendStr(struct zzz_String *str, const char *s, zzz_SIZE size)
{
    zzz_SIZE src_s = str->Size;
    if (zzz_UNLIKELY(str->Pos + size > src_s))
    {
        src_s *= zzz_Delta;
        while (zzz_UNLIKELY(str->Pos + size > src_s))
            src_s *= zzz_Delta;
        const char *src_d = str->Data;
        str->Data = (char *)zzz_AllocatorAlloc(str->A, src_s);
        str->Size = src_s;
        zzz_Copy(src_d, str->Pos, str->Data);
    }
    zzz_Copy(s, size, str->Data + str->Pos);
    str->Pos += size;
}

// 追加字符
static inline void zzz_StringAppendChar(struct zzz_String *str, const char c)
{
    zzz_SIZE src_s = str->Size;
    if (zzz_UNLIKELY(str->Pos + 1 > src_s))
    {
        src_s *= zzz_Delta;
        const char *src_d = str->Data;
        str->Data = (char *)zzz_AllocatorAlloc(str->A, src_s);
        str->Size = src_s;
        zzz_Copy(src_d, str->Pos, str->Data);
    }
    *(str->Data + str->Pos) = c;
    str->Pos += 1;
}

// 追加结束符
static inline void zzz_StringAppendEnd(struct zzz_String *str)
{
    zzz_StringAppendChar(str, 0);
}

// 获得字符串
static inline const char *zzz_StringStr(struct zzz_String *str)
{
    return str->Data;
}

// zzzJSON把文本转化成内存中的一棵树，zzz_Node为该数的节点，每个节点对应一个值
struct zzz_Node
{
    // 节点代表的值的类型
    char Type;

    // 节点代表的值的关键字
    const char *Key;
    // 节点代表的值的关键字长度
    zzz_SIZE KeyLen;

    union {
        // 如果节点代表的值的类型为数组或者对象，则表示数组或者对象的第一个值对应的节点
        struct zzz_Node *Node;
        // 如果节点代表的值的类型为字符串，数字，布尔值，则对应其字符串
        const char *Str;
    } Value;

    // 节点对应的值包含值的个数，如果类型非对象或者数组，则为0
    zzz_SIZE Len;

    // 下一个节点
    struct zzz_Node *Next;
    // 上一个节点
    struct zzz_Node *Prev;
    // 父节点
    struct zzz_Node *Father;
    // 最后一个节点
    struct zzz_Node *End;
};

// zzzJSON的基本单位：值，包含一个节点和一个内存分配器
struct zzz_Value
{
    struct zzz_Node *N;
    struct zzz_Allocator *A;
    const char *CacheStr;
    union {
        const char *UnEscapeStr;
        const double *Num;
    } Cache;
    const char *CacheKey;
    const char *CacheUnEscapeKey;
};

// 把Cache置零的函数
static inline void zzz_ValueInitCache(struct zzz_Value *v)
{
    v->CacheStr = 0;
    v->Cache.Num = 0;
    v->CacheKey = 0;
    v->CacheUnEscapeKey = 0;
}

// 函数说明详见《API》
static inline struct zzz_Value *zzz_ValueNew(struct zzz_Allocator *alloc)
{
    struct zzz_Value *v = (struct zzz_Value *)zzz_AllocatorAlloc(alloc, sizeof(struct zzz_Value));
    v->A = alloc;
    v->N = 0;
    zzz_ValueInitCache(v);
    return v;
}

// 创建一个值，并赋值
static inline struct zzz_Value *zzz_ValueInnerNew(struct zzz_Allocator *alloc, struct zzz_Node *n)
{
    struct zzz_Value *v = (struct zzz_Value *)zzz_AllocatorAlloc(alloc, sizeof(struct zzz_Value));
    v->A = alloc;
    v->N = n;
    zzz_ValueInitCache(v);
    return v;
}

// 跳过空格、tab、换行符
static inline zzz_BOOL zzz_Skin(const char c)
{
    if (zzz_UNLIKELY(
            zzz_UNLIKELY(c == ' ') ||
            zzz_UNLIKELY(c == '\t') ||
            zzz_UNLIKELY(c == '\n') ||
            zzz_UNLIKELY(c == '\r')))
    {
        return zzz_True;
    }
    return zzz_False;
}

// 下一个有效字符
static inline char zzz_Peek(const char *s, zzz_SIZE *index)
{
    while (zzz_UNLIKELY(zzz_Skin(s[*index])))
        ++(*index);
    return s[(*index)++];
}

// 消费一个字符
static inline zzz_BOOL zzz_Consume(const char c, const char *s, zzz_SIZE *index)
{
    if (s[*index] == c)
    {
        ++(*index);
        return zzz_True;
    }
    return zzz_False;
}

// 预期消费一个字符成功
static inline zzz_BOOL zzz_LikelyConsume(const char c, const char *s, zzz_SIZE *index)
{
    if (zzz_LIKELY(s[*index] == c))
    {
        ++(*index);
        return zzz_True;
    }
    return zzz_False;
}

// 预期消费一个字符失败
static inline zzz_BOOL zzz_UnLikelyConsume(const char c, const char *s, zzz_SIZE *index)
{
    if (zzz_UNLIKELY(s[*index] == c))
    {
        ++(*index);
        return zzz_True;
    }
    return zzz_False;
}

// 预期消费下一个有效字符成功
static inline zzz_BOOL zzz_LikelyPeekAndConsume(const char c, const char *s, zzz_SIZE *index)
{
    while (zzz_UNLIKELY(zzz_Skin(s[*index])))
        ++(*index);
    if (zzz_LIKELY(s[*index] == c))
    {
        ++(*index);
        return zzz_True;
    }
    return zzz_False;
}

// 预期消费下一个有效字符失败
static inline zzz_BOOL zzz_UnLikelyPeekAndConsume(const char c, const char *s, zzz_SIZE *index)
{
    while (zzz_UNLIKELY(zzz_Skin(s[*index])))
        ++(*index);
    if (zzz_UNLIKELY(s[*index] == c))
    {
        ++(*index);
        return zzz_True;
    }
    return zzz_False;
}

// 消费False
static inline zzz_BOOL zzz_ConsumeFalse(const char *s, zzz_SIZE *index)
{
    if (zzz_LIKELY(*((uint32_t *)("alse")) == *((uint32_t *)(s + *index))))
    {
        *index += 4;
        return zzz_True;
    }
    return zzz_False;
}

// 消费True
static inline zzz_BOOL zzz_ConsumeTrue(const char *s, zzz_SIZE *index)
{
    if (zzz_LIKELY(*((uint32_t *)zzz_StrTrue) == *((uint32_t *)(s + *index - 1))))
    {
        *index += 3;
        return zzz_True;
    }
    return zzz_False;
}

// 消费Null
static inline zzz_BOOL zzz_ConsumeNull(const char *s, zzz_SIZE *index)
{
    if (zzz_LIKELY(*((uint32_t *)zzz_StrNull) == *((uint32_t *)(s + *index - 1))))
    {
        *index += 3;
        return zzz_True;
    }
    return zzz_False;
}

// 十六进制对应的十进制数字
static inline zzz_SIZE zzz_HexCodePoint(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 16;
}

// 专为 zzz_ValueGetUnEscapeStr 使用
static inline zzz_SIZE zzz_HexCodePointForUnEscape(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return c - 'a' + 10;
}

// 消费一个十六进制字符
static inline zzz_BOOL zzz_ConsumeHexOne(const char *s, zzz_SIZE *index, zzz_SIZE *cp)
{
    zzz_SIZE tcp = zzz_HexCodePoint(s[*index]);
    if (zzz_LIKELY(tcp < 16))
    {
        *cp = *cp << 4;
        *cp += tcp;
        ++(*index);
        return zzz_True;
    }
    return zzz_False;
}

// 专为 zzz_ValueGetUnEscapeStr 使用
static inline void zzz_ConsumeHexOneForUnEscape(const char *s, zzz_SIZE *index, zzz_SIZE *cp)
{
    *cp = *cp << 4;
    *cp += zzz_HexCodePointForUnEscape(s[*index]);
    ++(*index);
    return;
}

// 消费4个十六进制字符
static inline zzz_BOOL zzz_ConsumeHex(const char *s, zzz_SIZE *index, zzz_SIZE *cp)
{
    if (zzz_LIKELY(zzz_LIKELY(zzz_ConsumeHexOne(s, index, cp)) &&
                   zzz_LIKELY(zzz_ConsumeHexOne(s, index, cp)) &&
                   zzz_LIKELY(zzz_ConsumeHexOne(s, index, cp)) &&
                   zzz_LIKELY(zzz_ConsumeHexOne(s, index, cp))))
    {
        return zzz_True;
    }
    return zzz_False;
}

// 专为 zzz_ValueGetUnEscapeStr 使用
static inline void zzz_ConsumeHexForUnEscape(const char *s, zzz_SIZE *index, zzz_SIZE *cp)
{
    zzz_ConsumeHexOneForUnEscape(s, index, cp);
    zzz_ConsumeHexOneForUnEscape(s, index, cp);
    zzz_ConsumeHexOneForUnEscape(s, index, cp);
    zzz_ConsumeHexOneForUnEscape(s, index, cp);
    return;
}

// 专为 zzz_ValueGetUnEscapeStr 使用，追加一个字符
static inline void zzz_Append(char *s, zzz_SIZE *index, char c)
{
    s[(*index)++] = c;
}

static inline void zzz_AppendLen(char *s, zzz_SIZE *index, const char *str, zzz_SIZE len)
{
    zzz_Copy(str, len, s + (*index));
    *index += len;
}

// 专为 zzz_ValueGetUnEscapeStr 使用，追加一个UTF8字符
static inline void zzz_AppendUTF8(char *s, zzz_SIZE *index, zzz_SIZE codepoint)
{

    // UTF8的规则，具体请参考 UNICODE 相关文档
    if (codepoint <= 0x7F)
    {
        zzz_Append(s, index, (char)(codepoint & 0xFF));
    }
    else if (codepoint <= 0x7FF)
    {
        zzz_Append(s, index, (char)(0xC0 | ((codepoint >> 6) & 0xFF)));
        zzz_Append(s, index, (char)(0x80 | ((codepoint & 0x3F))));
    }
    else if (codepoint <= 0xFFFF)
    {
        zzz_Append(s, index, (char)(0xE0 | ((codepoint >> 12) & 0xFF)));
        zzz_Append(s, index, (char)(0x80 | ((codepoint >> 6) & 0x3F)));
        zzz_Append(s, index, (char)(0x80 | (codepoint & 0x3F)));
    }
    else
    {
        zzz_Append(s, index, (char)(0xF0 | ((codepoint >> 18) & 0xFF)));
        zzz_Append(s, index, (char)(0x80 | ((codepoint >> 12) & 0x3F)));
        zzz_Append(s, index, (char)(0x80 | ((codepoint >> 6) & 0x3F)));
        zzz_Append(s, index, (char)(0x80 | (codepoint & 0x3F)));
    }
}

// 专为 zzz_ValueGetUnEscapeStr 使用，追加一个结束符号
static inline void zzz_AppendEnd(char *s, zzz_SIZE *index)
{
    zzz_Append(s, index, 0);
}

static inline void zzz_UnEscapeStr(const char *str, zzz_SIZE len, char *s)
{
    zzz_SIZE s_index = 0;
    zzz_SIZE index;
    char c;
    for (index = 0; index < len;)
    {
        c = str[index];
        // 如果是一个合法的JSON字符串，那么\后面一定有字符，因此，一定不会越界
        if (zzz_UNLIKELY(c == '\\'))
        {
            c = str[index + 1];
            switch (c)
            {
            case '"':
            {
                zzz_Append(s, &s_index, '\"');
                index += 2;
                break;
            }
            case '\\':
            {
                zzz_Append(s, &s_index, '\\');
                index += 2;
                break;
            }
            case 'b':
            {
                zzz_Append(s, &s_index, '\b');
                index += 2;
                break;
            }
            case 'f':
            {
                zzz_Append(s, &s_index, '\f');
                index += 2;
                break;
            }
            case 'n':
            {
                zzz_Append(s, &s_index, '\n');
                index += 2;
                break;
            }
            case 'r':
            {
                zzz_Append(s, &s_index, '\r');
                index += 2;
                break;
            }
            case 't':
            {
                zzz_Append(s, &s_index, '\t');
                index += 2;
                break;
            }
            case '/':
            {
                zzz_Append(s, &s_index, '/');
                index += 2;
                break;
            }
            case 'u':
            {
                index += 2;
                zzz_SIZE cp = 0;
                zzz_ConsumeHexForUnEscape(str, &index, &cp);

                if (zzz_UNLIKELY(cp >= 0xD800 && cp <= 0xDBFF))
                {
                    zzz_SIZE cp1 = 0;
                    index += 2;
                    zzz_ConsumeHexForUnEscape(str, &index, &cp1);
                    cp = (((cp - 0xD800) << 10) | (cp1 - 0xDC00)) + 0x10000;
                }
                zzz_AppendUTF8(s, &s_index, cp);
                break;
            }
            }
        }
        else
        {
            zzz_Append(s, &s_index, c);
            index += 1;
        }
    }
    zzz_AppendEnd(s, &s_index);
    return;
}

struct zzz_EscapeChar {
    const char *Str;
    zzz_SIZE Len;
};

static const struct zzz_EscapeChar zzz_escapeChars[256] = {
    {"\\u0000", 6},{"\\u0001", 6},{"\\u0002", 6},{"\\u0003", 6},{"\\u0004", 6},{"\\u0005", 6},{"\\u0006", 6},{"\\u0007", 6},

    {"\\b", 2},{"\\t", 2},{"\\n", 2},{"\\u000b", 6},{"\\f", 2},{"\\r", 2},{"\\u000e", 6},{"\\u000f", 6},

    {"\\u0010", 6},{"\\u0011", 6},{"\\u0012", 6},{"\\u0013", 6},{"\\u0014", 6},{"\\u0015", 6},{"\\u0016", 6},{"\\u0017", 6},
    {"\\u0018", 6},{"\\u0019", 6},{"\\u001a", 6},{"\\u001b", 6},{"\\u001c", 6},{"\\u001d", 6},{"\\u001e", 6},{"\\u001f", 6},

    {"\x20", 1},{"\x21", 1},{"\\\"", 2},{"\x23", 1},{"\x24", 1},{"\x25", 1},{"\x26", 1},{"\x27", 1},

    {"\x28", 1},{"\x29", 1},{"\x2a", 1},{"\x2b", 1},{"\x2c", 1},{"\x2d", 1},{"\x2e", 1},{"\x2f", 1},
    {"\x30", 1},{"\x31", 1},{"\x32", 1},{"\x33", 1},{"\x34", 1},{"\x35", 1},{"\x36", 1},{"\x37", 1},
    {"\x38", 1},{"\x39", 1},{"\x3a", 1},{"\x3b", 1},{"\x3c", 1},{"\x3d", 1},{"\x3e", 1},{"\x3f", 1},
    {"\x40", 1},{"\x41", 1},{"\x42", 1},{"\x43", 1},{"\x44", 1},{"\x45", 1},{"\x46", 1},{"\x47", 1},
    {"\x48", 1},{"\x49", 1},{"\x4a", 1},{"\x4b", 1},{"\x4c", 1},{"\x4d", 1},{"\x4e", 1},{"\x4f", 1},
    {"\x50", 1},{"\x51", 1},{"\x52", 1},{"\x53", 1},{"\x54", 1},{"\x55", 1},{"\x56", 1},{"\x57", 1},

    {"\x58", 1},{"\x59", 1},{"\x5a", 1},{"\x5b", 1},{"\\\\", 2},{"\x5d", 1},{"\x5e", 1},{"\x5f", 1},

    {"\x60", 1},{"\x61", 1},{"\x62", 1},{"\x63", 1},{"\x64", 1},{"\x65", 1},{"\x66", 1},{"\x67", 1}, 
    {"\x68", 1},{"\x69", 1},{"\x6a", 1},{"\x6b", 1},{"\x6c", 1},{"\x6d", 1},{"\x6e", 1},{"\x6f", 1}, 
    {"\x70", 1},{"\x71", 1},{"\x72", 1},{"\x73", 1},{"\x74", 1},{"\x75", 1},{"\x76", 1},{"\x77", 1}, 
    {"\x78", 1},{"\x79", 1},{"\x7a", 1},{"\x7b", 1},{"\x7c", 1},{"\x7d", 1},{"\x7e", 1},{"\x7f", 1}, 
    {"\x80", 1},{"\x81", 1},{"\x82", 1},{"\x83", 1},{"\x84", 1},{"\x85", 1},{"\x86", 1},{"\x87", 1}, 
    {"\x88", 1},{"\x89", 1},{"\x8a", 1},{"\x8b", 1},{"\x8c", 1},{"\x8d", 1},{"\x8e", 1},{"\x8f", 1}, 
    {"\x90", 1},{"\x91", 1},{"\x92", 1},{"\x93", 1},{"\x94", 1},{"\x95", 1},{"\x96", 1},{"\x97", 1}, 
    {"\x98", 1},{"\x99", 1},{"\x9a", 1},{"\x9b", 1},{"\x9c", 1},{"\x9d", 1},{"\x9e", 1},{"\x9f", 1}, 
    {"\xa0", 1},{"\xa1", 1},{"\xa2", 1},{"\xa3", 1},{"\xa4", 1},{"\xa5", 1},{"\xa6", 1},{"\xa7", 1}, 
    {"\xa8", 1},{"\xa9", 1},{"\xaa", 1},{"\xab", 1},{"\xac", 1},{"\xad", 1},{"\xae", 1},{"\xaf", 1}, 
    {"\xb0", 1},{"\xb1", 1},{"\xb2", 1},{"\xb3", 1},{"\xb4", 1},{"\xb5", 1},{"\xb6", 1},{"\xb7", 1}, 
    {"\xb8", 1},{"\xb9", 1},{"\xba", 1},{"\xbb", 1},{"\xbc", 1},{"\xbd", 1},{"\xbe", 1},{"\xbf", 1}, 
    {"\xc0", 1},{"\xc1", 1},{"\xc2", 1},{"\xc3", 1},{"\xc4", 1},{"\xc5", 1},{"\xc6", 1},{"\xc7", 1}, 
    {"\xc8", 1},{"\xc9", 1},{"\xca", 1},{"\xcb", 1},{"\xcc", 1},{"\xcd", 1},{"\xce", 1},{"\xcf", 1}, 
    {"\xd0", 1},{"\xd1", 1},{"\xd2", 1},{"\xd3", 1},{"\xd4", 1},{"\xd5", 1},{"\xd6", 1},{"\xd7", 1}, 
    {"\xd8", 1},{"\xd9", 1},{"\xda", 1},{"\xdb", 1},{"\xdc", 1},{"\xdd", 1},{"\xde", 1},{"\xdf", 1}, 
    {"\xe0", 1},{"\xe1", 1},{"\xe2", 1},{"\xe3", 1},{"\xe4", 1},{"\xe5", 1},{"\xe6", 1},{"\xe7", 1}, 
    {"\xe8", 1},{"\xe9", 1},{"\xea", 1},{"\xeb", 1},{"\xec", 1},{"\xed", 1},{"\xee", 1},{"\xef", 1}, 
    {"\xf0", 1},{"\xf1", 1},{"\xf2", 1},{"\xf3", 1},{"\xf4", 1},{"\xf5", 1},{"\xf6", 1},{"\xf7", 1}, 
    {"\xf8", 1},{"\xf9", 1},{"\xfa", 1},{"\xfb", 1},{"\xfc", 1},{"\xfd", 1},{"\xfe", 1},{"\xff", 1}
};


// str必定不为空，因此，一定会返回一个非空字符串
static inline const char *zzz_EscapeStr(const char *str, struct zzz_Allocator *a)
{
    zzz_SIZE len = 0;
    const char * src = str;
    while (zzz_LIKELY(*str != 0)) {
        len += zzz_escapeChars[(unsigned char)(*str)].Len;
        ++str;
    }
    char *s = zzz_AllocatorAlloc(a, len+1);
    zzz_SIZE index = 0;
    str = src;
    while (zzz_LIKELY(*str != 0)) {
        zzz_AppendLen(s, &index, zzz_escapeChars[(unsigned char)(*str)].Str, zzz_escapeChars[(unsigned char)(*str)].Len);
        ++str;
    }
    zzz_AppendEnd(s, &index);
    return s;
}

// str必定不为空，因此，一定会返回一个非空字符串
static inline const char *zzz_EscapeStrLen(const char *str, struct zzz_Allocator *a, zzz_SIZE len)
{
    zzz_SIZE l = 0;
    const char * src = str;
    zzz_SIZE srcLen = len;
    while (zzz_LIKELY(len != 0)) {
        l += zzz_escapeChars[(unsigned char)(*str)].Len;
        ++str;
        --len;
    }
    char *s = zzz_AllocatorAlloc(a, l+1);
    zzz_SIZE index = 0;
    str = src;
    len = srcLen;
    while (zzz_LIKELY(len != 0)) {
        zzz_AppendLen(s, &index, zzz_escapeChars[(unsigned char)(*str)].Str, zzz_escapeChars[(unsigned char)(*str)].Len);
        ++str;
        --len;
    }
    zzz_AppendEnd(s, &index);
    return s;
}

// 消费字符串
static inline zzz_BOOL zzz_ConsumeStr(const char *s, zzz_SIZE *index)
{
    char c;
    c = s[(*index)++];
    while (zzz_LIKELY(c != 0))
    {
        if (zzz_UNLIKELY((unsigned char)c <= 0x1f))
            return zzz_False;
        if (zzz_UNLIKELY(c == '\\'))
        {
            c = s[(*index)++];
            switch (c)
            {
            case '"':
            case '\\':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
            case '/':
                c = s[(*index)++];
                continue;
            case 'u':
            {
                zzz_SIZE cp = 0;
                if (zzz_LIKELY(zzz_ConsumeHex(s, index, &cp)))
                {
                    // 这里是UTF16标准，可以参考网上相关资料
                    // 搜索关键字 UNICODE
                    if (zzz_UNLIKELY(cp >= 0xDC00 && cp <= 0xDFFF))
                        return zzz_False;
                    if (zzz_UNLIKELY(cp >= 0xD800 && cp <= 0xDBFF))
                    {
                        if (zzz_LIKELY(zzz_LikelyConsume('\\', s, index) && zzz_LikelyConsume('u', s, index)))
                        {
                            zzz_SIZE cp2 = 0;
                            if (zzz_LIKELY(zzz_ConsumeHex(s, index, &cp2)))
                            {
                                if (zzz_UNLIKELY(cp2 < 0xDC00 || cp2 > 0xDFFF))
                                    return zzz_False;
                            }
                            else
                            {
                                return zzz_False;
                            }
                        }
                        else
                        {
                            return zzz_False;
                        }
                    }
                    c = s[(*index)++];
                }
                else
                {
                    return zzz_False;
                }
                continue;
            }
            default:
                return zzz_False;
            }
        }
        if (zzz_UNLIKELY(c == '"'))
        {
            return zzz_True;
        }
        c = s[(*index)++];
    }
    return zzz_False;
}

// 检查一个字符串是否符合JSON标准，主要用于 SetStr
static inline zzz_BOOL zzz_CheckStr(const char *s, zzz_SIZE *len)
{
    zzz_SIZE index = 0;
    char c;
    c = s[index++];
    while (zzz_LIKELY(c != 0))
    {
        if (zzz_UNLIKELY(zzz_UNLIKELY((unsigned char)c <= 0x1f) || zzz_UNLIKELY(c == '"')))
        {
            return zzz_False;
        }
        if (zzz_UNLIKELY(c == '\\'))
        {
            c = s[index++];
            switch (c)
            {
            case '"':
            case '\\':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
            case '/':
                c = s[index++];
                continue;
            case 'u':
            {
                zzz_SIZE cp = 0;
                if (zzz_LIKELY(zzz_ConsumeHex(s, &index, &cp)))
                {
                    // 这里是UTF16标准，可以参考网上相关资料
                    // 搜索关键字 UNICODE
                    if (zzz_UNLIKELY(cp >= 0xDC00 && cp <= 0xDFFFF))
                        return zzz_False;
                    if (zzz_UNLIKELY(cp >= 0xD800 && cp <= 0xDBFF))
                    {
                        if (zzz_LIKELY(zzz_LikelyConsume('\\', s, &index) && zzz_LikelyConsume('u', s, &index)))
                        {
                            zzz_SIZE cp2 = 0;
                            if (zzz_LIKELY(zzz_ConsumeHex(s, &index, &cp2)))
                            {
                                if (zzz_UNLIKELY(cp2 < 0xDC00 || cp2 > 0xDFFF))
                                    return zzz_False;
                            }
                            else
                            {
                                return zzz_False;
                            }
                        }
                        else
                        {
                            return zzz_False;
                        }
                    }
                    c = s[index++];
                }
                else
                {
                    return zzz_False;
                }
                continue;
            }
            default:
                return zzz_False;
            }
        }
        c = s[index++];
    }
    *len = index - 1;
    return zzz_True;
}
// 较为省事的一种实现方法，后面会实现更高效的算法
static inline zzz_BOOL zzz_CheckStrLen(struct zzz_Allocator *alloc, const char *s, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_StringCache == 0))
    {
        zzz_StringCache = zzz_StringNew(alloc, zzz_StringCacheInitMemSize);
    }
    else
    {
        zzz_StringReset(zzz_StringCache);
    }
    zzz_StringAppendStr(zzz_StringCache, s, len);
    zzz_StringAppendEnd(zzz_StringCache);
    zzz_SIZE avail_len;
    if (zzz_UNLIKELY(zzz_CheckStr(zzz_StringStr(zzz_StringCache), &avail_len) == zzz_False))
    {
        return zzz_False;
    }
    if (zzz_UNLIKELY(avail_len != len))
        return zzz_False;
    return zzz_True;
}

// 消费一个数字
static inline zzz_BOOL zzz_ConsumeNum(const char *s, zzz_SIZE *index)
{
    --(*index);

    if (s[*index] == '-')
        ++(*index);

    if (zzz_UnLikelyConsume('0', s, index))
    {
    }
    else if (zzz_LIKELY(zzz_LIKELY(s[*index] >= '1') && zzz_LIKELY(s[*index] <= '9')))
    {
        char c = s[++(*index)];
        while (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
        {
            c = s[++(*index)];
        }
    }
    else
    {
        return zzz_False;
    }

    if (zzz_Consume('.', s, index))
    {
        char c = s[*index];
        if ((zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
        {
            c = s[++(*index)];
            while (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
            {
                c = s[++(*index)];
            }
        }
        else
        {
            return zzz_False;
        }
    }

    if (s[*index] == 'e' || s[*index] == 'E')
    {
        char c = s[++(*index)];
        if (c == '-' || c == '+')
        {
            ++(*index);
        }
        c = s[*index];
        if (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
        {
            c = s[++(*index)];
            while (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
            {
                c = s[++(*index)];
            }
        }
        else
        {
            return zzz_False;
        }
    }

    return zzz_True;
}

// 检查一个数字是否符合JSON标准，主要用于 SetNum
static inline zzz_BOOL zzz_CheckNum(const char *s, zzz_SIZE *len)
{
    zzz_SIZE index = 0;

    if (s[index] == '-')
        ++(index);

    if (zzz_UnLikelyConsume('0', s, &index))
    {
    }
    else if (zzz_LIKELY(zzz_LIKELY(s[index] >= '1') && zzz_LIKELY(s[index] <= '9')))
    {
        char c = s[++index];
        while (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
        {
            c = s[++index];
        }
    }
    else
    {
        return zzz_False;
    }

    if (zzz_Consume('.', s, &index))
    {
        char c = s[index];
        if ((zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
        {
            c = s[++index];
            while (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
            {
                c = s[++index];
            }
        }
        else
        {
            return zzz_False;
        }
    }

    if (s[index] == 'e' || s[index] == 'E')
    {
        char c = s[++index];
        if (c == '-')
        {
            ++index;
        }
        else if (c == '+')
        {
            ++index;
        }
        c = s[index];
        if (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
        {
            c = s[++index];
            while (zzz_LIKELY(zzz_LIKELY(c >= '0') && zzz_LIKELY(c <= '9')))
            {
                c = s[++index];
            }
        }
        else
        {
            return zzz_False;
        }
    }
    *len = index;
    return zzz_LikelyConsume(0, s, &index);
}
static inline zzz_BOOL zzz_CheckNumLen(struct zzz_Allocator *alloc, const char *s, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_StringCache == 0))
    {
        zzz_StringCache = zzz_StringNew(alloc, zzz_StringCacheInitMemSize);
    }
    else
    {
        zzz_StringReset(zzz_StringCache);
    }
    zzz_StringAppendStr(zzz_StringCache, s, len);
    zzz_StringAppendEnd(zzz_StringCache);
    zzz_SIZE avail_len;
    if (zzz_UNLIKELY(zzz_CheckNum(zzz_StringStr(zzz_StringCache), &avail_len) == zzz_False))
    {
        return zzz_False;
    }
    if (zzz_UNLIKELY(avail_len != len))
        return zzz_False;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueParseFast(struct zzz_Value *v, const char *s)
{
    zzz_ValueInitCache(v);

    struct zzz_Node *src_node;
    if (zzz_LIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Prev = 0;
        v->N->Next = 0;
        v->N->Father = 0;
        v->N->Key = 0;
        src_node = 0;
    }
    else
    {
        src_node = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        *src_node = *v->N;
    }
    zzz_SIZE index = 0;
    struct zzz_Node *node = v->N;

    char c = zzz_Peek(s, &index);
    switch (c)
    {
    case '[':
    {
        node->Type = zzz_JSONTYPEARRAY;
        if (zzz_UnLikelyPeekAndConsume(']', s, &index))
        {
            node->Value.Node = 0;
            node->Len = 0;
            break;
        }
        struct zzz_Node *n = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        n->Father = node;
        n->Prev = 0;

        node->Value.Node = n;
        node->End = n;
        node->Len = 1;
        node = n;
        break;
    }
    case '{':
    {
        node->Type = zzz_JSONTYPEOBJECT;
        if (zzz_UnLikelyPeekAndConsume('}', s, &index))
        {
            node->Value.Node = 0;
            node->Len = 0;
            break;
        }
        struct zzz_Node *n = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        n->Father = node;
        n->Prev = 0;
        n->Next = 0;

        node->Value.Node = n;
        node->End = n;
        node->Len = 1;
        node = n;
        break;
    }
    case 'n':
        if (zzz_LIKELY(zzz_ConsumeNull(s, &index)))
        {
            node->Type = zzz_JSONTYPENULL;
            node->Value.Str = zzz_StrNull;
            node->Len = 4;
            break;
        }
        if (zzz_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return zzz_False;
    case 'f':
        if (zzz_LIKELY(zzz_ConsumeFalse(s, &index)))
        {
            node->Type = zzz_JSONTYPEBOOL;
            node->Value.Str = zzz_StrFalse;
            node->Len = 5;
            break;
        }
        if (zzz_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return zzz_False;
    case 't':
        if (zzz_LIKELY(zzz_ConsumeTrue(s, &index)))
        {
            node->Type = zzz_JSONTYPEBOOL;
            node->Value.Str = zzz_StrTrue;
            node->Len = 4;
            break;
        }
        if (zzz_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return zzz_False;
    case '"':
    {
        zzz_SIZE start = index;
        if (zzz_UNLIKELY(zzz_UnLikelyConsume('"', s, &index)))
        {
            node->Type = zzz_JSONTYPESTRING;
            node->Value.Str = s + index;
            node->Len = 0;
            break;
        }
        if (zzz_LIKELY(zzz_ConsumeStr(s, &index)))
        {
            node->Type = zzz_JSONTYPESTRING;
            node->Value.Str = s + start;
            node->Len = index - start - 1;
            break;
        }
        if (zzz_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return zzz_False;
    }
    default:
    {
        zzz_SIZE start = index - 1;
        if (zzz_LIKELY(zzz_ConsumeNum(s, &index)))
        {
            node->Type = zzz_JSONTYPENUMBER;
            node->Value.Str = s + start;
            node->Len = index - start;
            break;
        }
        if (zzz_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return zzz_False;
    }
    }
    while (zzz_LIKELY(node != v->N))
    {
        if (node->Father->Type == zzz_JSONTYPEOBJECT)
        {
            if (zzz_UNLIKELY(zzz_LikelyPeekAndConsume('"', s, &index) == zzz_False))
            {
                if (zzz_LIKELY(src_node == 0))
                    v->N = src_node;
                else
                    *v->N = *src_node;
                return zzz_False;
            }
            zzz_SIZE start = index;
            if (zzz_UNLIKELY(zzz_UnLikelyConsume('"', s, &index)))
            {
                node->Key = s + start;
                node->KeyLen = 0;
            }
            else
            {
                if (zzz_UNLIKELY(zzz_ConsumeStr(s, &index) == zzz_False))
                {
                    if (zzz_LIKELY(src_node == 0))
                        v->N = src_node;
                    else
                        *v->N = *src_node;
                    return zzz_False;
                }
                node->Key = s + start;
                node->KeyLen = index - start - 1;
            }
            if (zzz_UNLIKELY(zzz_LikelyPeekAndConsume(':', s, &index) == zzz_False))
            {
                if (zzz_LIKELY(src_node == 0))
                    v->N = src_node;
                else
                    *v->N = *src_node;
                return zzz_False;
            }
        }
        else
        {
            node->Key = 0;
        }
        c = zzz_Peek(s, &index);
        switch (c)
        {
        case '[':
        {
            node->Type = zzz_JSONTYPEARRAY;
            if (zzz_UnLikelyPeekAndConsume(']', s, &index))
            {
                node->Value.Node = 0;
                node->Len = 0;
                break;
            }
            struct zzz_Node *n = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
            n->Father = node;
            n->Prev = 0;

            node->Value.Node = n;
            node->End = n;
            node->Len = 1;
            node = n;
            continue;
        }
        case '{':
        {
            node->Type = zzz_JSONTYPEOBJECT;
            if (zzz_UnLikelyPeekAndConsume('}', s, &index))
            {
                node->Value.Node = 0;
                node->Len = 0;
                break;
            }
            struct zzz_Node *n = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
            n->Father = node;
            n->Prev = 0;
            n->Next = 0;

            node->Value.Node = n;
            node->End = n;
            node->Len = 1;
            node = n;
            continue;
        }
        case 'n':
            if (zzz_LIKELY(zzz_ConsumeNull(s, &index)))
            {
                node->Type = zzz_JSONTYPENULL;
                node->Value.Str = zzz_StrNull;
                node->Len = 4;
                break;
            }
            if (zzz_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return zzz_False;
        case 'f':
            if (zzz_LIKELY(zzz_ConsumeFalse(s, &index)))
            {
                node->Type = zzz_JSONTYPEBOOL;
                node->Value.Str = zzz_StrFalse;
                node->Len = 5;
                break;
            }
            if (zzz_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return zzz_False;
        case 't':
            if (zzz_LIKELY(zzz_ConsumeTrue(s, &index)))
            {
                node->Type = zzz_JSONTYPEBOOL;
                node->Value.Str = zzz_StrTrue;
                node->Len = 4;
                break;
            }
            if (zzz_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return zzz_False;
        case '"':
        {
            zzz_SIZE start = index;
            if (zzz_UNLIKELY(zzz_UnLikelyConsume('"', s, &index)))
            {
                node->Type = zzz_JSONTYPESTRING;
                node->Value.Str = s + index;
                node->Len = 0;
                break;
            }
            if (zzz_LIKELY(zzz_ConsumeStr(s, &index)))
            {
                node->Type = zzz_JSONTYPESTRING;
                node->Value.Str = s + start;
                node->Len = index - start - 1;
                break;
            }
            if (zzz_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return zzz_False;
        }
        default:
        {
            zzz_SIZE start = index - 1;
            if (zzz_LIKELY(zzz_ConsumeNum(s, &index)))
            {
                node->Type = zzz_JSONTYPENUMBER;
                node->Value.Str = s + start;
                node->Len = index - start;
                break;
            }
            if (zzz_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return zzz_False;
        }
        }
        while (zzz_LIKELY(node != v->N))
        {
            if (zzz_LikelyPeekAndConsume(',', s, &index))
            {
                struct zzz_Node *n = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
                n->Father = node->Father;
                n->Prev = node;
                node->Father->End = n;
                ++(node->Father->Len);
                node->Next = n;
                node = n;
                break;
            }
            else
            {
                char c = zzz_Peek(s, &index);
                if (zzz_LIKELY((c == '}' && zzz_LIKELY(node->Father->Type == zzz_JSONTYPEOBJECT)) ||
                               zzz_LIKELY(zzz_LIKELY(c == ']') && zzz_LIKELY(node->Father->Type == zzz_JSONTYPEARRAY))))
                {
                    node->Next = 0;
                    node = node->Father;
                }
                else
                {
                    if (zzz_LIKELY(src_node == 0))
                        v->N = src_node;
                    else
                        *v->N = *src_node;
                    return zzz_False;
                }
            }
        }
    }
    if (zzz_LIKELY(zzz_LikelyPeekAndConsume(0, s, &index)))
    {
        return zzz_True;
    }
    if (zzz_LIKELY(src_node == 0))
        v->N = src_node;
    else
        *v->N = *src_node;
    return zzz_False;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueParseLen(struct zzz_Value *v, const char *s, zzz_SIZE len)
{
    char *str = zzz_AllocatorAlloc(v->A, len + 1);
    zzz_Copy(s, len, str);
    str[len] = 0;
    return zzz_ValueParseFast(v, s);
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueParse(struct zzz_Value *v, const char *s)
{
    return zzz_ValueParseLen(v, s, zzz_StrLen(s));
}

// 函数说明详见《API》
static inline const char *zzz_ValueStringify(const struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return "";
    struct zzz_String *str = zzz_StringNew(v->A, zzz_StringInitMemSize);
    struct zzz_Node *node = v->N;

    switch (node->Type)
    {
    case zzz_JSONTYPEARRAY:
        zzz_StringAppendChar(str, '[');
        if (node->Value.Node != 0)
        {
            node = node->Value.Node;
        }
        else
        {
            zzz_StringAppendChar(str, ']');
        }
        break;
    case zzz_JSONTYPEOBJECT:
        zzz_StringAppendChar(str, '{');
        if (node->Len != 0)
        {
            node = node->Value.Node;
        }
        else
        {
            zzz_StringAppendChar(str, '}');
        }
        break;
    case zzz_JSONTYPESTRING:
        zzz_StringAppendChar(str, '"');
        zzz_StringAppendStr(str, node->Value.Str, node->Len);
        zzz_StringAppendChar(str, '"');
        break;
    default:
        zzz_StringAppendStr(str, node->Value.Str, node->Len);
        break;
    }

    while (zzz_LIKELY(node != v->N))
    {
        if (node->Key != 0)
        {
            zzz_StringAppendChar(str, '"');
            zzz_StringAppendStr(str, node->Key, node->KeyLen);
            zzz_StringAppendStr(str, "\":", 2);
        }
        switch (node->Type)
        {
        case zzz_JSONTYPEARRAY:
            zzz_StringAppendChar(str, '[');
            if (node->Value.Node != 0)
            {
                node = node->Value.Node;
                continue;
            }
            else
            {
                zzz_StringAppendChar(str, ']');
            }
            break;
        case zzz_JSONTYPEOBJECT:
            zzz_StringAppendChar(str, '{');
            if (node->Len != 0)
            {
                node = node->Value.Node;
                continue;
            }
            else
            {
                zzz_StringAppendChar(str, '}');
            }
            break;
        case zzz_JSONTYPESTRING:
            zzz_StringAppendChar(str, '"');
            zzz_StringAppendStr(str, node->Value.Str, node->Len);
            zzz_StringAppendChar(str, '"');
            break;
        default:
            zzz_StringAppendStr(str, node->Value.Str, node->Len);
            break;
        }
        while (zzz_LIKELY(node != v->N))
        {
            if (zzz_LIKELY(node->Next != 0))
            {
                zzz_StringAppendChar(str, ',');
                node = node->Next;
                break;
            }
            else
            {
                node = node->Father;
                if (node->Type == zzz_JSONTYPEARRAY)
                {
                    zzz_StringAppendChar(str, ']');
                }
                else
                {
                    zzz_StringAppendChar(str, '}');
                }
            }
        }
    }
    zzz_StringAppendEnd(str);
    return zzz_StringStr(str);
}

// 函数说明详见《API》
static inline const char *zzz_ValueGetStrFast(const struct zzz_Value *v, zzz_SIZE *len)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPESTRING))
        return 0;
    *len = v->N->Len;
    return v->N->Value.Str;
}

// 函数说明详见《API》
static inline const char *zzz_ValueGetStr(struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPESTRING))
        return 0;
    if (zzz_UNLIKELY(v->CacheStr != 0))
        return v->CacheStr;
    char *str = zzz_AllocatorAlloc(v->A, v->N->Len + 1);
    zzz_Copy(v->N->Value.Str, v->N->Len, str);
    str[v->N->Len] = 0;
    v->CacheStr = str;
    return str;
}

// 函数说明详见《API》
static inline const char *zzz_ValueGetUnEscapeStr(struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPESTRING))
        return 0;
    if (zzz_UNLIKELY(v->Cache.UnEscapeStr != 0))
        return v->Cache.UnEscapeStr;
    // 因为值里面的字符串一定是合法的字符串，因此，可以简化
    // 因为解码后的字符串一定比解码前短，所以申请原字符串+1的长度即可
    char *ret_str = zzz_AllocatorAlloc(v->A, v->N->Len + 1);
    zzz_UnEscapeStr(v->N->Value.Str, v->N->Len, ret_str);
    v->Cache.UnEscapeStr = ret_str;
    return ret_str;
}

// 函数说明详见《API》
static inline const char *zzz_ValueGetNumFast(const struct zzz_Value *v, zzz_SIZE *len)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPENUMBER))
        return 0;
    *len = v->N->Len;
    return v->N->Value.Str;
}

// 函数说明详见《API》
static inline const char *zzz_ValueGetNumStr(struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPENUMBER))
        return 0;
    if (zzz_UNLIKELY(v->CacheStr != 0))
        return v->CacheStr;
    char *str = zzz_AllocatorAlloc(v->A, v->N->Len + 1);
    zzz_Copy(v->N->Value.Str, v->N->Len, str);
    str[v->N->Len] = 0;
    v->CacheStr = str;
    return str;
}

// 函数说明详见《API》
static inline const double *zzz_ValueGetNum(struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPENUMBER))
        return 0;
    if (zzz_UNLIKELY(v->Cache.Num != 0))
        return v->Cache.Num;
    double *d = (double *)zzz_AllocatorAlloc(v->A, sizeof(double) / sizeof(char));
    *d = zzz_StrToDouble(v->N->Value.Str);
    v->Cache.Num = d;
    return d;
}

// 函数说明详见《API》
static inline const zzz_BOOL *zzz_ValueGetBool(const struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEBOOL))
        return 0;
    if (*(v->N->Value.Str) == 't')
        return &zzz_True;
    return &zzz_False;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueIsNull(const struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return zzz_False;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPENULL))
        return zzz_False;
    return zzz_True;
}

// 函数说明详见《API》
static inline const char *zzz_ValueGetKey(struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Key == 0))
        return 0;

    if (zzz_UNLIKELY(v->CacheKey != 0))
        return v->CacheKey;

    char *str = zzz_AllocatorAlloc(v->A, v->N->KeyLen + 1);
    zzz_Copy(v->N->Key, v->N->KeyLen, str);
    str[v->N->KeyLen] = 0;
    v->CacheKey = str;
    return str;
}

static inline const char *zzz_ValueGetUnEscapeKey(struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Key == 0))
        return 0;

    if (zzz_UNLIKELY(v->CacheUnEscapeKey != 0))
        return v->CacheUnEscapeKey;
    char *str = zzz_AllocatorAlloc(v->A, v->N->KeyLen + 1);
    zzz_UnEscapeStr(v->N->Key, v->N->KeyLen, str);
    v->CacheUnEscapeKey = str;
    return str;
}

// 函数说明详见《API》
static inline const char *zzz_ValueGetKeyFast(const struct zzz_Value *v, zzz_SIZE *len)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Key == 0))
        return 0;
    *len = v->N->KeyLen;
    return v->N->Key;
}

// 函数说明详见《API》
static inline struct zzz_Value *zzz_ValueObjGet(const struct zzz_Value *v, const char *key)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEOBJECT))
        return 0;
    struct zzz_Node *next = v->N->Value.Node;
    while (zzz_LIKELY(next != 0))
    {
        if (zzz_UNLIKELY(zzz_StrIsEqual(key, next->Key, next->KeyLen) == zzz_True))
        {
            struct zzz_Value *ret_val = zzz_ValueInnerNew(v->A, next);
            return ret_val;
        }
        next = next->Next;
    }
    return 0;
}

// 函数说明详见《API》
static inline struct zzz_Value *zzz_ValueObjGetLen(const struct zzz_Value *v, const char *key, zzz_SIZE len)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEOBJECT))
        return 0;
    struct zzz_Node *next = v->N->Value.Node;
    while (zzz_LIKELY(next != 0))
    {
        if (zzz_UNLIKELY(zzz_StrIsEqualLen(key, len, next->Key, next->KeyLen) == zzz_True))
        {
            struct zzz_Value *ret_val = zzz_ValueInnerNew(v->A, next);
            return ret_val;
        }
        next = next->Next;
    }
    return 0;
}

// 函数说明详见《API》
static inline const zzz_JSONType *zzz_ValueType(const struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    switch (v->N->Type)
    {
    case zzz_JSONTYPEARRAY:
        return &zzz_JSONTypeArray;
    case zzz_JSONTYPEOBJECT:
        return &zzz_JSONTypeObject;
    case zzz_JSONTYPESTRING:
        return &zzz_JSONTypeString;
    case zzz_JSONTYPENUMBER:
        return &zzz_JSONTypeNumber;
    case zzz_JSONTYPEBOOL:
        return &zzz_JSONTypeBool;
    case zzz_JSONTYPENULL:
        return &zzz_JSONTypeNull;
    }
    return 0;
}

// 函数说明详见《API》
static inline zzz_SIZE zzz_ValueSize(const struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEOBJECT && v->N->Type != zzz_JSONTYPEARRAY))
        return 0;
    return v->N->Len;
}

// 函数说明详见《API》
static inline struct zzz_Value *zzz_ValueArrayGet(const struct zzz_Value *v, zzz_SIZE index)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEARRAY))
        return 0;
    struct zzz_Node *next = v->N->Value.Node;
    zzz_SIZE i = 0;
    while (zzz_LIKELY(next != 0))
    {
        if (zzz_UNLIKELY(i == index))
        {
            struct zzz_Value *ret_val = zzz_ValueInnerNew(v->A, next);
            return ret_val;
        }
        next = next->Next;
        ++i;
    }
    return 0;
}

// 函数说明详见《API》
static inline struct zzz_Value *zzz_ValueBegin(const struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEOBJECT && v->N->Type != zzz_JSONTYPEARRAY))
        return 0;
    if (zzz_LIKELY(v->N->Value.Node != 0))
    {
        struct zzz_Value *ret_val = zzz_ValueInnerNew(v->A, v->N->Value.Node);
        return ret_val;
    }
    return 0;
}

// 函数说明详见《API》
static inline struct zzz_Value *zzz_ValueNext(const struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return 0;
    if (zzz_LIKELY(v->N->Next != 0))
    {
        struct zzz_Value *ret_val = zzz_ValueInnerNew(v->A, v->N->Next);
        return ret_val;
    }
    return 0;
}

// 把vv拷贝到v
static inline zzz_BOOL zzz_ValueCopyFrom(struct zzz_Value *v, const struct zzz_Value *vv)
{
    if (zzz_UNLIKELY(vv->N == 0))
        return zzz_False;
    struct zzz_Allocator *const A = v->A;
    v->N = (struct zzz_Node *)zzz_AllocatorAlloc(A, sizeof(struct zzz_Node));
    v->N->Prev = 0;
    v->N->Next = 0;
    v->N->Father = 0;

    struct zzz_Node *node = vv->N;
    struct zzz_Node *des_node = v->N;

    do
    {
        des_node->Type = node->Type;
        if (node->Key != 0)
        {
            char *k = zzz_AllocatorAlloc(A, node->KeyLen);
            zzz_Copy(node->Key, node->KeyLen, k);
            des_node->Key = k;
            des_node->KeyLen = node->KeyLen;
        }
        else
        {
            des_node->Key = 0;
        }
        switch (node->Type)
        {
        case zzz_JSONTYPEOBJECT:
        case zzz_JSONTYPEARRAY:
            des_node->Len = node->Len;
            if (zzz_LIKELY(node->Value.Node != 0))
            {
                node = node->Value.Node;
                struct zzz_Node *n = (struct zzz_Node *)zzz_AllocatorAlloc(A, sizeof(struct zzz_Node));
                n->Father = des_node;
                n->Prev = 0;
                des_node->Value.Node = n;
                des_node->End = n;
                des_node = n;
                continue;
            }
            des_node->Value.Node = 0;
            des_node->End = 0;
            break;
        case zzz_JSONTYPEBOOL:
        case zzz_JSONTYPENULL:
            des_node->Value.Str = node->Value.Str;
            des_node->Len = node->Len;
            break;
        case zzz_JSONTYPENUMBER:
        case zzz_JSONTYPESTRING:
        {
            char *s = zzz_AllocatorAlloc(A, node->Len);
            zzz_Copy(node->Value.Str, node->Len, s);
            des_node->Value.Str = s;
            des_node->Len = node->Len;
        }
        break;
        }
        while (zzz_LIKELY(node != vv->N))
        {
            if (zzz_LIKELY(node->Next != 0))
            {
                node = node->Next;
                struct zzz_Node *n = (struct zzz_Node *)zzz_AllocatorAlloc(A, sizeof(struct zzz_Node));
                n->Father = des_node->Father;
                n->Prev = des_node;
                n->Father->End = n;
                des_node->Next = n;
                des_node = n;
                break;
            }
            else
            {
                node = node->Father;
                des_node->Next = 0;
                des_node = des_node->Father;
            }
        }
    } while (zzz_LIKELY(node != vv->N));

    return zzz_True;
}

// 函数说明详见《API》
static inline struct zzz_Value *zzz_ValueCopy(const struct zzz_Value *v)
{
    struct zzz_Value *ret_val = zzz_ValueNew(v->A);
    if (zzz_UNLIKELY(zzz_ValueCopyFrom(ret_val, v) == zzz_False)) return 0;
    return ret_val;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueMove(struct zzz_Value *v)
{
    if (zzz_UNLIKELY(v->N == 0))
        return zzz_False;
    if (v->N->Father != 0)
    {
        if (v->N->Prev == 0)
        {
            v->N->Father->Value.Node = v->N->Next;
        }
        else
        {
            v->N->Prev->Next = v->N->Next;
            v->N->Prev = 0;
        }
        if (v->N->Next == 0)
        {
            v->N->Father->End = v->N->Prev;
        }
        else
        {
            v->N->Next->Prev = v->N->Prev;
            v->N->Next = 0;
        }
        --(v->N->Father->Len);
        v->N->Father = 0;
    }
    return zzz_True;
}

// 函数说明详见《API》
static inline void zzz_ValueSetNull(struct zzz_Value *v)
{
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPENULL;
    v->N->Value.Str = zzz_StrNull;
    v->N->Len = 4;
}

// 函数说明详见《API》
static inline void zzz_ValueSetBool(struct zzz_Value *v, zzz_BOOL b)
{
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPEBOOL;
    if (b == zzz_False)
    {
        v->N->Value.Str = zzz_StrFalse;
        v->N->Len = 5;
        return;
    }
    v->N->Value.Str = zzz_StrTrue;
    v->N->Len = 4;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetNumStrFast(struct zzz_Value *v, const char *num)
{
    zzz_SIZE len = 0;
    if (zzz_UNLIKELY(zzz_CheckNum(num, &len) == zzz_False))
        return zzz_False;
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPENUMBER;
    v->N->Value.Str = num;
    v->N->Len = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetNumStrLenFast(struct zzz_Value *v, const char *num, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_CheckNumLen(v->A, num, len) == zzz_False))
        return zzz_False;
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPENUMBER;
    v->N->Value.Str = num;
    v->N->Len = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetNumStr(struct zzz_Value *v, const char *num)
{
    zzz_SIZE len = 0;
    if (zzz_UNLIKELY(zzz_CheckNum(num, &len) == zzz_False))
        return zzz_False;
    zzz_ValueInitCache(v);
    char *s = zzz_AllocatorAlloc(v->A, len);
    zzz_Copy(num, len, s);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPENUMBER;
    v->N->Value.Str = s;
    v->N->Len = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetNumStrLen(struct zzz_Value *v, const char *num, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_CheckNumLen(v->A, num, len) == zzz_False))
        return zzz_False;
    zzz_ValueInitCache(v);
    char *s = zzz_AllocatorAlloc(v->A, len);
    zzz_Copy(num, len, s);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPENUMBER;
    v->N->Value.Str = s;
    v->N->Len = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetNum(struct zzz_Value *v, const double d)
{
    // double最大精度是17，使用sprintf转出字符串，%.17g，最大长度不超过32
    char *buff = zzz_AllocatorAlloc(v->A, 32);
    zzz_DoubleToStr(d, buff);
    return zzz_ValueSetNumStrFast(v, buff);
}

static inline zzz_BOOL zzz_ValueSetStrEscape(struct zzz_Value *v, const char *str)
{
    zzz_ValueInitCache(v);
    const char *es = zzz_EscapeStr(str, v->A);
    return zzz_ValueSetStrFast(v, es);
}

static inline zzz_BOOL zzz_ValueSetStrLenEscape(struct zzz_Value *v, const char *str, zzz_SIZE len)
{
    zzz_ValueInitCache(v);
    const char *es = zzz_EscapeStrLen(str, v->A, len);
    return zzz_ValueSetStrFast(v, es);
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetStrFast(struct zzz_Value *v, const char *str)
{
    zzz_SIZE len = 0;
    if (zzz_UNLIKELY(zzz_CheckStr(str, &len) == zzz_False))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPESTRING;
    v->N->Value.Str = str;
    v->N->Len = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetStrLenFast(struct zzz_Value *v, const char *str, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_CheckStrLen(v->A, str, len) == zzz_False))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPESTRING;
    v->N->Value.Str = str;
    v->N->Len = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetStr(struct zzz_Value *v, const char *str)
{
    zzz_SIZE len = 0;
    if (zzz_UNLIKELY(zzz_CheckStr(str, &len) == zzz_False))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    char *s = zzz_AllocatorAlloc(v->A, len);
    zzz_Copy(str, len, s);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPESTRING;
    v->N->Value.Str = s;
    v->N->Len = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetStrLen(struct zzz_Value *v, const char *str, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_CheckStrLen(v->A, str, len) == zzz_False))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    char *s = zzz_AllocatorAlloc(v->A, len);
    zzz_Copy(str, len, s);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPESTRING;
    v->N->Value.Str = s;
    v->N->Len = len;
    return zzz_True;
}

static inline zzz_BOOL zzz_ValueSetKeyEscape(struct zzz_Value *v, const char *key)
{
    zzz_ValueInitCache(v);
    const char *es = zzz_EscapeStr(key, v->A);
    return zzz_ValueSetKeyFast(v, es);
}

static inline zzz_BOOL zzz_ValueSetKeyLenEscape(struct zzz_Value *v, const char *key, zzz_SIZE len)
{
    zzz_ValueInitCache(v);
    const char *es = zzz_EscapeStrLen(key, v->A, len);
    return zzz_ValueSetKeyFast(v, es);
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetKeyFast(struct zzz_Value *v, const char *key)
{
    zzz_SIZE len = 0;
    if (zzz_UNLIKELY(zzz_CheckStr(key, &len) == zzz_False))
    {
        return zzz_False;
    }
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = zzz_JSONTYPENULL;
        v->N->Value.Str = zzz_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && zzz_UNLIKELY(v->N->Father->Type != zzz_JSONTYPEOBJECT))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    v->N->Key = key;
    v->N->KeyLen = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetKeyLenFast(struct zzz_Value *v, const char *key, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_CheckStrLen(v->A, key, len) == zzz_False))
    {
        return zzz_False;
    }
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = zzz_JSONTYPENULL;
        v->N->Value.Str = zzz_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && zzz_UNLIKELY(v->N->Father->Type != zzz_JSONTYPEOBJECT))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    v->N->Key = key;
    v->N->KeyLen = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetKey(struct zzz_Value *v, const char *key)
{
    zzz_SIZE len = 0;
    if (zzz_UNLIKELY(zzz_CheckStr(key, &len) == zzz_False))
    {
        return zzz_False;
    }
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = zzz_JSONTYPENULL;
        v->N->Value.Str = zzz_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && zzz_UNLIKELY(v->N->Father->Type != zzz_JSONTYPEOBJECT))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    char *s = zzz_AllocatorAlloc(v->A, len);
    zzz_Copy(key, len, s);
    v->N->Key = s;
    v->N->KeyLen = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetKeyLen(struct zzz_Value *v, const char *key, zzz_SIZE len)
{
    if (zzz_UNLIKELY(zzz_CheckStrLen(v->A, key, len) == zzz_False))
    {
        return zzz_False;
    }
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = zzz_JSONTYPENULL;
        v->N->Value.Str = zzz_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && zzz_UNLIKELY(v->N->Father->Type != zzz_JSONTYPEOBJECT))
    {
        return zzz_False;
    }
    zzz_ValueInitCache(v);
    char *s = zzz_AllocatorAlloc(v->A, len);
    zzz_Copy(key, len, s);
    v->N->Key = s;
    v->N->KeyLen = len;
    return zzz_True;
}

// 函数说明详见《API》
static inline void zzz_ValueSetArray(struct zzz_Value *v)
{
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPEARRAY;
    v->N->Value.Node = 0;
    v->N->Len = 0;
    return;
}

// 函数说明详见《API》
static inline void zzz_ValueSetObj(struct zzz_Value *v)
{
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = (struct zzz_Node *)zzz_AllocatorAlloc(v->A, sizeof(struct zzz_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = zzz_JSONTYPEOBJECT;
    v->N->Value.Node = 0;
    v->N->Len = 0;
    return;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSetFast(struct zzz_Value *v, struct zzz_Value *vv)
{
    if (zzz_UNLIKELY(zzz_ValueMove(vv) == zzz_False))
        return zzz_False;
    zzz_ValueInitCache(v);
    zzz_ValueInitCache(vv);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = vv->N;
        vv->N = 0;
        return zzz_True;
    }
    v->N->Type = vv->N->Type;
    if (v->N->Key != 0 && vv->N->Key != 0)
    {
        v->N->Key = vv->N->Key;
        v->N->KeyLen = vv->N->KeyLen;
    }
    v->N->Value = vv->N->Value;
    v->N->Len = vv->N->Len;
    if (v->N->Type == zzz_JSONTYPEARRAY || v->N->Type == zzz_JSONTYPEOBJECT)
    {
        v->N->End = vv->N->End;
        struct zzz_Node *next = v->N->Value.Node;
        while (zzz_LIKELY(next != 0))
        {
            next->Father = v->N;
            next = next->Next;
        }
    }
    vv->N = 0;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueSet(struct zzz_Value *v, const struct zzz_Value *vv)
{
    struct zzz_Value *cp = zzz_ValueCopy(vv);
    if (zzz_UNLIKELY(cp == 0))
        return zzz_False;
    zzz_ValueInitCache(v);
    if (zzz_UNLIKELY(v->N == 0))
    {
        v->N = cp->N;
        return zzz_True;
    }
    v->N->Type = cp->N->Type;
    if (v->N->Key != 0 && vv->N->Key != 0)
    {
        v->N->Key = cp->N->Key;
        v->N->KeyLen = cp->N->KeyLen;
    }
    v->N->Value = cp->N->Value;
    v->N->Len = cp->N->Len;
    if (v->N->Type == zzz_JSONTYPEARRAY || v->N->Type == zzz_JSONTYPEOBJECT)
    {
        v->N->End = vv->N->End;
        struct zzz_Node *next = v->N->Value.Node;
        while (zzz_LIKELY(next != 0))
        {
            next->Father = v->N;
            next = next->Next;
        }
    }
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueObjAddFast(struct zzz_Value *v, struct zzz_Value *vv)
{
    if (zzz_UNLIKELY(v->N == 0))
        return zzz_False;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEOBJECT))
        return zzz_False;
    if (zzz_UNLIKELY(vv->N == 0))
        return zzz_False;
    if (zzz_UNLIKELY(vv->N->Key == 0))
        return zzz_False;
    if (zzz_UNLIKELY(zzz_ValueMove(vv) == zzz_False))
        return zzz_False;
    zzz_ValueInitCache(vv);
    vv->N->Father = v->N;
    if (zzz_UNLIKELY(v->N->Value.Node == 0))
    {
        v->N->Value.Node = vv->N;
        v->N->Len = 1;
        v->N->End = vv->N;
    }
    else
    {
        v->N->End->Next = vv->N;
        vv->N->Prev = v->N->End;
        v->N->End = vv->N;
        ++v->N->Len;
    }
    vv->N = 0;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueObjAdd(struct zzz_Value *v, const struct zzz_Value *vv)
{
    if (zzz_UNLIKELY(v->N == 0))
        return zzz_False;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEOBJECT))
        return zzz_False;
    if (zzz_UNLIKELY(vv->N == 0))
        return zzz_False;
    if (zzz_UNLIKELY(vv->N->Key == 0))
        return zzz_False;
    struct zzz_Value *cp = zzz_ValueCopy(vv);
    if (zzz_UNLIKELY(cp == 0))
        return zzz_False;
    cp->N->Father = v->N;
    if (zzz_UNLIKELY(v->N->Value.Node == 0))
    {
        v->N->Value.Node = cp->N;
        v->N->Len = 1;
        v->N->End = cp->N;
    }
    else
    {
        v->N->End->Next = cp->N;
        cp->N->Prev = v->N->End;
        v->N->End = cp->N;
        ++v->N->Len;
    }
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueArrayAddFast(struct zzz_Value *v, struct zzz_Value *vv)
{
    if (zzz_UNLIKELY(v->N == 0))
        return zzz_False;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEARRAY))
        return zzz_False;
    if (zzz_UNLIKELY(zzz_ValueMove(vv) == zzz_False))
        return zzz_False;
    zzz_ValueInitCache(vv);
    vv->N->Key = 0;
    vv->N->Father = v->N;
    if (zzz_UNLIKELY(v->N->Value.Node == 0))
    {
        v->N->Value.Node = vv->N;
        v->N->Len = 1;
        v->N->End = vv->N;
    }
    else
    {
        v->N->End->Next = vv->N;
        vv->N->Prev = v->N->End;
        v->N->End = vv->N;
        ++v->N->Len;
    }
    vv->N = 0;
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueArrayAdd(struct zzz_Value *v, const struct zzz_Value *vv)
{
    if (zzz_UNLIKELY(v->N == 0))
        return zzz_False;
    if (zzz_UNLIKELY(v->N->Type != zzz_JSONTYPEARRAY))
        return zzz_False;
    struct zzz_Value *cp = zzz_ValueCopy(vv);
    if (zzz_UNLIKELY(cp == 0))
        return zzz_False;
    cp->N->Key = 0;
    cp->N->Father = v->N;
    if (zzz_UNLIKELY(v->N->Value.Node == 0))
    {
        v->N->Value.Node = cp->N;
        v->N->Len = 1;
        v->N->End = cp->N;
    }
    else
    {
        v->N->End->Next = cp->N;
        cp->N->Prev = v->N->End;
        v->N->End = cp->N;
        ++v->N->Len;
    }
    return zzz_True;
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueArrayDel(struct zzz_Value *v, zzz_SIZE index)
{
    struct zzz_Value *dv = zzz_ValueArrayGet(v, index);
    if (zzz_UNLIKELY(dv == 0))
        return zzz_False;
    return zzz_ValueMove(dv);
}

// 函数说明详见《API》
static inline zzz_BOOL zzz_ValueObjDel(struct zzz_Value *v, const char *key)
{
    struct zzz_Value *dv = zzz_ValueObjGet(v, key);
    if (zzz_UNLIKELY(dv == 0))
        return zzz_False;
    return zzz_ValueMove(dv);
}
#endif
