#ifndef z_JSON_H
#define z_JSON_H

// 长命名的 类型 & 常量 & 用作常量的宏，详见《数据结构》
typedef unsigned int z_SIZE;

typedef char z_BOOL;
static const z_BOOL z_True = 1;
static const z_BOOL z_False = 0;

typedef char z_JSONType;
#define z_JSONTYPEBOOL 1
#define z_JSONTYPEARRAY 2
#define z_JSONTYPEOBJ 3
#define z_JSONTYPESTRING 4
#define z_JSONTYPENULL 5
#define z_JSONTYPENUM 6
static const z_JSONType z_JSONTypeArray = z_JSONTYPEARRAY;
static const z_JSONType z_JSONTypeObj = z_JSONTYPEOBJ;
static const z_JSONType z_JSONTypeString = z_JSONTYPESTRING;
static const z_JSONType z_JSONTypeNum = z_JSONTYPENUM;
static const z_JSONType z_JSONTypeBool = z_JSONTYPEBOOL;
static const z_JSONType z_JSONTypeNull = z_JSONTYPENULL;

// 长命名的固字符串，详见《数据结构》
static const char *z_StrTrue = "true";
static const char *z_StrFalse = "false";
static const char *z_StrNull = "null";

// 内存配置，详见《内存配置》
#ifndef z_DELTA
#define z_DELTA 2
#endif
static const z_SIZE z_Delta = z_DELTA;

#ifndef z_ALLOCATORINITMEMSIZE
#define z_ALLOCATORINITMEMSIZE 1024 * 4
#endif
static const z_SIZE z_AllocatorInitMemSize = z_ALLOCATORINITMEMSIZE;

#ifndef z_STRINGINITMEMSIZE
#define z_STRINGINITMEMSIZE 1024
#endif
static const z_SIZE z_StringInitMemSize = z_STRINGINITMEMSIZE;

#ifndef z_STRINGCACHEINITMEMSIZE
#define z_STRINGCACHEINITMEMSIZE 128
#endif
static const z_SIZE z_StringCacheInitMemSize = z_STRINGCACHEINITMEMSIZE;

// 环境适配

#include <malloc.h> // 使用其 malloc 和 free 函数
#include <string.h> // 使用其 memcpy 函数
#include <stdlib.h> // 使用其 atof 函数
#include <stdio.h>  // 使用其 sprintf 函数

#ifndef z_MEMORY_MODE
#define z_MEMORY_MODE 1
#endif

#if z_MEMORY_MODE == 1

static inline void *z_New(z_SIZE size)
{
    return malloc(size);
}
static inline void z_Free(void *pointer) { free(pointer); }

#elif z_MEMORY_MODE == 2
// 测试模式，主要是用来测试内存的分配数和释放数，防止内存泄漏，同时用于观察内存分配次数。
// 通过观察，可以得出较好z_DELTA和z_ALLOCATORINITMEMSIZE。

static z_SIZE AllocMemorySize = 0, AllocMemoryCount = 0, FreeMemoryCount = 0;
static inline void *z_New(z_SIZE size) { return AllocMemorySize += size, AllocMemoryCount += 1, malloc(size); }
static inline void z_Free(void *ptr) { FreeMemoryCount += 1, free(ptr); }

#elif z_MEMORY_MODE == 3
// 自定义模式，需要实现z_New，z_Free和z_Copy三个函数
#endif

// 分支预测
#ifndef z_EXPECT_MODE
#if defined(__GNUC__) || defined(__clang__)
#define z_EXPECT_MODE 1
#else
#define z_EXPECT_MODE 2
#endif
#endif

#if z_EXPECT_MODE == 1
// gcc和clang，使用__builtin_expect
#define z_LIKELY(x) __builtin_expect(x, 1)
#define z_UNLIKELY(x) __builtin_expect(x, 0)
#elif z_EXPECT_MODE == 2
// msvc 不需要分支优化
#define z_LIKELY(x) x
#define z_UNLIKELY(x) x
#elif z_EXPECT_MODE == 3
// 自定义分支预测
#endif

// 长命名数据结构，详见《数据结构》
struct z_Allocator;
struct z_Value;

// 长命名数据结构，详见《API》
static inline struct z_Allocator *z_AllocatorNew();
static inline void z_AllocatorRelease(struct z_Allocator *root_alloc);
static inline struct z_Value *z_ValueNew(struct z_Allocator *alloc);

static inline z_BOOL z_ValueParseFast(struct z_Value *v, const char *s);
static inline z_BOOL z_ValueParseLen(struct z_Value *v, const char *s, z_SIZE len);
static inline z_BOOL z_ValueParse(struct z_Value *v, const char *s);

static inline const char *z_ValueStringify(const struct z_Value *v);

static inline const char *z_ValueGetStringFast(const struct z_Value *v, z_SIZE *len);
static inline const char *z_ValueGetUnEscapeString(struct z_Value *v);
static inline const char *z_ValueGetString(struct z_Value *v);

static inline const char *z_ValueGetNumFast(const struct z_Value *v, z_SIZE *len);
static inline const char *z_ValueGetNumStr(struct z_Value *v);
static inline const double *z_ValueGetNum(struct z_Value *v);

static inline const z_BOOL *z_ValueGetBool(const struct z_Value *v);

static inline z_BOOL z_ValueIsNull(const struct z_Value *v);

static inline const char *z_ValueGetKey(struct z_Value *v);
static inline const char *z_ValueGetUnEscapeKey(struct z_Value *v);
static inline const char *z_ValueGetKeyFast(const struct z_Value *v, z_SIZE *len);

static inline struct z_Value *z_ValueObjGet(const struct z_Value *v, const char *key);
static inline struct z_Value *z_ValueObjGetLen(const struct z_Value *v, const char *key, z_SIZE len);

static inline const z_JSONType *z_ValueType(const struct z_Value *v);

static inline z_SIZE z_ValueSize(const struct z_Value *v);

static inline struct z_Value *z_ValueArrayGet(const struct z_Value *v, z_SIZE index);

static inline struct z_Value *z_ValueBegin(const struct z_Value *v);
static inline struct z_Value *z_ValueNext(const struct z_Value *v);

static inline struct z_Value *z_ValueCopy(const struct z_Value *v);
static inline z_BOOL z_ValueMove(struct z_Value *v);

static inline void z_ValueSetNull(struct z_Value *v);

static inline void z_ValueSetBool(struct z_Value *v, z_BOOL b);

static inline z_BOOL z_ValueSetNumStrFast(struct z_Value *v, const char *num);
static inline z_BOOL z_ValueSetNumStrLenFast(struct z_Value *v, const char *num, z_SIZE len);
static inline z_BOOL z_ValueSetNumStr(struct z_Value *v, const char *num);
static inline z_BOOL z_ValueSetNumStrLen(struct z_Value *v, const char *num, z_SIZE len);
static inline z_BOOL z_ValueSetNum(struct z_Value *v, const double d);

static inline z_BOOL z_ValueSetStrFast(struct z_Value *v, const char *str);
static inline z_BOOL z_ValueSetStrLenFast(struct z_Value *v, const char *str, z_SIZE len);
static inline z_BOOL z_ValueSetStr(struct z_Value *v, const char *str);
static inline z_BOOL z_ValueSetStrLen(struct z_Value *v, const char *str, z_SIZE len);

static inline z_BOOL z_ValueSetKeyFast(struct z_Value *v, const char *key);
static inline z_BOOL z_ValueSetKeyLenFast(struct z_Value *v, const char *key, z_SIZE len);
static inline z_BOOL z_ValueSetKey(struct z_Value *v, const char *key);
static inline z_BOOL z_ValueSetKeyLen(struct z_Value *v, const char *key, z_SIZE len);

static inline void z_ValueSetArray(struct z_Value *v);

static inline void z_ValueSetObj(struct z_Value *v);

static inline z_BOOL z_ValueSetFast(struct z_Value *v, struct z_Value *vv);
static inline z_BOOL z_ValueSet(struct z_Value *v, const struct z_Value *vv);

static inline z_BOOL z_ValueObjAddFast(struct z_Value *v, struct z_Value *vv);
static inline z_BOOL z_ValueObjAdd(struct z_Value *v, const struct z_Value *vv);

static inline z_BOOL z_ValueArrayAddFast(struct z_Value *v, struct z_Value *vv);
static inline z_BOOL z_ValueArrayAdd(struct z_Value *v, const struct z_Value *vv);

static inline z_BOOL z_ValueArrayDel(struct z_Value *v, z_SIZE index);

static inline z_BOOL z_ValueObjDel(struct z_Value *v, const char *key);

// 短命名开关，默认开
#ifndef z_SHORT_API
#define z_SHORT_API 1
#endif
#if z_SHORT_API == 1

