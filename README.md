# zzzJSON

![](https://raw.githubusercontent.com/dacez/zzzJSON/master/resource/zzzJSON.jpg)
[简体中文](https://github.com/dacez/zzzjson/blob/master/READMECHN.md)
- [zzzJSON](#zzzjson)
- [Abstract](#abstract)
- [Parse on use](#parse-on-use)
- [Benchmarks and Measurements](#benchmarks-and-measurements)
  * [Conformance](#conformance)
  * [Performance](#performance)
  * [Results](#results)
    + [environment](#environment)
    + [conformance](#conformance)
    + [performance](#performance)
      - [No Number JSON](#no-number-json)
      - [JSON From nativejson-benchmark](#json-from-nativejson-benchmark)
      - [JSON From Taobao](#json-from-taobao)
      - [All JSON Above](#all-json-above)
      - [Random Short JSON](#random-short-json)
      - [Random Long  JSON](#random-long--json)
- [Example](#example)
- [API](#api)
  * [Short APIs](#short-apis)
  * [Long APIs](#long-apis)

# Abstract

+ Fastest parsing and serialization speed.

+ Wrote by pure C, no ambiguous grammar, easy to understand, compile, use and modify.

+ Tencent is used on a large scale.

# Parse on use

Like copy on write, zzzJSON only parse numbers when using get APIs. 
zzzJSON only judge whether it is a correct number string when parsing  JSON string into tree structure.

# Benchmarks and Measurements
> Reference by nativejson-benchmark

## Conformance

Benchmark        | Description
-----------------|---------------------------------------------------
Parse Validation | Use [JSON_checker](http://www.json.org/JSON_checker/) test suite to test whether the library can identify valid and invalid JSONs. (`fail01.json` is excluded as it is relaxed in [RFC7159]. `fail18.json` is excluded as depth of JSON is not specified.)
Parse Double     | 66 JSONs, each with a decimal value in an array, are parsed. The parsed `double` values are compared to the correct answer.
Parse String     | 9 JSONs, each with a string value in an array, are parsed. The parsed strings are compared to the correct answer.
Roundtrip        | 27 condensed JSONs are parsed and stringified. The results are compared to the original JSONs.

## Performance

Benchmark      | Description
---------------|----------------------------------------------------
Parse          | Parse in-memory JSON into tree structure.
Stringify      | Serialize tree structure into condensed JSON in memory.

## Results

### environment

| CPU                 | OS            | Compiler  | Optimization |
| ------------------- | ------------------- | ------- | -------- |
| i7-6700 （3.40GHZ） | Ubuntu 18.04（WSL） | G++-7.3 | O3       |

### conformance

zzzJSON pass all tests.(soure code: conformance_test.cpp)

### performance

#### No Number JSON
| Parser   | Parse(ms) | Stringify(ms) | All(ms) |
| ------------ | -------- | ---------- | -------- |
|cjson                  |162                    |122                    |284                    |
|gason                  |60                     |321                    |398                    |
|jsoncpp                |830                    |556                    |1588                   |
|nlohmannjson           |994                    |312                    |1414                   |
|picojson               |824                    |295                    |1214                   |
|rapidjson              |128                    |92                     |224                    |
|rapidjsonfp            |147                    |91                     |245                    |
|rapidjsonstr           |127                    |89                     |225                    |
|taocppjson             |724                    |261                    |1070                   |
|zzzjson                |86                     |56                     |149                    |

####  JSON From nativejson-benchmark

| Parser   | Parse(ms) | Stringify(ms) | All(ms) |
| ------------ | -------- | ---------- | -------- |
|arduinojson            |779                    |25                     |807                    |
|cjson                  |53                     |183                    |230                    |
|gason                  |8                      |88                     |93                     |
|jsoncpp                |88                     |126                    |221                    |
|nlohmannjson           |62                     |24                     |91                     |
|parson                 |52                     |277                    |336                    |
|picojson               |54                     |108                    |165                    |
|rapidjson              |11                     |15                     |23                     |
|rapidjsonfp            |20                     |17                     |39                     |
|rapidjsonstr           |24                     |12                     |37                     |
|taocppjson             |34                     |29                     |66                     |
|zzzjson                |18                     |7                      |22                     |

####  JSON From Taobao

| Parser   | Parse(ms) | Stringify(ms) | All(ms) |
| ------------ | -------- | ---------- | -------- |
|arduinojson            |23                     |62                     |87                     |
|cjson                  |38                     |23                     |60                     |
|gason                  |9                      |51                     |63                     |
|jsoncpp                |67                     |51                     |126                    |
|nlohmannjson           |72                     |29                     |105                    |
|parson                 |48                     |85                     |141                    |
|picojson               |54                     |35                     |96                     |
|rapidjson              |18                     |12                     |28                     |
|rapidjsonfp            |18                     |14                     |30                     |
|rapidjsonstr           |16                     |12                     |29                     |
|taocppjson             |47                     |28                     |86                     |
|zzzjson                |14                     |7                      |23                     |

#### All JSON Above

| Parser   | Parse(ms) | Stringify(ms) | All(ms) |
| ------------ | -------- | ---------- | -------- |
|arduinojson            |2056                   |505                    |2508                   |
|cjson                  |243                    |345                    |596                    |
|gason                  |63                     |431                    |500                    |
|jsoncpp                |451                    |431                    |961                    |
|nlohmannjson           |518                    |217                    |840                    |
|parson                 |309                    |857                    |1214                   |
|picojson               |405                    |387                    |862                    |
|rapidjson              |97                     |119                    |220                    |
|rapidjsonfp            |113                    |126                    |245                    |
|rapidjsonstr           |116                    |121                    |240                    |
|taocppjson             |278                    |207                    |561                    |
|zzzjson                |95                     |59                     |166                    |

#### Random Short JSON

| Parser   | Parse(ms) | Stringify(ms) | All(ms) |
| ------------ | -------- | ---------- | -------- |
|cjson                  |100                    |107                    |200                    |
|jsoncpp                |136                    |127                    |284                    |
|nlohmannjson           |177                    |58                     |242                    |
|picojson               |94                     |95                     |205                    |
|rapidjson              |31                     |39                     |56                     |
|rapidjsonfp            |36                     |19                     |63                     |
|rapidjsonstr           |37                     |35                     |60                     |
|taocppjson             |90                     |50                     |143                    |
|zzzjson                |26                     |16                     |38                     |

#### Random Long  JSON

| Parser   | Parse(ms) | Stringify(ms) | All(ms) |
| ------------ | -------- | ---------- | -------- |
|cjson                  |81                     |67                     |155                    |
|jsoncpp                |130                    |112                    |244                    |
|nlohmannjson           |199                    |42                     |254                    |
|picojson               |127                    |93                     |222                    |
|rapidjson              |24                     |17                     |45                     |
|rapidjsonfp            |28                     |24                     |46                     |
|rapidjsonstr           |30                     |24                     |48                     |
|taocppjson             |100                    |34                     |149                    |
|zzzjson                |20                     |9                      |29                     |

You can use the commands below to get the results above
```shell
./build.sh all
```

# Example

```c
#include "zzzjson.h"
#include <stdio.h>

// DeepCopy
void GetAndSet(Value *srcv, Value *desv)
{
    // GetType
    const JSONType *t;
    t = Type(srcv);
    if (t == 0)
        return;
    switch (*t)
    {
    case JSONTYPEARRAY:
    {
        // DeepCopy Array
        SetArray(desv);
        Value *next = Begin(srcv);
        while (next != 0)
        {
            Value *v = NewValue(desv->A);
            GetAndSet(next, v);
            if (ArrayAddFast(desv, v) != True)
                return;
            next = Next(next);
        }
        break;
    }
    case JSONTYPEOBJECT:
    {
        // DeepCopy Object
        SetObj(desv);
        Value *next = Begin(srcv);
        while (next != 0)
        {
            Value *v = NewValue(desv->A);
            SetKeyFast(v, GetKey(next));
            GetAndSet(next, v);
            if (ObjAddFast(desv, v) != True)
                return;
            next = Next(next);
        }
        break;
    }
    case JSONTYPEBOOL:
    {
        // Copy Bool 
        const zzz_BOOL *b = GetBool(srcv);
        if (b == 0)
            return;
        SetBool(desv, *b);
        break;
    }
    case JSONTYPENULL:
    {
        // Copy nil
        if (IsNull(srcv) == False)
            return;
        SetNull(desv);
        break;
    }

    case JSONTYPESTRING:
    {
        // DeepCopy String
        const char *str = GetStr(srcv);
        if (str == 0)
            return;
        if (SetStrFast(desv, str) != True)
            return;
        break;
    }
    case JSONTYPENUMBER:
    {
        // DeepCopy Number
        const char *str = GetNumStr(srcv);
        if (str == 0)
            return;
        if (SetNumStrFast(desv, str) != True)
            return;
        break;
    }
    }
}
int main()
{
    const char *src_json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"],null]";
    // New Allocator
    Allocator *A = NewAllocator();
    // New Value
    Value *src_v = NewValue(A);
    Value *des_v = NewValue(A);
    // Parse
    BOOL ret = ParseFast(src_v, src_json);
    if (ret != True)
    {
        printf("ParseFast Fail!\n");
        return 1;
    }
    // DeepCopy
    GetAndSet(src_v, des_v);
    // Stringify
    const char *des_json = Stringify(des_v);
    printf("src_json:%s\n", src_json);
    if (des_json != 0)
        printf("des_json:%s\n", des_json);
    // Free Allocator
    ReleaseAllocator(A);
    return 0;
}
```

# API

## Short APIs

```C
Allocator *NewAllocator()
void ReleaseAllocator(Allocator *root_alloc)
Value *NewValue(Allocator *alloc)
BOOL ParseFast(Value *v, const char *s)
BOOL ParseLen(Value *v, const char *s, SIZE len)
BOOL Parse(Value *v, const char *s)
const char *Stringify(const Value *v)
const char *GetStrFast(const Value *v, SIZE *len)
const char *GetUnEscapeStr(Value *v)
const char *GetStr(Value *v)
const char *GetNumFast(const Value *v, zzz_SIZE *len)
const char *GetNumStr(Value *v)
const double *GetNum(Value *v)
const double *GetDouble(struct zzz_Value *v)
const int *GetInt(struct zzz_Value *v)
const long *GetLong(struct zzz_Value *v)
const long long *GetLongLong(struct zzz_Value *v)
const BOOL *GetBool(const Value *v)
BOOL IsNull(const Value *v)
const char *GetKey(Value *v)
const char *GetUnEscapeKey(Value *v)
const char *GetKeyFast(const Value *v, SIZE *len)
Value *ObjGet(const Value *v, const char *key)
Value *ObjGetLen(const Value *v, const char *key, SIZE len)
const JSONType *Type(const Value *v)
SIZE Size(const Value *v)
Value *ArrayGet(const Value *v, SIZE index)
Value *Begin(const Value *v)
Value *Next(const Value *v)
Value *Copy(const Value *v)
BOOL Move(Value *v)
BOOL SetNull(Value *v)
BOOL SetBool(Value *v, BOOL b)
BOOL SetNumStrFast(Value *v, const char *num)
BOOL SetNumStrLenFast(Value *v, const char *num, SIZE len)
BOOL SetNumStr(Value *v, const char *num)
BOOL SetNumStrLen(Value *v, const char *num, SIZE len)
BOOL SetNum(Value *v, const double d)
BOOL SetDouble(Value *v, const double d)
BOOL SetInt(Value *v, const int d)
BOOL SetLong(Value *v, const long d)
BOOL SetLongLong(Value *v, const long long d)
BOOL SetStrFast(Value *v, const char *str)
BOOL SetStrLenFast(Value *v, const char *str, SIZE len)
BOOL SetStr(Value *v, const char *str)
BOOL SetStrLen(Value *v, const char *str, SIZE len)
BOOL SetStrEscape(Value *v, const char *str)
BOOL SetStrLenEscape(Value *v, const char *str, SIZE len)
BOOL SetKeyFast(Value *v, const char *key)
BOOL SetKeyLenFast(Value *v, const char *key, SIZE len)
BOOL SetKey(Value *v, const char *key)
BOOL SetKeyLen(Value *v, const char *key, SIZE len)
BOOL SetKeyEscape(Value *v, const char *key)
BOOL SetKeyLenEscape(Value *v, const char *key, SIZE len)
BOOL SetArray(Value *v)
BOOL SetObj(Value *v)
BOOL SetFast(Value *v, Value *vv)
BOOL Set(Value *v, const Value *vv)
BOOL ObjAddFast(Value *v, Value *vv)
BOOL ObjAdd(Value *v, const Value *vv)
BOOL ArrayAddFast(Value *v, Value *vv)
BOOL ArrayAdd(Value *v, const Value *vv)
BOOL ArrayDel(Value *v, SIZE index)
BOOL ObjDel(Value *v, const char *key)
```



## Long APIs

> define “zzz_SHORT_API 0” to use long APIs
>
> \#define zzz_SHORT_API 0
>
> // must be above to include
>
> \#include "zzzjson.h"

```C
struct zzz_Allocator *zzz_AllocatorNew();
void zzz_AllocatorRelease(struct zzz_Allocator *root_alloc);
struct zzz_Value *zzz_ValueNew(struct zzz_Allocator *alloc);
zzz_BOOL zzz_ValueParseFast(struct zzz_Value *v, const char *s);
zzz_BOOL zzz_ValueParseLen(struct zzz_Value *v, const char *s, zzz_SIZE len);
zzz_BOOL zzz_ValueParse(struct zzz_Value *v, const char *s);
const char *zzz_ValueStringify(const struct zzz_Value *v);
const char *zzz_ValueGetStrFast(const struct zzz_Value *v, zzz_SIZE *len);
const char *zzz_ValueGetUnEscapeStr(struct zzz_Value *v);
const char *zzz_ValueGetStr(struct zzz_Value *v);
const char *zzz_ValueGetNumFast(const struct zzz_Value *v, zzz_SIZE *len);
const char *zzz_ValueGetNumStr(struct zzz_Value *v);
const double *zzz_ValueGetNum(struct zzz_Value *v);
const double *zzz_ValueGetDouble(struct zzz_Value *v);
const int *zzz_ValueGetInt(struct zzz_Value *v);
const long *zzz_ValueGetLong(struct zzz_Value *v);
const long long *zzz_ValueGetLongLong(struct zzz_Value *v);
const zzz_BOOL *zzz_ValueGetBool(const struct zzz_Value *v);
zzz_BOOL zzz_ValueIsNull(const struct zzz_Value *v);
const char *zzz_ValueGetKey(struct zzz_Value *v);
const char *zzz_ValueGetUnEscapeKey(struct zzz_Value *v);
const char *zzz_ValueGetKeyFast(const struct zzz_Value *v, zzz_SIZE *len);
struct zzz_Value *zzz_ValueObjGet(const struct zzz_Value *v, const char *key);
struct zzz_Value *zzz_ValueObjGetLen(const struct zzz_Value *v, const char *key, zzz_SIZE len);
const zzz_JSONType *zzz_ValueType(const struct zzz_Value *v);
zzz_SIZE zzz_ValueSize(const struct zzz_Value *v);
struct zzz_Value *zzz_ValueArrayGet(const struct zzz_Value *v, zzz_SIZE index);
struct zzz_Value *zzz_ValueBegin(const struct zzz_Value *v);
struct zzz_Value *zzz_ValueNext(const struct zzz_Value *v);
struct zzz_Value *zzz_ValueCopy(const struct zzz_Value *v);
zzz_BOOL zzz_ValueMove(struct zzz_Value *v);
zzz_BOOL zzz_ValueSetNull(struct zzz_Value *v);
zzz_BOOL zzz_ValueSetBool(struct zzz_Value *v, zzz_BOOL b);
zzz_BOOL zzz_ValueSetNumStrFast(struct zzz_Value *v, const char *num);
zzz_BOOL zzz_ValueSetNumStrLenFast(struct zzz_Value *v, const char *num, zzz_SIZE len);
zzz_BOOL zzz_ValueSetNumStr(struct zzz_Value *v, const char *num);
zzz_BOOL zzz_ValueSetNumStrLen(struct zzz_Value *v, const char *num, zzz_SIZE len);
zzz_BOOL zzz_ValueSetNum(struct zzz_Value *v, const double d);
zzz_BOOL zzz_ValueSetDouble(struct zzz_Value *v, const double d);
zzz_BOOL zzz_ValueSetInt(struct zzz_Value *v, const int n);
zzz_BOOL zzz_ValueSetLong(struct zzz_Value *v, const long n);
zzz_BOOL zzz_ValueSetLongLong(struct zzz_Value *v, const long long n);
zzz_BOOL zzz_ValueSetStrFast(struct zzz_Value *v, const char *str);
zzz_BOOL zzz_ValueSetStrLenFast(struct zzz_Value *v, const char *str, zzz_SIZE len);
zzz_BOOL zzz_ValueSetStr(struct zzz_Value *v, const char *str);
zzz_BOOL zzz_ValueSetStrLen(struct zzz_Value *v, const char *str, zzz_SIZE len);
zzz_BOOL zzz_ValueSetStrEscape(struct zzz_Value *v, const char *str);
zzz_BOOL zzz_ValueSetStrLenEscape(struct zzz_Value *v, const char *str, zzz_SIZE len);
zzz_BOOL zzz_ValueSetKeyFast(struct zzz_Value *v, const char *key);
zzz_BOOL zzz_ValueSetKeyLenFast(struct zzz_Value *v, const char *key, zzz_SIZE len);
zzz_BOOL zzz_ValueSetKey(struct zzz_Value *v, const char *key);
zzz_BOOL zzz_ValueSetKeyLen(struct zzz_Value *v, const char *key, zzz_SIZE len);
zzz_BOOL zzz_ValueSetKeyEscape(struct zzz_Value *v, const char *key);
zzz_BOOL zzz_ValueSetKeyLenEscape(struct zzz_Value *v, const char *key, zzz_SIZE len);
zzz_BOOL zzz_ValueSetArray(struct zzz_Value *v);
zzz_BOOL zzz_ValueSetObj(struct zzz_Value *v);
zzz_BOOL zzz_ValueSetFast(struct zzz_Value *v, struct zzz_Value *vv);
zzz_BOOL zzz_ValueSet(struct zzz_Value *v, const struct zzz_Value *vv);
zzz_BOOL zzz_ValueObjAddFast(struct zzz_Value *v, struct zzz_Value *vv);
zzz_BOOL zzz_ValueObjAdd(struct zzz_Value *v, const struct zzz_Value *vv);
zzz_BOOL zzz_ValueArrayAddFast(struct zzz_Value *v, struct zzz_Value *vv);
zzz_BOOL zzz_ValueArrayAdd(struct zzz_Value *v, const struct zzz_Value *vv);
zzz_BOOL zzz_ValueArrayDel(struct zzz_Value *v, zzz_SIZE index);
zzz_BOOL zzz_ValueObjDel(struct zzz_Value *v, const char *key);
```