// 短命名数据结构，详见《数据结构》
typedef struct z_Allocator Allocator;
typedef struct z_Value Value;

typedef z_SIZE SIZE;

typedef z_BOOL BOOL;
static const BOOL True = 1;
static const BOOL False = 0;

typedef z_JSONType JSONType;
#define JSONTYPEBOOL z_JSONTYPEBOOL
#define JSONTYPEARRAY z_JSONTYPEARRAY
#define JSONTYPEOBJ z_JSONTYPEOBJ
#define JSONTYPESTRING z_JSONTYPESTRING
#define JSONTYPENULL z_JSONTYPENULL
#define JSONTYPENUM z_JSONTYPENUM
static const JSONType JSONTypeArray = z_JSONTypeArray;
static const JSONType JSONTypeObj = z_JSONTypeObj;
static const JSONType JSONTypeString = z_JSONTypeString;
static const JSONType JSONTypeNum = z_JSONTypeNum;
static const JSONType JSONTypeBool = z_JSONTypeBool;
static const JSONType JSONTypeNull = z_JSONTypeNull;

// 短命名API，详见《API》
static inline Allocator *NewAllocator()
{
    return z_AllocatorNew();
}
static inline void ReleaseAllocator(Allocator *root_alloc)
{
    z_AllocatorRelease(root_alloc);
}
static inline Value *NewValue(Allocator *alloc)
{
    return z_ValueNew(alloc);
}
static inline BOOL ParseFast(Value *v, const char *s)
{
    return z_ValueParseFast(v, s);
}
static inline BOOL ParseLen(Value *v, const char *s, SIZE len)
{
    return z_ValueParseLen(v, s, len);
}
static inline BOOL Parse(Value *v, const char *s)
{
    return z_ValueParse(v, s);
}
static inline const char *Stringify(const Value *v)
{
    return z_ValueStringify(v);
}
static inline const char *GetStringFast(const Value *v, SIZE *len)
{
    return z_ValueGetStringFast(v, len);
}
static inline const char *GetUnEscapeString(Value *v)
{
    return z_ValueGetUnEscapeString(v);
}
static inline const char *GetString(Value *v)
{
    return z_ValueGetString(v);
}
static inline const char *GetNumFast(const Value *v, z_SIZE *len)
{
    return z_ValueGetNumFast(v, len);
}
static inline const char *GetNumStr(Value *v)
{
    return z_ValueGetNumStr(v);
}
static inline const double *GetNum(Value *v)
{
    return z_ValueGetNum(v);
}
static inline const BOOL *GetBool(const Value *v)
{
    return z_ValueGetBool(v);
}
static inline BOOL IsNull(const Value *v)
{
    return z_ValueIsNull(v);
}
static inline const char *GetKey(Value *v)
{
    return z_ValueGetKey(v);
}
static inline const char *GetUnEscapeKey(Value *v)
{
    return z_ValueGetUnEscapeKey(v);
}
static inline const char *GetKeyFast(const Value *v, SIZE *len)
{
    return z_ValueGetKeyFast(v, len);
}
static inline Value *ObjGet(const Value *v, const char *key)
{
    return z_ValueObjGet(v, key);
}
static inline Value *ObjGetLen(const Value *v, const char *key, SIZE len)
{
    return z_ValueObjGetLen(v, key, len);
}
static inline const JSONType *Type(const Value *v)
{
    return z_ValueType(v);
}
static inline SIZE Size(const Value *v)
{
    return z_ValueSize(v);
}
static inline Value *ArrayGet(const Value *v, SIZE index)
{
    return z_ValueArrayGet(v, index);
}
static inline Value *Begin(const Value *v)
{
    return z_ValueBegin(v);
}
static inline Value *Next(const Value *v)
{
    return z_ValueNext(v);
}
static inline Value *Copy(const Value *v)
{
    return z_ValueCopy(v);
}
static inline BOOL Move(Value *v)
{
    return z_ValueMove(v);
}
static inline void SetNull(Value *v)
{
    z_ValueSetNull(v);
}
static inline void SetBool(Value *v, BOOL b)
{
    z_ValueSetBool(v, b);
}
static inline BOOL SetNumStrFast(Value *v, const char *num)
{
    return z_ValueSetNumStrFast(v, num);
}
static inline BOOL SetNumStrLenFast(Value *v, const char *num, SIZE len)
{
    return z_ValueSetNumStrLenFast(v, num, len);
}
static inline BOOL SetNumStr(Value *v, const char *num)
{
    return z_ValueSetNumStr(v, num);
}
static inline BOOL SetNumStrLen(Value *v, const char *num, SIZE len)
{
    return z_ValueSetNumStrLen(v, num, len);
}
static inline BOOL SetNum(Value *v, const double d)
{
    return z_ValueSetNum(v, d);
}
static inline BOOL SetStrFast(Value *v, const char *str)
{
    return z_ValueSetStrFast(v, str);
}
static inline BOOL SetStrLenFast(Value *v, const char *str, SIZE len)
{
    return z_ValueSetStrLenFast(v, str, len);
}
static inline BOOL SetStr(Value *v, const char *str)
{
    return z_ValueSetStr(v, str);
}
static inline BOOL SetStrLen(Value *v, const char *str, SIZE len)
{
    return z_ValueSetStrLen(v, str, len);
}
static inline BOOL SetKeyFast(Value *v, const char *key)
{
    return z_ValueSetKeyFast(v, key);
}
static inline BOOL SetKeyLenFast(Value *v, const char *key, SIZE len)
{
    return z_ValueSetKeyLenFast(v, key, len);
}
static inline BOOL SetKey(Value *v, const char *key)
{
    return z_ValueSetKey(v, key);
}
static inline BOOL SetKeyLen(Value *v, const char *key, SIZE len)
{
    return z_ValueSetKeyLen(v, key, len);
}
static inline void SetArray(Value *v)
{
    z_ValueSetArray(v);
}
static inline void SetObj(Value *v)
{
    z_ValueSetObj(v);
}
static inline BOOL SetFast(Value *v, Value *vv)
{
    return z_ValueSetFast(v, vv);
}
static inline BOOL Set(Value *v, const Value *vv)
{
    return z_ValueSet(v, vv);
}
static inline BOOL ObjAddFast(Value *v, Value *vv)
{
    return z_ValueObjAddFast(v, vv);
}
static inline BOOL ObjAdd(Value *v, const Value *vv)
{
    return z_ValueObjAdd(v, vv);
}
static inline BOOL ArrayAddFast(Value *v, Value *vv)
{
    return z_ValueArrayAddFast(v, vv);
}
static inline BOOL ArrayAdd(Value *v, const Value *vv)
{
    return z_ValueArrayAdd(v, vv);
}
static inline BOOL ArrayDel(Value *v, SIZE index)
{
    return z_ValueArrayDel(v, index);
}
static inline BOOL ObjDel(Value *v, const char *key)
{
    return z_ValueObjDel(v, key);
}
#endif

// 内存拷贝函数
static inline void z_Copy(const char *src, z_SIZE len, char *des)
{
    memcpy(des, src, len);
}

// 字符串长度计算函数
static inline z_SIZE z_StrLen(const char *str)
{
    return (z_SIZE)strlen(str);
}

static inline double z_StrToDouble(const char *str)
{
    return atof(str);
}

static inline void z_DoubleToStr(double d, char *buff)
{
    sprintf(buff, "%.17g", d);
}

// 字符串比较，a必须以0结束，len为b的长度。
static inline z_BOOL z_StrIsEqual(const char *a, const char *b, z_SIZE len)
{
    z_SIZE i;
    for (i = 0; z_LIKELY(i < len); ++i)
    {
        if (z_LIKELY(a[i] != b[i]))
        {
            return z_False;
        }
    }
    // a字符串必须结束才能算相等
    if (z_LIKELY(a[i] == 0))
        return z_True;
    return z_False;
}

// 字符串比较，len为b的长度。
static inline z_BOOL z_StrIsEqualLen(const char *a, z_SIZE a_len, const char *b, z_SIZE b_len)
{
    if (z_LIKELY(a_len != b_len)) {
        return z_False;
    }
    z_SIZE i;
    for (i = 0; z_LIKELY(i < a_len); ++i)
    {
        if (z_LIKELY(a[i] != b[i]))
        {
            return z_False;
        }
    }
    return z_True;
}

// 内存分配器节点
struct z_ANode
{
    // 数据地址
    char *Data;
    // 数据大小
    z_SIZE Size;
    // 使用到的位置
    z_SIZE Pos;
    // 下一个节点
    struct z_ANode *Next;
};

// 内存分配器
// 内存分配器为由内存分配器节点组成的链表，Root为根节点，End总是指向最后一个节点
struct z_Allocator
{
    // 根节点
    struct z_ANode *Root;
    // 最后一个节点
    struct z_ANode *End;
};

// 函数说明详见《API》
static inline struct z_Allocator *z_AllocatorNew()
{
    // 每次分配一大块内存，避免多次分配
    void *ptr = z_New(sizeof(struct z_Allocator) + sizeof(struct z_ANode) + z_AllocatorInitMemSize);
    struct z_Allocator *alloc = (struct z_Allocator *)ptr;
    alloc->Root = (struct z_ANode *)((char *)ptr + sizeof(struct z_Allocator));
    alloc->End = alloc->Root;

    alloc->Root->Size = z_AllocatorInitMemSize;
    alloc->Root->Data = (char *)ptr + sizeof(struct z_Allocator) + sizeof(struct z_ANode);
    alloc->Root->Pos = 0;
    alloc->Root->Next = 0;
    return alloc;
}

// 函数说明详见《API》
static inline void z_AllocatorRelease(struct z_Allocator *alloc)
{
    // 遍历整个链表，每次释放一块内存
    struct z_ANode *next = alloc->Root->Next;
    while (z_LIKELY(next != 0))
    {
        struct z_ANode *nn = next->Next;
        z_Free((void *)next);
        next = nn;
    }
    // 最后释放第一块内存
    z_Free((void *)alloc);
}

// 追加一个大小为 init_size 的节点。
static inline void z_AllocatorAppendChild(z_SIZE init_size, struct z_Allocator *alloc)
{
    // 每次分配一大块内存，避免多次分配
    void *ptr = z_New(sizeof(struct z_ANode) + init_size);
    struct z_ANode *node = (struct z_ANode *)ptr;
    node->Size = init_size;
    node->Data = (char *)ptr + sizeof(struct z_ANode);
    node->Pos = 0;
    node->Next = 0;
    alloc->End->Next = node;
    alloc->End = node;
    return;
}

// 函数说明详见《API》
static inline char *z_AllocatorAlloc(struct z_Allocator *alloc, z_SIZE size)
{
    struct z_ANode *cur_node = alloc->End;
    z_SIZE s = cur_node->Size;
    if (z_UNLIKELY(cur_node->Pos + size > s))
    {
        s *= z_Delta;
        // 通过循环计算最终需要的空间大小
        // 这里应该有更好的方法，就是直接通过计算所得
        while (z_UNLIKELY(size > s))
            s *= z_Delta;
        z_AllocatorAppendChild(s, alloc);
        cur_node = alloc->End;
    }
    char *ret = cur_node->Data + cur_node->Pos;
    cur_node->Pos += size;
    return ret;
}

// 字符串
// 字符串由内存分配器分配内存，当追加操作导致内存不够时，直接分配 z_Delta 倍内存，并把旧内存拷贝到新内存中
struct z_String
{
    // 数据
    char *Data;
    // 位置
    z_SIZE Pos;
    // 使用内存大小
    z_SIZE Size;
    // 分配器
    struct z_Allocator *A;
};

static z_String *z_StringCache = 0;

// 新建一个字符串
static inline struct z_String *z_StringNew(struct z_Allocator *alloc, z_SIZE init_size)
{
    struct z_String *str = (struct z_String *)z_AllocatorAlloc(alloc, sizeof(struct z_String) + init_size);
    str->Size = init_size;
    str->Data = (char *)str + sizeof(struct z_String);
    str->Pos = 0;
    str->A = alloc;
    return str;
}

// 清空一个字符串
static inline void z_StringReset(struct z_String *str) 
{
    str->Pos = 0;
}

// 追加字符串
static inline void z_StringAppendStr(struct z_String *str, const char *s, z_SIZE size)
{
    z_SIZE src_s = str->Size;
    if (z_UNLIKELY(str->Pos + size > src_s))
    {
        src_s *= z_Delta;
        while (z_UNLIKELY(str->Pos + size > src_s))
            src_s *= z_Delta;
        const char *src_d = str->Data;
        str->Data = (char *)z_AllocatorAlloc(str->A, src_s);
        str->Size = src_s;
        z_Copy(src_d, str->Pos, str->Data);
    }
    z_Copy(s, size, str->Data + str->Pos);
    str->Pos += size;
}

// 追加字符
static inline void z_StringAppendChar(struct z_String *str, const char c)
{
    z_SIZE src_s = str->Size;
    if (z_UNLIKELY(str->Pos + 1 > src_s))
    {
        src_s *= z_Delta;
        const char *src_d = str->Data;
        str->Data = (char *)z_AllocatorAlloc(str->A, src_s);
        str->Size = src_s;
        z_Copy(src_d, str->Pos, str->Data);
    }
    *(str->Data + str->Pos) = c;
    str->Pos += 1;
}

// 追加结束符
static inline void z_StringAppendEnd(struct z_String *str)
{
    z_StringAppendChar(str, 0);
}

// 获得字符串
static inline const char *z_StringStr(struct z_String *str)
{
    return str->Data;
}

// zzzJSON把文本转化成内存中的一棵树，z_Node为该数的节点，每个节点对应一个值
struct z_Node
{
    // 节点代表的值的类型
    char Type;

    // 节点代表的值的关键字
    const char *Key;
    // 节点代表的值的关键字长度
    z_SIZE KeyLen;

    union {
        // 如果节点代表的值的类型为数组或者对象，则表示数组或者对象的第一个值对应的节点
        struct z_Node *Node;
        // 如果节点代表的值的类型为字符串，数字，布尔值，则对应其字符串
        const char *Str;
    } Value;

    // 节点对应的值包含值的个数，如果类型非对象或者数组，则为0
    z_SIZE Len;

    // 下一个节点
    struct z_Node *Next;
    // 上一个节点
    struct z_Node *Prev;
    // 父节点
    struct z_Node *Father;
    // 最后一个节点
    struct z_Node *End;
};

// zzzJSON的基本单位：值，包含一个节点和一个内存分配器
struct z_Value
{
    struct z_Node *N;
    struct z_Allocator *A;
    const char *CacheStr;
    union {
        const char *UnEscapeStr;
        const double *Num;
    } Cache;
    const char *CacheKey;
    const char *CacheUnEscapeKey;
};

// 把Cache置零的函数
static inline void z_ValueInitCache(struct z_Value *v)
{
    v->CacheStr = 0;
    v->Cache.UnEscapeStr = 0;
    v->CacheKey = 0;
    v->CacheUnEscapeKey = 0;
}

// 函数说明详见《API》
static inline struct z_Value *z_ValueNew(struct z_Allocator *alloc)
{
    struct z_Value *v = (struct z_Value *)z_AllocatorAlloc(alloc, sizeof(struct z_Value));
    v->A = alloc;
    v->N = 0;
    z_ValueInitCache(v);
    return v;
}

// 创建一个值，并赋值
static inline struct z_Value *z_ValueInnerNew(struct z_Allocator *alloc, struct z_Node *n)
{
    struct z_Value *v = (struct z_Value *)z_AllocatorAlloc(alloc, sizeof(struct z_Value));
    v->A = alloc;
    v->N = n;
    z_ValueInitCache(v);
    return v;
}

// 跳过空格、tab、换行符
static inline z_BOOL z_Skin(const char c)
{
    if (z_UNLIKELY(
            z_UNLIKELY(c == ' ') ||
            z_UNLIKELY(c == '\t') ||
            z_UNLIKELY(c == '\n') ||
            z_UNLIKELY(c == '\r')))
    {
        return z_True;
    }
    return z_False;
}

// 下一个有效字符
static inline char z_Peek(const char *s, z_SIZE *index)
{
    while (z_UNLIKELY(z_Skin(s[*index])))
        ++(*index);
    return s[(*index)++];
}

// 消费一个字符
static inline z_BOOL z_Consume(const char c, const char *s, z_SIZE *index)
{
    if (s[*index] == c)
    {
        ++(*index);
        return z_True;
    }
    return z_False;
}

// 预期消费一个字符成功
static inline z_BOOL z_LikelyConsume(const char c, const char *s, z_SIZE *index)
{
    if (z_LIKELY(s[*index] == c))
    {
        ++(*index);
        return z_True;
    }
    return z_False;
}

// 预期消费一个字符失败
static inline z_BOOL z_UnLikelyConsume(const char c, const char *s, z_SIZE *index)
{
    if (z_UNLIKELY(s[*index] == c))
    {
        ++(*index);
        return z_True;
    }
    return z_False;
}

// 预期消费下一个有效字符成功
static inline z_BOOL z_LikelyPeekAndConsume(const char c, const char *s, z_SIZE *index)
{
    while (z_UNLIKELY(z_Skin(s[*index])))
        ++(*index);
    if (z_LIKELY(s[*index] == c))
    {
        ++(*index);
        return z_True;
    }
    return z_False;
}

// 预期消费下一个有效字符失败
static inline z_BOOL z_UnLikelyPeekAndConsume(const char c, const char *s, z_SIZE *index)
{
    while (z_UNLIKELY(z_Skin(s[*index])))
        ++(*index);
    if (z_UNLIKELY(s[*index] == c))
    {
        ++(*index);
        return z_True;
    }
    return z_False;
}

// 消费False
static inline z_BOOL z_ConsumeFalse(const char *s, z_SIZE *index)
{
    if (z_LIKELY(*((z_SIZE *)("alse")) == *((z_SIZE *)(s + *index)))) {
        *index += 4;
        return z_True;
    }
    return z_False;
}

// 消费True
static inline z_BOOL z_ConsumeTrue(const char *s, z_SIZE *index)
{
    if (z_LIKELY(*((z_SIZE *)z_StrTrue) == *((z_SIZE *)(s + *index - 1)))) {
        *index += 3;
        return z_True;
    }
    return z_False;
}

// 消费Null
static inline z_BOOL z_ConsumeNull(const char *s, z_SIZE *index)
{
    if (z_LIKELY(*((z_SIZE *)z_StrNull) == *((z_SIZE *)(s + *index - 1)))) {
        *index += 3;
        return z_True;
    }
    return z_False;
}

// 十六进制对应的十进制数字
static inline z_SIZE z_HexCodePoint(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 16;
}

// 专为 z_ValueGetUnEscapeString 使用
static inline z_SIZE z_HexCodePointForUnEscape(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return c - 'a' + 10;
}

// 消费一个十六进制字符
static inline z_BOOL z_ConsumeHexOne(const char *s, z_SIZE *index, z_SIZE *cp)
{
    z_SIZE tcp = z_HexCodePoint(s[*index]);
    if (z_LIKELY(tcp < 16))
    {
        *cp = *cp << 4;
        *cp += tcp;
        ++(*index);
        return z_True;
    }
    return z_False;
}

// 专为 z_ValueGetUnEscapeString 使用
static inline void z_ConsumeHexOneForUnEscape(const char *s, z_SIZE *index, z_SIZE *cp)
{
    *cp = *cp << 4;
    *cp += z_HexCodePointForUnEscape(s[*index]);
    ++(*index);
    return;
}

// 消费4个十六进制字符
static inline z_BOOL z_ConsumeHex(const char *s, z_SIZE *index, z_SIZE *cp)
{
    if (z_LIKELY(z_LIKELY(z_ConsumeHexOne(s, index, cp)) &&
                   z_LIKELY(z_ConsumeHexOne(s, index, cp)) &&
                   z_LIKELY(z_ConsumeHexOne(s, index, cp)) &&
                   z_LIKELY(z_ConsumeHexOne(s, index, cp))))
    {
        return z_True;
    }
    return z_False;
}

// 专为 zz_ValueGetUnEscapeString 使用
static inline void z_ConsumeHexForUnEscape(const char *s, z_SIZE *index, z_SIZE *cp)
{
    z_ConsumeHexOneForUnEscape(s, index, cp);
    z_ConsumeHexOneForUnEscape(s, index, cp);
    z_ConsumeHexOneForUnEscape(s, index, cp);
    z_ConsumeHexOneForUnEscape(s, index, cp);
    return;
}

// 专为 zz_ValueGetUnEscapeString 使用，追加一个字符
static inline void z_Append(char *s, z_SIZE *index, char c)
{
    s[(*index)++] = c;
}

// 专为 zz_ValueGetUnEscapeString 使用，追加一个UTF8字符
static inline void z_AppendUTF8(char *s, z_SIZE *index, z_SIZE codepoint)
{

    // UTF8的规则，具体请参考 UNICODE 相关文档
    if (codepoint <= 0x7F)
    {
        z_Append(s, index, (char)(codepoint & 0xFF));
    }
    else if (codepoint <= 0x7FF)
    {
        z_Append(s, index, (char)(0xC0 | ((codepoint >> 6) & 0xFF)));
        z_Append(s, index, (char)(0x80 | ((codepoint & 0x3F))));
    }
    else if (codepoint <= 0xFFFF)
    {
        z_Append(s, index, (char)(0xE0 | ((codepoint >> 12) & 0xFF)));
        z_Append(s, index, (char)(0x80 | ((codepoint >> 6) & 0x3F)));
        z_Append(s, index, (char)(0x80 | (codepoint & 0x3F)));
    }
    else
    {
        z_Append(s, index, (char)(0xF0 | ((codepoint >> 18) & 0xFF)));
        z_Append(s, index, (char)(0x80 | ((codepoint >> 12) & 0x3F)));
        z_Append(s, index, (char)(0x80 | ((codepoint >> 6) & 0x3F)));
        z_Append(s, index, (char)(0x80 | (codepoint & 0x3F)));
    }
}

// 专为 zz_ValueGetUnEscapeString 使用，追加一个结束符号
static inline void z_AppendEnd(char *s, z_SIZE *index)
{
    z_Append(s, index, 0);
}

static inline void z_UnEscapeString(const char *str, z_SIZE len, char *s)
{
    z_SIZE s_index = 0;
    z_SIZE index;
    char c;
    for (index = 0; index < len;)
    {
        c = str[index];
        // 如果是一个合法的JSON字符串，那么\后面一定有字符，因此，一定不会越界
        if (z_UNLIKELY(c == '\\'))
        {
            c = str[index + 1];
            switch (c)
            {
            case '"':
            {
                z_Append(s, &s_index, '\"');
                index += 2;
                break;
            }
            case '\\':
            {
                z_Append(s, &s_index, '\\');
                index += 2;
                break;
            }
            case 'b':
            {
                z_Append(s, &s_index, '\b');
                index += 2;
                break;
            }
            case 'f':
            {
                z_Append(s, &s_index, '\f');
                index += 2;
                break;
            }
            case 'n':
            {
                z_Append(s, &s_index, '\n');
                index += 2;
                break;
            }
            case 'r':
            {
                z_Append(s, &s_index, '\r');
                index += 2;
                break;
            }
            case 't':
            {
                z_Append(s, &s_index, '\t');
                index += 2;
                break;
            }
            case '/':
            {
                z_Append(s, &s_index, '/');
                index += 2;
                break;
            }
            case 'u':
            {
                index += 2;
                z_SIZE cp = 0;
                z_ConsumeHexForUnEscape(str, &index, &cp);

                if (z_UNLIKELY(cp >= 0xD800 && cp <= 0xDBFF))
                {
                    z_SIZE cp1 = 0;
                    index += 2;
                    z_ConsumeHexForUnEscape(str, &index, &cp1);
                    cp = (((cp - 0xD800) << 10) | (cp1 - 0xDC00)) + 0x10000;
                }
                z_AppendUTF8(s, &s_index, cp);
                break;
            }
            }
        }
        else
        {
            z_Append(s, &s_index, c);
            index += 1;
        }
    }
    z_AppendEnd(s, &s_index);
    return;
}

// 消费字符串
static inline z_BOOL z_ConsumeStr(const char *s, z_SIZE *index)
{
    char c;
    c = s[(*index)++];
    while (z_LIKELY(c != 0))
    {
        if (z_UNLIKELY((unsigned char)c <= 0x1f))
            return z_False;
        if (z_UNLIKELY(c == '\\'))
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
                z_SIZE cp = 0;
                if (z_LIKELY(z_ConsumeHex(s, index, &cp)))
                {
                    // 这里是UTF16标准，可以参考网上相关资料
                    // 搜索关键字 UNICODE
                    if (z_UNLIKELY(cp >= 0xDC00 && cp <= 0xDFFF))
                        return z_False;
                    if (z_UNLIKELY(cp >= 0xD800 && cp <= 0xDBFF))
                    {
                        if (z_LIKELY(z_LikelyConsume('\\', s, index) && z_LikelyConsume('u', s, index)))
                        {
                            z_SIZE cp2 = 0;
                            if (z_LIKELY(z_ConsumeHex(s, index, &cp2)))
                            {
                                if (z_UNLIKELY(cp2 < 0xDC00 || cp2 > 0xDFFF))
                                    return z_False;
                            }
                            else
                            {
                                return z_False;
                            }
                        }
                        else
                        {
                            return z_False;
                        }
                    }
                    c = s[(*index)++];
                }
                else
                {
                    return z_False;
                }
                continue;
            }
            default:
                return z_False;
            }
        }
        if (z_UNLIKELY(c == '"'))
        {
            return z_True;
        }
        c = s[(*index)++];
    }
    return z_False;
}

// 检查一个字符串是否符合JSON标准，主要用于 SetStr
static inline z_BOOL z_CheckStr(const char *s, z_SIZE *len)
{
    z_SIZE index = 0;
    char c;
    c = s[index++];
    while (z_LIKELY(c != 0))
    {
        if (z_UNLIKELY(z_UNLIKELY((unsigned char)c <= 0x1f) || z_UNLIKELY(c == '"')))
            return z_False;
        if (z_UNLIKELY(c == '\\'))
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
                z_SIZE cp = 0;
                if (z_LIKELY(z_ConsumeHex(s, &index, &cp)))
                {
                    // 这里是UTF16标准，可以参考网上相关资料
                    // 搜索关键字 UNICODE
                    if (z_UNLIKELY(cp >= 0xDC00 && cp <= 0xDFFFF))
                        return z_False;
                    if (z_UNLIKELY(cp >= 0xD800 && cp <= 0xDBFF))
                    {
                        if (z_LIKELY(z_LikelyConsume('\\', s, &index) && z_LikelyConsume('u', s, &index)))
                        {
                            z_SIZE cp2 = 0;
                            if (z_LIKELY(z_ConsumeHex(s, &index, &cp2)))
                            {
                                if (z_UNLIKELY(cp2 < 0xDC00 || cp2 > 0xDFFF))
                                    return z_False;
                            }
                            else
                            {
                                return z_False;
                            }
                        }
                        else
                        {
                            return z_False;
                        }
                    }
                    c = s[index++];
                }
                else
                {
                    return z_False;
                }
                continue;
            }
            default:
                return z_False;
            }
        }
        c = s[index++];
    }
    *len = index - 1;
    return z_True;
}
static inline z_BOOL z_CheckStrLen(struct z_Allocator *alloc, const char *s, z_SIZE len)
{
    if (z_UNLIKELY(z_StringCache == 0)) {
        z_StringCache = z_StringNew(alloc, z_StringCacheInitMemSize);
    } else {
        z_StringReset(z_StringCache);
    }
    z_StringAppendStr(z_StringCache, s, len);
    z_StringAppendEnd(z_StringCache);
    z_SIZE avail_len;
    if (z_UNLIKELY(z_CheckStr(z_StringStr(z_StringCache), &avail_len) == z_False)){
        return z_False;
    }
    if (z_UNLIKELY(avail_len != len)) return z_False;
    return z_True;
}

// 消费一个数字
static inline z_BOOL z_ConsumeNum(const char *s, z_SIZE *index)
{
    --(*index);

    z_BOOL minus = z_Consume('-', s, index);

    if (z_UnLikelyConsume('0', s, index))
    {
    }
    else if (z_LIKELY(z_LIKELY(s[*index] >= '1') && z_LIKELY(s[*index] <= '9')))
    {
        char c = s[++(*index)];
        while (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
        {
            c = s[++(*index)];
        }
    }
    else
    {
        return z_False;
    }

    if (z_Consume('.', s, index))
    {
        char c = s[*index];
        if ((z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
        {
            c = s[++(*index)];
            while (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
            {
                c = s[++(*index)];
            }
        }
        else
        {
            return z_False;
        }
    }

    if (s[*index] == 'e' || s[*index] == 'E')
    {
        char c = s[++(*index)];
        if (c == '-')
        {
            ++(*index);
        }
        else if (c == '+')
        {
            ++(*index);
        }
        c = s[*index];
        if (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
        {
            c = s[++(*index)];
            while (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
            {
                c = s[++(*index)];
            }
        }
        else
        {
            return z_False;
        }
    }

    return z_True;
}

// 检查一个数字是否符合JSON标准，主要用于 SetNum
static inline z_BOOL z_CheckNum(const char *s, z_SIZE *len)
{
    z_SIZE index = 0;

    z_BOOL minus = z_Consume('-', s, &index);

    if (z_UnLikelyConsume('0', s, &index))
    {
    }
    else if (z_LIKELY(z_LIKELY(s[index] >= '1') && z_LIKELY(s[index] <= '9')))
    {
        char c = s[++index];
        while (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
        {
            c = s[++index];
        }
    }
    else
    {
        return z_False;
    }

    if (z_Consume('.', s, &index))
    {
        char c = s[index];
        if ((z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
        {
            c = s[++index];
            while (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
            {
                c = s[++index];
            }
        }
        else
        {
            return z_False;
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
        if (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
        {
            c = s[++index];
            while (z_LIKELY(z_LIKELY(c >= '0') && z_LIKELY(c <= '9')))
            {
                c = s[++index];
            }
        }
        else
        {
            return z_False;
        }
    }
    *len = index;
    return z_LikelyConsume(0, s, &index);
}
static inline z_BOOL z_CheckNumLen(struct z_Allocator *alloc, const char *s, z_SIZE len)
{
    if (z_UNLIKELY(z_StringCache == 0)) {
        z_StringCache = z_StringNew(alloc, z_StringCacheInitMemSize);
    } else {
        z_StringReset(z_StringCache);
    }
    z_StringAppendStr(z_StringCache, s, len);
    z_StringAppendEnd(z_StringCache);
    z_SIZE avail_len;
    if (z_UNLIKELY(z_CheckNum(z_StringStr(z_StringCache), &avail_len) == z_False)){
        return z_False;
    }
    if (z_UNLIKELY(avail_len != len)) return z_False;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueParseFast(struct z_Value *v, const char *s)
{
    z_ValueInitCache(v);

    struct z_Node *src_node;
    if (z_LIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Prev = 0;
        v->N->Next = 0;
        v->N->Father = 0;
        v->N->Key = 0;
        src_node = 0;
    }
    else
    {
        src_node = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        *src_node = *v->N;
    }
    z_SIZE index = 0;
    struct z_Node *node = v->N;

    char c = z_Peek(s, &index);
    switch (c)
    {
    case '[':
    {
        node->Type = z_JSONTYPEARRAY;
        if (z_UnLikelyPeekAndConsume(']', s, &index))
        {
            node->Value.Node = 0;
            node->Len = 0;
            break;
        }
        struct z_Node *n = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
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
        node->Type = z_JSONTYPEOBJ;
        if (z_UnLikelyPeekAndConsume('}', s, &index))
        {
            node->Value.Node = 0;
            node->Len = 0;
            break;
        }
        struct z_Node *n = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
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
        if (z_LIKELY(z_ConsumeNull(s, &index)))
        {
            node->Type = z_JSONTYPENULL;
            node->Value.Str = z_StrNull;
            node->Len = 4;
            break;
        }
        if (z_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return z_False;
    case 'f':
        if (z_LIKELY(z_ConsumeFalse(s, &index)))
        {
            node->Type = z_JSONTYPEBOOL;
            node->Value.Str = z_StrFalse;
            node->Len = 5;
            break;
        }
        if (z_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return z_False;
    case 't':
        if (z_LIKELY(z_ConsumeTrue(s, &index)))
        {
            node->Type = z_JSONTYPEBOOL;
            node->Value.Str = z_StrTrue;
            node->Len = 4;
            break;
        }
        if (z_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return z_False;
    case '"':
    {
        z_SIZE start = index;
        if (z_UNLIKELY(z_UnLikelyConsume('"', s, &index)))
        {
            node->Type = z_JSONTYPESTRING;
            node->Value.Str = s + index;
            node->Len = 0;
            break;
        }
        if (z_LIKELY(z_ConsumeStr(s, &index)))
        {
            node->Type = z_JSONTYPESTRING;
            node->Value.Str = s + start;
            node->Len = index - start - 1;
            break;
        }
        if (z_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return z_False;
    }
    default:
    {
        z_SIZE start = index - 1;
        if (z_LIKELY(z_ConsumeNum(s, &index)))
        {
            node->Type = z_JSONTYPENUM;
            node->Value.Str = s + start;
            node->Len = index - start;
            break;
        }
        if (z_LIKELY(src_node == 0))
            v->N = src_node;
        else
            *v->N = *src_node;
        return z_False;
    }
    }
    while (z_LIKELY(node != v->N))
    {
        if (node->Father->Type == z_JSONTYPEOBJ)
        {
            if (z_UNLIKELY(z_LikelyPeekAndConsume('"', s, &index) == z_False))
            {
                if (z_LIKELY(src_node == 0))
                    v->N = src_node;
                else
                    *v->N = *src_node;
                return z_False;
            }
            z_SIZE start = index;
            if (z_UNLIKELY(z_UnLikelyConsume('"', s, &index)))
            {
                node->Key = s + start;
                node->KeyLen = 0;
            }
            else
            {
                if (z_UNLIKELY(z_ConsumeStr(s, &index) == z_False))
                {
                    if (z_LIKELY(src_node == 0))
                        v->N = src_node;
                    else
                        *v->N = *src_node;
                    return z_False;
                }
                node->Key = s + start;
                node->KeyLen = index - start - 1;
            }
            if (z_UNLIKELY(z_LikelyPeekAndConsume(':', s, &index) == z_False))
            {
                if (z_LIKELY(src_node == 0))
                    v->N = src_node;
                else
                    *v->N = *src_node;
                return z_False;
            }
        }
        else
        {
            node->Key = 0;
        }
        c = z_Peek(s, &index);
        switch (c)
        {
        case '[':
        {
            node->Type = z_JSONTYPEARRAY;
            if (z_UnLikelyPeekAndConsume(']', s, &index))
            {
                node->Value.Node = 0;
                node->Len = 0;
                break;
            }
            struct z_Node *n = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
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
            node->Type = z_JSONTYPEOBJ;
            if (z_UnLikelyPeekAndConsume('}', s, &index))
            {
                node->Value.Node = 0;
                node->Len = 0;
                break;
            }
            struct z_Node *n = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
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
            if (z_LIKELY(z_ConsumeNull(s, &index)))
            {
                node->Type = z_JSONTYPENULL;
                node->Value.Str = z_StrNull;
                node->Len = 4;
                break;
            }
            if (z_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return z_False;
        case 'f':
            if (z_LIKELY(z_ConsumeFalse(s, &index)))
            {
                node->Type = z_JSONTYPEBOOL;
                node->Value.Str = z_StrFalse;
                node->Len = 5;
                break;
            }
            if (z_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return z_False;
        case 't':
            if (z_LIKELY(z_ConsumeTrue(s, &index)))
            {
                node->Type = z_JSONTYPEBOOL;
                node->Value.Str = z_StrTrue;
                node->Len = 4;
                break;
            }
            if (z_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return z_False;
        case '"':
        {
            z_SIZE start = index;
            if (z_UNLIKELY(z_UnLikelyConsume('"', s, &index)))
            {
                node->Type = z_JSONTYPESTRING;
                node->Value.Str = s + index;
                node->Len = 0;
                break;
            }
            if (z_LIKELY(z_ConsumeStr(s, &index)))
            {
                node->Type = z_JSONTYPESTRING;
                node->Value.Str = s + start;
                node->Len = index - start - 1;
                break;
            }
            if (z_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return z_False;
        }
        default:
        {
            z_SIZE start = index - 1;
            if (z_LIKELY(z_ConsumeNum(s, &index)))
            {
                node->Type = z_JSONTYPENUM;
                node->Value.Str = s + start;
                node->Len = index - start;
                break;
            }
            if (z_LIKELY(src_node == 0))
                v->N = src_node;
            else
                *v->N = *src_node;
            return z_False;
        }
        }
        while (z_LIKELY(node != v->N))
        {
            if (z_LikelyPeekAndConsume(',', s, &index))
            {
                struct z_Node *n = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
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
                char c = z_Peek(s, &index);
                if (z_LIKELY((c == '}' && z_LIKELY(node->Father->Type == z_JSONTYPEOBJ)) ||
                               z_LIKELY(z_LIKELY(c == ']') && z_LIKELY(node->Father->Type == z_JSONTYPEARRAY))))
                {
                    node->Next = 0;
                    node = node->Father;
                }
                else
                {
                    if (z_LIKELY(src_node == 0))
                        v->N = src_node;
                    else
                        *v->N = *src_node;
                    return z_False;
                }
            }
        }
    }
    if (z_LIKELY(z_LikelyPeekAndConsume(0, s, &index)))
    {
        return z_True;
    }
    if (z_LIKELY(src_node == 0))
        v->N = src_node;
    else
        *v->N = *src_node;
    return z_False;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueParseLen(struct z_Value *v, const char *s, z_SIZE len)
{
    char *str = z_AllocatorAlloc(v->A, len + 1);
    z_Copy(s, len, str);
    str[len] = 0;
    return z_ValueParseFast(v, s);
}

// 函数说明详见《API》
static inline z_BOOL z_ValueParse(struct z_Value *v, const char *s)
{
    return z_ValueParseLen(v, s, z_StrLen(s));
}

// 函数说明详见《API》
static inline const char *z_ValueStringify(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return "";
    struct z_String *str = z_StringNew(v->A, z_StringInitMemSize);
    struct z_Node *node = v->N;

    switch (node->Type)
    {
    case z_JSONTYPEARRAY:
        z_StringAppendChar(str, '[');
        if (node->Value.Node != 0)
        {
            node = node->Value.Node;
        }
        else
        {
            z_StringAppendChar(str, ']');
        }
        break;
    case z_JSONTYPEOBJ:
        z_StringAppendChar(str, '{');
        if (node->Len != 0)
        {
            node = node->Value.Node;
        }
        else
        {
            z_StringAppendChar(str, '}');
        }
        break;
    case z_JSONTYPESTRING:
        z_StringAppendChar(str, '"');
        z_StringAppendStr(str, node->Value.Str, node->Len);
        z_StringAppendChar(str, '"');
        break;
    default:
        z_StringAppendStr(str, node->Value.Str, node->Len);
        break;
    }

    while (z_LIKELY(node != v->N))
    {
        if (node->Key != 0)
        {
            z_StringAppendChar(str, '"');
            z_StringAppendStr(str, node->Key, node->KeyLen);
            z_StringAppendStr(str, "\":", 2);
        }
        switch (node->Type)
        {
        case z_JSONTYPEARRAY:
            z_StringAppendChar(str, '[');
            if (node->Value.Node != 0)
            {
                node = node->Value.Node;
                continue;
            }
            else
            {
                z_StringAppendChar(str, ']');
            }
            break;
        case z_JSONTYPEOBJ:
            z_StringAppendChar(str, '{');
            if (node->Len != 0)
            {
                node = node->Value.Node;
                continue;
            }
            else
            {
                z_StringAppendChar(str, '}');
            }
            break;
        case z_JSONTYPESTRING:
            z_StringAppendChar(str, '"');
            z_StringAppendStr(str, node->Value.Str, node->Len);
            z_StringAppendChar(str, '"');
            break;
        default:
            z_StringAppendStr(str, node->Value.Str, node->Len);
            break;
        }
        while (z_LIKELY(node != v->N))
        {
            if (z_LIKELY(node->Next != 0))
            {
                z_StringAppendChar(str, ',');
                node = node->Next;
                break;
            }
            else
            {
                node = node->Father;
                if (node->Type == z_JSONTYPEARRAY)
                {
                    z_StringAppendChar(str, ']');
                }
                else
                {
                    z_StringAppendChar(str, '}');
                }
            }
        }
    }
    z_StringAppendEnd(str);
    return z_StringStr(str);
}

// 函数说明详见《API》
static inline const char *z_ValueGetStringFast(const struct z_Value *v, z_SIZE *len)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPESTRING))
        return 0;
    *len = v->N->Len;
    return v->N->Value.Str;
}

// 函数说明详见《API》
static inline const char *z_ValueGetString(struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPESTRING))
        return 0;
    if (z_UNLIKELY(v->CacheStr != 0))
        return v->CacheStr;
    char *str = z_AllocatorAlloc(v->A, v->N->Len + 1);
    z_Copy(v->N->Value.Str, v->N->Len, str);
    str[v->N->Len] = 0;
    v->CacheStr = str;
    return str;
}

// 函数说明详见《API》
static inline const char *z_ValueGetUnEscapeString(struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPESTRING))
        return 0;
    if (z_UNLIKELY(v->Cache.UnEscapeStr != 0))
        return v->Cache.UnEscapeStr;
    // 因为值里面的字符串一定是合法的字符串，因此，可以简化
    // 因为解码后的字符串一定比解码前短，所以申请原字符串+1的长度即可
    char *ret_str = z_AllocatorAlloc(v->A, v->N->Len + 1);
    z_UnEscapeString(v->N->Value.Str, v->N->Len, ret_str);
    v->Cache.UnEscapeStr = ret_str;
    return ret_str;
}

// 函数说明详见《API》
static inline const char *z_ValueGetNumFast(const struct z_Value *v, z_SIZE *len)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPENUM))
        return 0;
    *len = v->N->Len;
    return v->N->Value.Str;
}

// 函数说明详见《API》
static inline const char *z_ValueGetNumStr(struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPENUM))
        return 0;
    if (z_UNLIKELY(v->CacheStr != 0))
        return v->CacheStr;
    char *str = z_AllocatorAlloc(v->A, v->N->Len + 1);
    z_Copy(v->N->Value.Str, v->N->Len, str);
    str[v->N->Len] = 0;
    v->CacheStr = str;
    return str;
}

// 函数说明详见《API》
static inline const double *z_ValueGetNum(struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPENUM))
        return 0;
    if (z_UNLIKELY(v->Cache.Num != 0))
        return v->Cache.Num;
    double *d = (double *)z_AllocatorAlloc(v->A, sizeof(d) / sizeof(char));
    *d = z_StrToDouble(v->N->Value.Str);
    v->Cache.Num = d;
    return d;
}

// 函数说明详见《API》
static inline const z_BOOL *z_ValueGetBool(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEBOOL))
        return 0;
    if (*(v->N->Value.Str) == 't')
        return &z_True;
    return &z_False;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueIsNull(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return z_False;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPENULL))
        return z_False;
    return z_True;
}

// 函数说明详见《API》
static inline const char *z_ValueGetKey(struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Key == 0))
        return 0;

    if (z_UNLIKELY(v->CacheKey != 0))
        return v->CacheKey;

    char *str = z_AllocatorAlloc(v->A, v->N->KeyLen + 1);
    z_Copy(v->N->Key, v->N->KeyLen, str);
    str[v->N->KeyLen] = 0;
    v->CacheKey = str;
    return str;
}

static inline const char *z_ValueGetUnEscapeKey(struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Key == 0))
        return 0;

    if (z_UNLIKELY(v->CacheUnEscapeKey != 0))
        return v->CacheUnEscapeKey;
    char *str = z_AllocatorAlloc(v->A, v->N->KeyLen + 1);
    z_UnEscapeString(v->N->Key, v->N->KeyLen, str);
    v->CacheUnEscapeKey = str;
    return str;
}

// 函数说明详见《API》
static inline const char *z_ValueGetKeyFast(const struct z_Value *v, z_SIZE *len)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Key == 0))
        return 0;
    *len = v->N->KeyLen;
    return v->N->Key;
}

// 函数说明详见《API》
static inline struct z_Value *z_ValueObjGet(const struct z_Value *v, const char *key)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEOBJ))
        return 0;
    struct z_Node *next = v->N->Value.Node;
    while (z_LIKELY(next != 0))
    {
        if (z_UNLIKELY(z_StrIsEqual(key, next->Key, next->KeyLen) == z_True))
        {
            struct z_Value *ret_val = z_ValueInnerNew(v->A, next);
            return ret_val;
        }
        next = next->Next;
    }
    return 0;
}

// 函数说明详见《API》
static inline struct z_Value *z_ValueObjGetLen(const struct z_Value *v, const char *key, z_SIZE len)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEOBJ))
        return 0;
    struct z_Node *next = v->N->Value.Node;
    while (z_LIKELY(next != 0))
    {
        if (z_UNLIKELY(z_StrIsEqualLen(key, len, next->Key, next->KeyLen) == z_True))
        {
            struct z_Value *ret_val = z_ValueInnerNew(v->A, next);
            return ret_val;
        }
        next = next->Next;
    }
    return 0;
}

// 函数说明详见《API》
static inline const z_JSONType *z_ValueType(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    switch (v->N->Type)
    {
    case z_JSONTYPEARRAY:
        return &z_JSONTypeArray;
    case z_JSONTYPEOBJ:
        return &z_JSONTypeObj;
    case z_JSONTYPESTRING:
        return &z_JSONTypeString;
    case z_JSONTYPENUM:
        return &z_JSONTypeNum;
    case z_JSONTYPEBOOL:
        return &z_JSONTypeBool;
    case z_JSONTYPENULL:
        return &z_JSONTypeNull;
    }
    return 0;
}

// 函数说明详见《API》
static inline z_SIZE z_ValueSize(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEOBJ && v->N->Type != z_JSONTYPEARRAY))
        return 0;
    return v->N->Len;
}

// 函数说明详见《API》
static inline struct z_Value *z_ValueArrayGet(const struct z_Value *v, z_SIZE index)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEARRAY))
        return 0;
    struct z_Node *next = v->N->Value.Node;
    z_SIZE i = 0;
    while (z_LIKELY(next != 0))
    {
        if (z_UNLIKELY(i == index))
        {
            struct z_Value *ret_val = z_ValueInnerNew(v->A, next);
            return ret_val;
        }
        next = next->Next;
        ++i;
    }
    return 0;
}

// 函数说明详见《API》
static inline struct z_Value *z_ValueBegin(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEOBJ && v->N->Type != z_JSONTYPEARRAY))
        return 0;
    if (z_LIKELY(v->N->Value.Node != 0))
    {
        struct z_Value *ret_val = z_ValueInnerNew(v->A, v->N->Value.Node);
        return ret_val;
    }
    return 0;
}

// 函数说明详见《API》
static inline struct z_Value *z_ValueNext(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    if (z_LIKELY(v->N->Next != 0))
    {
        struct z_Value *ret_val = z_ValueInnerNew(v->A, v->N->Next);
        return ret_val;
    }
    return 0;
}

// 函数说明详见《API》
static inline struct z_Value *z_ValueCopy(const struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return 0;
    struct z_Value *const ret_val = z_ValueNew(v->A);
    struct z_Allocator *const A = v->A;
    ret_val->N = (struct z_Node *)z_AllocatorAlloc(A, sizeof(struct z_Node));
    ret_val->N->Prev = 0;
    ret_val->N->Next = 0;
    ret_val->N->Father = 0;

    struct z_Node *node = v->N;
    struct z_Node *des_node = ret_val->N;

    do
    {
        des_node->Type = node->Type;
        if (node->Key != 0)
        {
            char *k = z_AllocatorAlloc(A, node->KeyLen);
            z_Copy(node->Key, node->KeyLen, k);
            des_node->Key = k;
            des_node->KeyLen = node->KeyLen;
        }
        else
        {
            des_node->Key = 0;
        }
        switch (node->Type)
        {
        case z_JSONTYPEOBJ:
        case z_JSONTYPEARRAY:
            des_node->Len = node->Len;
            if (z_LIKELY(node->Value.Node != 0))
            {
                node = node->Value.Node;
                struct z_Node *n = (struct z_Node *)z_AllocatorAlloc(A, sizeof(struct z_Node));
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
        case z_JSONTYPEBOOL:
        case z_JSONTYPENULL:
            des_node->Value.Str = node->Value.Str;
            des_node->Len = node->Len;
            break;
        case z_JSONTYPENUM:
        case z_JSONTYPESTRING:
        {
            char *s = z_AllocatorAlloc(A, node->Len);
            z_Copy(node->Value.Str, node->Len, s);
            des_node->Value.Str = s;
            des_node->Len = node->Len;
        }
        break;
        }
        while (z_LIKELY(node != v->N))
        {
            if (z_LIKELY(node->Next != 0))
            {
                node = node->Next;
                struct z_Node *n = (struct z_Node *)z_AllocatorAlloc(A, sizeof(struct z_Node));
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
    } while (z_LIKELY(node != v->N));

    return ret_val;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueMove(struct z_Value *v)
{
    if (z_UNLIKELY(v->N == 0))
        return z_False;
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
    return z_True;
}

// 函数说明详见《API》
static inline void z_ValueSetNull(struct z_Value *v)
{
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPENULL;
    v->N->Value.Str = z_StrNull;
    v->N->Len = 4;
}

// 函数说明详见《API》
static inline void z_ValueSetBool(struct z_Value *v, z_BOOL b)
{
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPEBOOL;
    if (b == z_False)
    {
        v->N->Value.Str = z_StrFalse;
        v->N->Len = 5;
        return;
    }
    v->N->Value.Str = z_StrTrue;
    v->N->Len = 4;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetNumStrFast(struct z_Value *v, const char *num)
{
    z_SIZE len = 0;
    if (z_UNLIKELY(z_CheckNum(num, &len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPENUM;
    v->N->Value.Str = num;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetNumStrLenFast(struct z_Value *v, const char *num, z_SIZE len)
{
    if (z_UNLIKELY(z_CheckNumLen(v->A, num, len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPENUM;
    v->N->Value.Str = num;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetNumStr(struct z_Value *v, const char *num)
{
    z_SIZE len = 0;
    if (z_UNLIKELY(z_CheckNum(num, &len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    char *s = z_AllocatorAlloc(v->A, len);
    z_Copy(num, len, s);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPENUM;
    v->N->Value.Str = s;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetNumStrLen(struct z_Value *v, const char *num, z_SIZE len)
{
    if (z_UNLIKELY(z_CheckNumLen(v->A, num, len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    char *s = z_AllocatorAlloc(v->A, len);
    z_Copy(num, len, s);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPENUM;
    v->N->Value.Str = s;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetNum(struct z_Value *v, const double d)
{
    // double最大精度是17，使用sprintf转出字符串，%.17g，最大长度不超过32
    char *buff = z_AllocatorAlloc(v->A, 32);
    z_DoubleToStr(d, buff);
    return z_ValueSetNumStrFast(v, buff);
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetStrFast(struct z_Value *v, const char *str)
{
    z_SIZE index = 0;
    z_SIZE len = 0;
    if (z_UNLIKELY(z_CheckStr(str, &len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPESTRING;
    v->N->Value.Str = str;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetStrLenFast(struct z_Value *v, const char *str, z_SIZE len)
{
    if (z_UNLIKELY(z_CheckStrLen(v->A, str, len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPESTRING;
    v->N->Value.Str = str;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetStr(struct z_Value *v, const char *str)
{
    z_SIZE index = 0;
    z_SIZE len = 0;
    if (z_UNLIKELY(z_CheckStr(str, &len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    char *s = z_AllocatorAlloc(v->A, len);
    z_Copy(str, len, s);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPESTRING;
    v->N->Value.Str = s;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetStrLen(struct z_Value *v, const char *str, z_SIZE len)
{
    if (z_UNLIKELY(z_CheckStrLen(v->A, str, len) == z_False))
        return z_False;
    z_ValueInitCache(v);
    char *s = z_AllocatorAlloc(v->A, len);
    z_Copy(str, len, s);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPESTRING;
    v->N->Value.Str = s;
    v->N->Len = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetKeyFast(struct z_Value *v, const char *key)
{
    z_SIZE len = 0;
    if (z_UNLIKELY(z_CheckStr(key, &len) == z_False))
    {
        return z_False;
    }
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = z_JSONTYPENULL;
        v->N->Value.Str = z_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && z_UNLIKELY(v->N->Father->Type != z_JSONTYPEOBJ))
    {
        return z_False;
    }
    z_ValueInitCache(v);
    v->N->Key = key;
    v->N->KeyLen = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetKeyLenFast(struct z_Value *v, const char *key, z_SIZE len)
{
    if (z_UNLIKELY(z_CheckStrLen(v->A, key, len) == z_False))
    {
        return z_False;
    }
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = z_JSONTYPENULL;
        v->N->Value.Str = z_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && z_UNLIKELY(v->N->Father->Type != z_JSONTYPEOBJ))
    {
        return z_False;
    }
    z_ValueInitCache(v);
    v->N->Key = key;
    v->N->KeyLen = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetKey(struct z_Value *v, const char *key)
{
    z_SIZE len = 0;
    if (z_UNLIKELY(z_CheckStr(key, &len) == z_False))
    {
        return z_False;
    }
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = z_JSONTYPENULL;
        v->N->Value.Str = z_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && z_UNLIKELY(v->N->Father->Type != z_JSONTYPEOBJ))
    {
        return z_False;
    }
    z_ValueInitCache(v);
    char *s = z_AllocatorAlloc(v->A, len);
    z_Copy(key, len, s);
    v->N->Key = s;
    v->N->KeyLen = len;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetKeyLen(struct z_Value *v, const char *key, z_SIZE len)
{
    if (z_UNLIKELY(z_CheckStrLen(v->A, key, len) == z_False))
    {
        return z_False;
    }
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
        v->N->Type = z_JSONTYPENULL;
        v->N->Value.Str = z_StrNull;
        v->N->Len = 4;
    }
    else if (v->N->Father != 0 && z_UNLIKELY(v->N->Father->Type != z_JSONTYPEOBJ))
    {
        return z_False;
    }
    z_ValueInitCache(v);
    char *s = z_AllocatorAlloc(v->A, len);
    z_Copy(key, len, s);
    v->N->Key = s;
    v->N->KeyLen = len;
    return z_True;
}

// 函数说明详见《API》
static inline void z_ValueSetArray(struct z_Value *v)
{
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPEARRAY;
    v->N->Value.Node = 0;
    v->N->Len = 0;
    return;
}

// 函数说明详见《API》
static inline void z_ValueSetObj(struct z_Value *v)
{
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = (struct z_Node *)z_AllocatorAlloc(v->A, sizeof(struct z_Node));
        v->N->Key = 0;
        v->N->Prev = 0;
        v->N->Father = 0;
        v->N->Next = 0;
    }
    v->N->Type = z_JSONTYPEOBJ;
    v->N->Value.Node = 0;
    v->N->Len = 0;
    return;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSetFast(struct z_Value *v, struct z_Value *vv)
{
    if (z_UNLIKELY(z_ValueMove(vv) == z_False))
        return z_False;
    z_ValueInitCache(v);
    z_ValueInitCache(vv);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = vv->N;
        vv->N = 0;
        return z_True;
    }
    v->N->Type = vv->N->Type;
    if (v->N->Key != 0 && vv->N->Key != 0)
    {
        v->N->Key = vv->N->Key;
        v->N->KeyLen = vv->N->KeyLen;
    }
    v->N->Value = vv->N->Value;
    v->N->Len = vv->N->Len;
    if (v->N->Type == z_JSONTYPEARRAY || v->N->Type == z_JSONTYPEOBJ)
    {
        v->N->End = vv->N->End;
        struct z_Node *next = v->N->Value.Node;
        while (z_LIKELY(next != 0))
        {
            next->Father = v->N;
            next = next->Next;
        }
    }
    vv->N = 0;
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueSet(struct z_Value *v, const struct z_Value *vv)
{
    struct z_Value *cp = z_ValueCopy(vv);
    if (z_UNLIKELY(cp == 0))
        return z_False;
    z_ValueInitCache(v);
    if (z_UNLIKELY(v->N == 0))
    {
        v->N = cp->N;
        return z_True;
    }
    v->N->Type = cp->N->Type;
    if (v->N->Key != 0 && vv->N->Key != 0)
    {
        v->N->Key = cp->N->Key;
        v->N->KeyLen = cp->N->KeyLen;
    }
    v->N->Value = cp->N->Value;
    v->N->Len = cp->N->Len;
    if (v->N->Type == z_JSONTYPEARRAY || v->N->Type == z_JSONTYPEOBJ)
    {
        v->N->End = vv->N->End;
        struct z_Node *next = v->N->Value.Node;
        while (z_LIKELY(next != 0))
        {
            next->Father = v->N;
            next = next->Next;
        }
    }
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueObjAddFast(struct z_Value *v, struct z_Value *vv)
{
    if (z_UNLIKELY(v->N == 0))
        return z_False;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEOBJ))
        return z_False;
    if (z_UNLIKELY(vv->N == 0))
        return z_False;
    if (z_UNLIKELY(vv->N->Key == 0))
        return z_False;
    if (z_UNLIKELY(z_ValueMove(vv) == z_False))
        return z_False;
    z_ValueInitCache(vv);
    vv->N->Father = v->N;
    if (z_UNLIKELY(v->N->Value.Node == 0))
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
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueObjAdd(struct z_Value *v, const struct z_Value *vv)
{
    if (z_UNLIKELY(v->N == 0))
        return z_False;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEOBJ))
        return z_False;
    if (z_UNLIKELY(vv->N == 0))
        return z_False;
    if (z_UNLIKELY(vv->N->Key == 0))
        return z_False;
    struct z_Value *cp = z_ValueCopy(vv);
    if (z_UNLIKELY(cp == 0))
        return z_False;
    cp->N->Father = v->N;
    if (z_UNLIKELY(v->N->Value.Node == 0))
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
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueArrayAddFast(struct z_Value *v, struct z_Value *vv)
{
    if (z_UNLIKELY(v->N == 0))
        return z_False;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEARRAY))
        return z_False;
    if (z_UNLIKELY(z_ValueMove(vv) == z_False))
        return z_False;
    z_ValueInitCache(vv);
    vv->N->Key = 0;
    vv->N->Father = v->N;
    if (z_UNLIKELY(v->N->Value.Node == 0))
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
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueArrayAdd(struct z_Value *v, const struct z_Value *vv)
{
    if (z_UNLIKELY(v->N == 0))
        return z_False;
    if (z_UNLIKELY(v->N->Type != z_JSONTYPEARRAY))
        return z_False;
    struct z_Value *cp = z_ValueCopy(vv);
    if (z_UNLIKELY(cp == 0))
        return z_False;
    cp->N->Key = 0;
    cp->N->Father = v->N;
    if (z_UNLIKELY(v->N->Value.Node == 0))
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
    return z_True;
}

// 函数说明详见《API》
static inline z_BOOL z_ValueArrayDel(struct z_Value *v, z_SIZE index)
{
    struct z_Value *dv = z_ValueArrayGet(v, index);
    if (z_UNLIKELY(dv == 0))
        return z_False;
    return z_ValueMove(dv);
}

// 函数说明详见《API》
static inline z_BOOL z_ValueObjDel(struct z_Value *v, const char *key)
{
    struct z_Value *dv = z_ValueObjGet(v, key);
    if (z_UNLIKELY(dv == 0))
        return z_False;
    return z_ValueMove(dv);
}
#endif