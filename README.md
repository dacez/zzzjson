# zzzJSON

[TOC]

## 一、概述

zzzJSON是一个纯C实现的JSON解析器，代码朴素，注释丰富，简单易懂，拥有极致的JSON解析和序列化速度，提供对JSON数据的读写功能，同时，仅依赖glibc且所有代码均包含在**一个**头文件中，使用极其方便，特别适用以下场景：

+ 读取或者修改一个大JSON中的某几个值的场景；
+ 多个JSON合并成一个JSON的场景；
+ 判断JSON文本是否正确的场景；
+ 无法容忍精度损失的场景;
+ 纯C编码的场景；
+ JSON文本中数字较少的场景。

### zzzJSON的宗旨

zzzJSON从诞生之日起始终坚持以下宗旨：

1. 极其朴素的代码，丰富的文档和注释，没有任何花销的技巧，简单易读好修改；
2. 极致的解析和序列化速度；
3. 读时解析；
4. 纯C实现，轻松嵌入其它语言；
5. 单个头文件；

**极其朴素的代码，丰富的文档和注释，没有任何花销的技巧，简单易读好修改**是zzzJSON的第一宗旨，因为放心地使用开源代码的前提是能够驾驭开源代码，zzzJSON使用最朴素的语法，没有任何花销，简单易读，丰富的文档和注释使使用者在需要增加新特性时，能够轻松修改，最大限度地降低使用者的心智负担。

**极致的解析和序列化速度**是zzzJSON的灵魂，在解析和序列化方面，zzzJSON的表现非常惊艳。

**读时解析**是zzzJSON的特色，在业务开发过程中，经常需要修改一个大JSON的一个小部分，或者获取其中一个值，读时解析在解析JSON文本时只判断数字的正确性，而不把数字转换成数字类型，最大限度地减少解析数字造成的性能开销。使用时可以根据业务要求，选择不同精度的解析函数，甚至使用超过双精度浮点型的精度。

**纯C实现**是zzzJSON通用化的保证，使其能够轻松嵌入各种高级语言中，例如：C++，Go，Rust等，最大限度地扩展了其应用范围。

**单个头文件**是zzzJSON方便使用的前提，所有代码包含在一个文件中，使用只需包含头文件即可。

### 性能测试简介

zzzJSON满足所有JSON标准，同时在多种纬度下提供最快的解析和序列化速度，详细介绍请参考《性能测试》。

### 数据结构简介

zzzJSON定义了三种类型：**32位无符号整形**、**布尔类型**和**JSON类型**，为了方便使用还定义了2个布尔类型的常量和6个JSON类型的常量，同时定义3个结构：**内存分配器**、**值**和**字符串**，其中字符串仅供内部使用，详细介绍请参考《数据结构》。

### API简介

zzzJSON提供丰富的API，主要包括**内存操作**、**解析与序列化**和**读写操作**，涉及到拷贝操作的API都有快速版本，使用快速版本能够获得更好的性能，详见《API》。

### 内存配置简介

zzzJSON为了达到极致的解析和序列化速度，实现了内存分配，因此，可以根据不同的场景配置内存分配的参数，详见《内存配置》。 

### 环境适配简介

zzzJSON适配绝大部分的环境，如果环境不能适配，则需要进行简单的移植，在牺牲部分性能的情况下，可以做到只依赖C++编译器，不依赖任何头文件和库，详见《环境适配》 。

## 二、性能测试

性能测试分为两部分：正确性测试和速度测试，正确性测试用于保障zzzJSON满足所有JSON标志，速度测试用于证明zzzJSON的解析和序列化速度。

### 测试环境

| CPU                 | 操作系统            | 编译器  | 优化等级 |
| ------------------- | ------------------- | ------- | -------- |
| i7-6700 （3.40GHZ） | Ubuntu 18.04（WSL） | G++-7.3 | O3       |

### 正确性测试

zzzJSON的正确性测试参考nativejson-benchmark的测试方法，从以下方面进行测试：

+ JSON官方网站提供的正确与不正确的JSON样本测试（层数限制那个样本除外）；
+ 字符串测试；
+ 双精度浮点型测试；
+ 来回测试（解析后序列化再对比）。

zzzJSON通过以上四个测试，详细的代码在 conformance_test.cpp 文件中。

### 性能测试

zzzJSON从多方面测试其解析和序列化速度，主要包括：无数字数据测试、nativejson-benchmark数据测试、淘宝数据测试、混合数据测试、随机长JSON测试和随机短JSON测试，每个测试分别测试解析耗时、序列化耗时和全部耗时，其中全部耗时包括创建对象、解析、序列化和析构的耗时总和，详细的代码在 performance_test.cpp 文件中。以下测试包含了C/C++的主流JSON解析器，其中RapidJSON开启了SIMD。zzzJSON不使用SIMD的主要原因是：1. 不能使用相同的二进制统一部署；2. 增加代码复杂度。

无数字数据测试、nativejson-benchmark数据测试、淘宝数据测试和混合数据测试在每次测试的时候均重启进程，防止命中缓存。随机长JSON测试和随机短JSON测试在同一个进程中进行，可以体现批量处理JSON文本时的性能。

#### 性能调优

由于测试的数据比较大，可以对zzzJSON对内存参数进行了调优，会提高性能，这里为了公平起见，zzzJSON使用默认参数，（调大会加快速度，例如：#define ZZZ_DELTA 4），具体如下：

```c
// 内存分配器初次分配内存大小（默认值）
#define ZZZ_ALLOCATORINITMEMSIZE 1024 * 4
// 字符串初次大小（默认值）
#define ZZZ_STRINGINITMEMSIZE 1024
// 内存不够时，内存翻倍数（默认值）
#define ZZZ_DELTA 2
```

内存调优的详细说明请参考《内存配置》。

#### 无数字数据测试

由于zzzJSON使用读时解析，因此，在解析的时候只判断数字的正确性，而不把数字转化成浮点数。大部分JSON解析器都会把数字转化为浮点型，为了保证公平，使用无数字的JSON文本则能够避免这种情况，测试结果如下：
> 以下所有数据单位为ms。

| 解析器名字   | 解析耗时 | 序列化耗时 | 全部耗时 |
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

以上测试表明，在无数字数据测试中，zzzJSON的速度最快。
> rapidJSON全部开启了SIMD，其中rapidJSON为默认的rapidJSON，rapidJSONFP为支持全精度的rapidJSON，rapidJSONSTR为把数字解析为字符串的rapidJSON。
>
> 由于系统有多个进程在跑，因此，数据会有波动，例如：解析耗时 + 序列化耗时 < 全部耗时。但是上表反映的情况基本符合客观事实，多次重跑结果大致相同。
>
> ArduinoJSON和parsonJSON不参与无数字数据测试是因为他们解析长JSON时会发生错误。

#### nativejson-benchmark数据测试

nativejson-benchmark在JSON性能测试方面做了非常大的贡献，因此，使用nativejson-benchmark的数据进行测试非常有意义，测试结果如下：
> 以下所有数据单位为ms。

| 解析器名字   | 解析耗时 | 序列化耗时 | 全部耗时 |
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

以上测试表明，在nativejson-benchmark数据测试中，zzzJSON的速度最快。

#### 淘宝数据测试

使用fastjson提供的淘宝网的真实数据进行测试，能够更好的体现真实的使用情况，测试结果如下：
> 以下所有数据单位为ms。

| 解析器名字   | 解析耗时 | 序列化耗时 | 全部耗时 |
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

以上测试表明，在淘宝数据测试中，zzzJSON的速度最快。

#### 混合数据测试
> 以下所有数据单位为ms。

混合数据测试使用了nativejson-benchmark、淘宝和json-iterator的数据，能够更好的体现真实的使用情况，测试结果如下：

| 解析器名字   | 解析耗时 | 序列化耗时 | 全部耗时 |
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

以上测试表明，在混合数据测试中，zzzJSON的速度最快。

#### 随机短JSON测试
该测试随机生成一万条短JSON用于测试，该测试用于检验批量处理JSON的性能，测试结果如下：
| 解析器名字   | 解析耗时 | 序列化耗时 | 全部耗时 |
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
以上测试结果表明，在批量处理短JSON的场景，zzzJSON的速度最快。

#### 随机长JSON测试
该测试随机生成一百条长JSON用于测试，该测试用于检验批量处理JSON的性能，测试结果如下：
| 解析器名字   | 解析耗时 | 序列化耗时 | 全部耗时 |
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
以上测试结果表明，在批量处理长JSON的场景，zzzJSON的速度最快。

### 总结

测试结果表明，在主流JSON解析库中，zzzJSON拥有最快的解析和序列化速度，在不需要使用JSON中的所有数字的应用场景中，建议使用zzzJSON。

以上数据，可以通过运行以下命令获得（g++版本大于5.4，nlohmannJSON要求编译器版本比较高，Clang请参考《编译运行》）：

```shell
./build.sh all
```

> 在不同环境下，数据会略有不同，但是能够反映客观结果。
>
> 以上表格的元数据在 source.data 中。

## 三、数据结构

zzzJSON所有对外的数据结构都有长命名和短命名，其中长命名一直生效，短命名默认生效，可以通过定义宏使其失效，代码如下：

```c
#define ZZZ_SHORT_API 0
// 务必要定义在 include 语句之前
#include "zzzjson.h"
```

> 宏**ZZZ_SHORT_API**的值决定了所有对外的**数据结构**和**API**的短命名是否生效。
>
> 使用长命名的原因是C语言没有名空间，短命名容易跟其它代码冲突。

### 类型

zzzJSON一共有三种基础类型，分别是：

- 32位无符号整形
- 布尔类型
- JSON类型

下面罗列zzzJSON使用过程中需要的类型，如下：

| 类型           | 长命名       | 短命名 |
| -------------- | ------------ | ------ |
| 32位无符号整形 | zzz_UINT32   | UINT32 |
| 布尔类型       | zzz_BOOL     | BOOL   |
| JSON类型       | zzz_JSONTYPE | TYPE   |

### 常量

为了方便使用，zzzJSON定义了2个布尔类型的常量和6个JSON类型的常量，如下：

| 类型     | 长命名             | 短命名     | 值   |
| -------- | ------------------ | ---------- | ---- |
| 布尔类型 | zzz_True           | True       | 1    |
| 布尔类型 | zzz_False          | False      | 0    |
| JSON类型 | zzz_JSONTypeArray  | TypeArray  | 2    |
| JSON类型 | zzz_JSONTypeObj    | TypeObj    | 3    |
| JSON类型 | zzz_JSONTypeString | TypeString | 4    |
| JSON类型 | zzz_JSONTypeNum    | TypeNum    | 6    |
| JSON类型 | zzz_JSONTypeBool   | TypeBool   | 1    |
| JSON类型 | zzz_JSONTypeNull   | TypeNull   | 5    |

为了在C语言中方便地使用switch语句，zzzJSON针对每个JSON类型定义了相关的宏，如下：

| 长命名     | 短命名             | 值   |
| ---------- | ------------------ | ---- |
| TYPEBOOL   | zzz_JSONTYPEBOOL   | 1    |
| TYPEARRAY  | zzz_JSONTYPEARRAY  | 2    |
| TYPEOBJ    | zzz_JSONTYPEOBJ    | 3    |
| TYPESTRING | zzz_JSONTYPESTRING | 4    |
| TYPENULL   | zzz_JSONTYPENULL   | 5    |
| TYPENUM    | zzz_JSONTYPENUM    | 6    |

### 结构

zzzJSON定义了2个数据结构，分别是**内存分配器**、**值**，如下：

| 类型       | 长命名        | 短命名    | 作用           |
| ---------- | ------------- | --------- | -------------- |
| 内存分配器 | zzz_Allocator | Allocator | 分配和释放内存 |
| 值         | zzz_Value     | Value     | JSON的值       |

## 四、API

zzzJSON所有API都有长命名和短命名，其中长命名一直生效，短命名默认生效，可以通过定义宏使其失效，代码如下：

```c
#define ZZZ_SHORT_API 0
// 务必要定义在 include 语句之前
#include "zzzjson.h"
```

> 宏**ZZZ_SHORT_API**的值决定了所有对外的**数据结构**和**API**的短命名是否生效。
>
> 使用长命名的原因是C语言没有名空间，短命名容易跟其它代码冲突。

zzzJSON的API主要包括四个部分，分别是：

+ 内存分配
+ 解析和序列化 
+ 读操作 
+ 写操作 

涉及拷贝操作的API都有快速版本，使用快速版本能够获得更好的性能。

### 快速上手

在查阅枯燥的API前，先阅读一个简单的例子，该例子里面用到的API能够覆盖日常80%以上的应用场景。该例子实现了值的深复制，具体代码如下：

```c
#include "zzzjson.h"
#include <stdio.h>

// 深复制函数，这里命名为GetAndSet更直观
void GetAndSet(Value *srcv, Value *desv)
{
    // 获取值的类型
    const TYPE *t;
    t = Type(srcv);
    if (t == 0)
        return;
    switch (*t)
    {
    case TYPEARRAY:
    {
        // 如果是数组，则把当前值设为数组，然后遍历并复制数组中的每个值
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
    case TYPEOBJ:
    {
        // 如果是对象，则把当前值设为对象，然后遍历并复制对象中的每个值
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
    case TYPEBOOL:
    {
        // 如果是布尔值，则获取并复制该值
        const zzz_BOOL *b = GetBool(srcv);
        if (b == 0)
            return;
        SetBool(desv, *b);
        break;
    }
    case TYPENULL:
    {
        // 如果是空，则复制该值
        if (IsNull(srcv) == False)
            return;
        SetNull(desv);
        break;
    }

    case TYPESTRING:
    {
        // 如果是字符串，则获取并复制该字符串
        const char *str = GetString(srcv);
        if (str == 0)
            return;
        // 如果需要拷贝字符串，则需要使用SetStr
        if (SetStrFast(desv, str) != True)
            return;
        break;
    }
    case TYPENUM:
    {
        // 如果是数字，则获取并复制该数字
        const char *str = GetNumStr(srcv);
        if (str == 0)
            return;
        // 如果需要拷贝数字，则需要使用SetNumStr
        if (SetNumFast(desv, str) != True)
            return;
        break;
    }
    }
}
int main()
{
    const char *src_json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"],null]";
    // 创建一个内存分配器
    Allocator *A = NewAllocator();
    // 创建两个值
    Value *src_v = NewValue(A);
    Value *des_v = NewValue(A);
    // 解析JSON文本
    BOOL ret = ParseFast(src_v, src_json);
    if (ret != True)
    {
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
```

输出如下：

src_json:[{"key":true},false,{"key1":true},[null,false,[],true],["",123,"str"],null]
des_json:[{"key":true},false,{"key1":true},[null,false,[],true],["",123,"str"],null]


### 内存分配

内存分别相关的API主要有3个，创建内存分配器、释放内存分配器和创建值，其中，创建的值不需要释放，由内存分配器统一释放，API如下：

#### NewAllocator 

```c
// 短命名
Allocator *NewAllocator();
// 长命名
struct zzz_Allocator *zzz_AllocatorNew();
```

作用：

创建一个内存分配器。

参数：

+ 无

返回：

+ 内存分配器的地址

注意事项：

+ 创建内存分配器之后需要释放，不然会造成内存泄漏。**内存分配器必须通过API创建**，不要试图直接创建。

错误代码示例：

```c
#include "zzzjson.h"
int main() {
    Allocator A;
    Value *v = NewValue(&A);
    ReleaseAllocator(&A);
}
```

以上代码会导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    ReleaseAllocator(A);
}
```

#### ReleaseAllocator 

```c
// 短命名
void ReleaseAllocator(Allocator *root_alloc);
// 长命名
void zzz_AllocatorRelease(struct zzz_Allocator *root_alloc);
```

作用：

释放一个内存分配器。

参数：

+ root_alloc：内存分配器的地址

返回：

+ 无

注意事项：

+ 创建内存分配器之后需要释放，不然会造成内存泄漏。

错误代码示例：

```c
#include "zzzjson.h"
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
}
```

以上代码会导致内存泄漏。

正确代码示例：

```c
#include "zzzjson.h"
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    ReleaseAllocator(A);
}
```

####NewValue 

```c
// 短命名
Value *NewValue(Allocator *alloc);
// 长命名
struct zzz_Value *zzz_ValueNew(struct zzz_Allocator *alloc);
```

作用：

创建一个值。

参数：

+ alloc：内存分配器的地址

返回：

+ 值的地址

注意事项：

+ **值必须通过API创建**，否则会导致不可预测的结果，值创建之后不需要释放，由内存分配器统一释放。

错误代码示例：

```c
#include "zzzjson.h"
int main() {
    Value v;
    SetNumStr(&v, "123");
}
```

以上代码会导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    ReleaseAllocator(A);
}
```

### 解析与序列化

解析与序列化提供了4个API：Parse、ParseLen、ParseFast和Stringify。ParseFast为Parse的快速版本，无内存拷贝，ParseLen可以指定字符串的长度。

#### Parse

```c
// 短命名
BOOL Parse(Value *v, const char *s);
// 长命名
zzz_BOOL zzz_ValueParse(struct zzz_Value *v, const char *s);
```

作用：

把JSON文本解析成值。

参数：

- v：值的地址
- s：JSON文本的地址，必须以0结尾

返回：

- True：成功
- False：失败

注意事项：

+ JSON文本的编码类型必须为UTF8，否则会出现不可预知的结果。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char * json = "[123]";
    BOOL ret = Parse(v, json);
    if (ret == True) {
        const char *str = Stringify(v);
        if (str != 0) printf("%s\n", str);
    } 
    ReleaseAllocator(A);
}
```

输出如下：

[123]

#### ParseLen 

```c
// 短命名
BOOL ParseLen(Value *v, const char *s, UINT32 len);
// 长命名
zzz_BOOL zzz_ValueParseLen(struct zzz_Value *v, const char *s, zzz_UINT32 len);
```

作用：

跟Parse一样，但是可以指定字符串的长度，免去字符串长度的计算。

参数：

+ v：值的地址
+ s：JSON文本的地址
+ len：JSON文本的长度

返回：

+ True：成功
+ False：失败

注意事项：

+ JSON文本的编码类型必须为UTF8，否则会出现不可预知的结果。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char * json = "[123]";
    BOOL ret = ParseLen(v, json, strlen(json));
    if (ret == True) {
        const char *str = Stringify(v);
        if (str != 0) printf("%s\n", str);
    } 
    ReleaseAllocator(A);
}
```

输出如下：

[123]

#### ParseFast

```c
// 短命名
BOOL ParseFast(Value *v, const char *s);
// 长命名
zzz_BOOL zzz_ValueParseFast(struct zzz_Value *v, const char *s);
```

作用：

Parse的快速版本，不对JSON文本进行拷贝。

参数：

- v：值的地址
- s：JSON文本的地址，必须以0结尾

返回：

- True：成功
- False：失败

注意事项：

+ JSON文本的编码类型必须为UTF8，JSON文本在zzzJSON的生命周期内不能更改，否则会出现不可预知的结果。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char * json = "[123]";
    BOOL ret = ParseFast(v, json);
    if (ret == True) {
        const char *str = Stringify(v);
        if (str != 0) printf("%s\n", str);
    } 
    ReleaseAllocator(A);
}
```

输出如下：

[123]

#### Stringify

```c
// 短命名
const char *Stringify(const Value *v);
// 长命名
const char *zzz_ValueStringify(const struct zzz_Value *v);
```

作用：

把值序列化成JSON文本。

参数：

- v：值的地址

返回：

- JSON文本的地址，如果为0，则表示序列化失败

注意事项：

+ 需要判断返回值是不是0，防止导致不可预测的结果；
+ 每次调用该API都会创建一个字符串，这些字符串在内存分配器释放时才会被释放，因此，不建议对同一个值重复调用。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main() {
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char * json = "[123]";
    BOOL ret = ParseFast(v, json);
    if (ret == True) {
        const char *str = Stringify(v);
        if (str != 0) printf("%s\n", str);
    } 
    ReleaseAllocator(A);
}
```

输出如下：

[123]

### 读操作

读操作不仅包含了获取值数据的各种操作，而且提供了拷贝操作。

#### GetString

```c
// 短命名
const char *GetString(Value *v);
// 长命名
const char *zzz_ValueGetString(struct zzz_Value *v);
```

作用：

获取JSON类型为字符串的值的字符串。

参数：

- v：值的地址

返回：

- 字符串的地址，如果为0，则表示获取失败

注意事项：

+ 需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[\"str\",123]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 0);
        if (vv != 0)
        {
            const char *str = GetString(vv);
            if (str != 0)
                printf("%s\n", str);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

str

#### GetStringFast

```c
// 短命名
const char *GetStringFast(const Value *v, UINT32 *len);
// 长命名
const char *zzz_ValueGetStringFast(const struct zzz_Value *v, zzz_UINT32 *len);
```

作用：

GetString的快速版本，不对原字符串进行拷贝。

参数：

- v：值的地址
- len：字符串的长度

返回：

- 字符串的地址，如果为0，则表示获取失败

注意事项：

+ 返回的字符串不能修改，需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[\"str\",123]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 0);
        if (vv != 0)
        {
            UINT32 len = 0;
            const char *str = GetStringFast(vv, &len);
            if (str != 0){
                int i = 0;
                for (i = 0; i < len; ++i) printf("%c", str[i]);
                printf("\n");
            }
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

str

#### GetUnEscapeString

```c
// 短命名
const char *GetUnEscapeString(Value *v);
// 长命名
const char *zzz_ValueGetUnEscapeString(struct zzz_Value *v);
```

作用：

获得解码过的字符串

参数：

- v：值的地址

返回：

- 字符串的地址，如果为0，则表示获取失败

注意事项：

+ 原JSON字符串中必须含有\uXXXX才有意义，否则只会拖慢速度；

- 返回的字符串不能修改，需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"

int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    // \uD87E\uDD21 == 爵 这里请参考 UTF16 的文档规则
    const char *json = "\"\\u007a\\u007a\\u007a\\u004A\\u0053\\u004F\\u004E\\u597D\\u68D2\\uD87E\\uDD21\"";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        const char *str = GetUnEscapeString(v);
        if (str != 0) printf("%s\n", str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

zzzJSON好棒爵

#### GetNumStr 

```c
// 短命名
const char *GetNumStr(Value *v);
// 长命名
const char *zzz_ValueGetNumStr(struct zzz_Value *v);
```

作用：

获取JSON类型为数字的值的数字的字符串表示。

参数：

- v：值的地址

返回：

- 字符串的地址，如果为0，则表示获取失败

注意事项：

+ 返回的字符串不能修改，需要判断返回值是不是0，防止导致不可预测的结果；
+ 建议根据实际精度要求，把字符串转化为相关的数字类型，提高性能。

正确代码示例：

```c
#include "zzzjson.h"
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[\"str\",123.4567899883784758435486768576854]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 1);
        if (vv != 0)
        {
            const char *num = GetNumStr(vv);
            if (num != 0){
                double d = atof(num);
                printf("%.17g\n", d);
            }
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

123.45678998837847

#### GetNumFast

```c
// 短命名
const char *GetNumFast(const Value *v, zzz_UINT32 *len);
// 长命名
const char *zzz_ValueGetNumFast(const struct zzz_Value *v, zzz_UINT32 *len);
```

作用：

GetNumStr的快速版本，不对原字符串进行拷贝。

参数：

- v：值的地址
- len：字符串的长度

返回：

- 字符串的地址，如果为0，则表示获取失败

注意事项：

+ 返回的字符串不能修改，需要判断返回值是不是0，防止导致不可预测的结果；
+ 建议根据实际精度要求，把字符串转化为相关的数字类型，提高性能；
+ 返回结果可以使用**atoi**等函数进行转换，**atoi**遇到非数字字符就会结束，JSON中，数字后面只可能是**,]}**和**字符串结束符号(0)**，因此，Fast版本，可以直接使用**atoi**。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
#include <stdlib.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[\"str\",123.4567899883784758435486768576854]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 1);
        if (vv != 0)
        {
            UINT32 len = 0;
            const char *num = GetNumFast(vv, &len);
            if (num != 0){
                double d = atof(num);
                printf("%.17g\n", d);
            }
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

123.45678998837847

#### GetNum

```c
// 短命名
const double *GetNum(Value *v);
// 长命名
const double * zzz_ValueGetNum(struct zzz_Value *v);
```

作用：

GetNumStr的简单版本，性能不佳。

参数：

- v：值的地址

返回：

- 浮点型的地址，如果为0，则表示获取失败

注意事项：

- 需要判断返回值是不是0，防止导致不可预测的结果；
- 建议使用GetNumFast。

正确代码示例：

```c
#include "zzzjson.h"
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[\"str\",123.4567899883784758435486768576854]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 1);
        if (vv != 0)
        {
            const double *d = GetNum(vv);
            if (d != 0){
                printf("%.17g\n", *d);
            }
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

123.45678998837847

#### GetBool 

```c
// 短命名
const BOOL * GetBool(const Value *v);
// 长命名
const zzz_BOOL *zzz_ValueGetBool(const struct zzz_Value *v);
```

作用：

获取JSON类型为布尔类型的值的布尔值。

参数：

- v：值的地址

返回：

- 布尔值的地址，如果为0，则表示获取失败

注意事项：

- 返回的布尔类型不能修改，需要判断返回值是不是0，防止导致不可预测的结果；

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[\"str\",123,true]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 2);
        if (vv != 0)
        {
            const BOOL *b = GetBool(vv);
            if (b != 0) printf("%d\n", *b);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

1

#### IsNull 

```c
// 短命名
BOOL IsNull(const Value *v);
// 长命名
zzz_BOOL zzz_ValueIsNull(const struct zzz_Value *v);
```

作用：

判断JSON类型是否为空类型。

参数：

- v：值的地址

返回：

- 是则返回True，否则返回False

注意事项：

+ 无

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[\"str\",123,true,null]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 3);
        if (vv != 0)
        {
            printf("%d\n", IsNull(vv));
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

1

#### GetKey

```c
// 短命名
const char *GetKey(const Value *v);
// 长命名
const char *zzz_ValueGetKey(const struct zzz_Value *v);
```

作用：

获取JSON类型的关键字。

参数：

- v：值的地址

返回：

- 关键字的地址

注意事项：

- 返回的关键字不能修改，需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
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
        Value *vv = Begin(v);
        if (vv != 0)
        {
            const char *key = GetKey(vv);
            if (key != 0) printf("%s\n", key);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

key123

#### GetKeyFast

```c
// 短命名
const char *GetKeyFast(const Value *v, UINT32 *len);
// 长命名
const char *zzz_ValueGetKeyFast(const struct zzz_Value *v, zzz_UINT32 *len);
```

作用：

GetKey的快速版本，不对原字符串进行拷贝。

参数：

- v：值的地址
- len：关键字的长度

返回：

- 关键字的地址

注意事项：

- 返回的关键字不能修改，需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
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
        Value *vv = Begin(v);
        if (vv != 0)
        {
            UINT32 len;
            UINT32 i;
            const char *key = GetKeyFast(vv, &len);
            if (key != 0) for(i = 0; i < len; ++i) printf("%c", key[i]);
            printf("\n");
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

key123

#### GetUnEscapeKey

```c
// 短命名
const char *GetUnEscapeKey(Value *v);
// 长命名
const char *zzz_ValueGetUnEscapeKey(struct zzz_Value *v);
```

作用：

获得解码之后的Key。

参数：

- v：值的地址

返回：

- 关键字的地址

注意事项：

- 返回的关键字不能修改，需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"

int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "{\"key123\":123}";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = Begin(v);
        if (vv != 0)
        {
            UINT32 len;
            UINT32 i;
            const char *key = GetUnEscapeKey(vv);
            printf("%s\n", key);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

key123

####ObjGet 

```c
// 短命名
Value *ObjGet(const Value *v, const char *key);
// 长命名
struct zzz_Value *zzz_ValueObjGet(const struct zzz_Value *v, const char *key);
```

作用：

获取类型为对象的值中关键字为key的值。

参数：

- v：值的地址
- key：关键字，必须以0结尾

返回：

- 值的地址

注意事项：

- 需要判断返回值是不是0，防止导致不可预测的结果；
- 修改返回值会影响参数v。

正确代码示例：

```c
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
        if (vv != 0)
        {
            SetNumStr(vv, "1234");
            const char *ret_json = Stringify(v);
            printf("%s\n", ret_json);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

{"key123":1234}

#### ArrayGet

```c
// 短命名
Value *ArrayGet(const Value *v, UINT32 index);
// 长命名
struct zzz_Value *zzz_ValueArrayGet(const struct zzz_Value *v, zzz_UINT32 index);
```

作用：

获取类型为数组的值的第index个值。

参数：

- v：值的地址
- index：下标

返回：

- 值的地址

注意事项：

- index从0开始；
- 需要判断返回值是不是0，防止导致不可预测的结果；
- 修改返回值会影响参数v。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 1);
        if (vv != 0)
        {
            SetNumStr(vv, "1234");
            const char *ret_json = Stringify(v);
            printf("%s\n", ret_json);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

[123,1234,789]

####Begin

```c
// 短命名
Value *Begin(const Value *v);
// 长命名
struct zzz_Value *zzz_ValueBegin(const struct zzz_Value *v);
```

作用：

获取类型为数组或者对象的值的第一个值，主要用于遍历。

参数：

- v：值的地址

返回：

- 值的地址

注意事项：

- 需要判断返回值是不是0，防止导致不可预测的结果；
- 修改返回值会影响参数v。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = Begin(v);
        if (vv != 0)
        {
            SetNumStr(vv, "999");
            const char *ret_json = Stringify(v);
            printf("%s\n", ret_json);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

[999,456,789]

####Next

```c
// 短命名
Value *Next(const Value *v);
// 长命名
struct zzz_Value *zzz_ValueNext(const struct zzz_Value *v);
```

作用：

获取类型为数组或者对象的值中的值的下一个值，主要用于遍历。

参数：

- v：值的地址

返回：

- 值的地址

注意事项：

- 需要判断返回值是不是0，防止导致不可预测的结果；
- 修改返回值会影响参数v。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = 0;
        for (vv = Begin(v); vv != 0; vv = Next(vv))
        {
            const char *num = GetNum(vv);
            if (num != 0)
                printf("%s ", num);
        }
    }
    printf("\n");
    ReleaseAllocator(A);
}
```

输出如下：

123 456 789

#### Type

```c
// 短命名
const TYPE * Type(const Value *v);
// 长命名
const zzz_JSONTYPE *zzz_ValueType(const struct zzz_Value *v);
```

作用：

获取值的类型。

参数：

- v：值的地址

返回：

- 值的类型的地址

注意事项：

- 返回的JSON类型不能修改，需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
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
        if (vv != 0)
        {
            const TYPE *t = Type(vv);
            if (t != 0) printf("%d\n", *t);
        }
    }
    ReleaseAllocator(A);
}
```

输出如下：

6

#### Size

```c
// 短命名
UINT32 Size(const Value *v) ;
// 长命名
zzz_UINT32 zzz_ValueSize(const struct zzz_Value *v);
```

作用：

获取值包含值的个数。

参数：

- v：值的地址

返回：

- 值包含值的个数

注意事项：

- 非数组或对象的值返回0。

正确代码示例：

```c
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
        printf("%u\n", Size(v));
    }
    ReleaseAllocator(A);
}
```

输出如下：

1

####Copy

```c
// 短命名
Value *Copy(const Value *v);
// 长命名
struct zzz_Value *zzz_ValueCopy(const struct zzz_Value *v);
```

作用：

拷贝一个值。

参数：

- v：值的地址

返回：

- 值的地址

注意事项：

- 需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = Copy(v);
        const char *vv_json = Stringify(vv);
        if (vv_json != 0) printf("%s\n", vv_json);
        Value *vvv = NewValue(A);
        SetNumStr(vvv, "12345");
        ArrayAdd(vv, vvv);
        vv_json = Stringify(vv);
        if (vv_json != 0) printf("%s\n", vv_json);
        const char *v_json = Stringify(v);
        if (v_json != 0) printf("%s\n", v_json);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[123,456,789]
[123,456,789,12345]
[123,456,789]

###写操作

写操作不仅包含了设置值数据的各种操作，而且提供了剪切操作。

####SetNull

```c
// 短命名
void SetNull(Value *v);
// 长命名
void zzz_ValueSetNull(struct zzz_Value *v);
```

作用：

设置值的类型为空。

参数：

- v：值的地址

返回：

- 无

注意事项：

- 无。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        SetNull(v);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

null

#### SetBool

```c
// 短命名
void SetBool(Value *v, BOOL b);
// 长命名
void zzz_ValueSetBool(struct zzz_Value *v, zzz_BOOL b);
```

作用：

设置值的类型为布尔类型，并且赋值。

参数：

- v：值的地址
- b：值的值

返回：

- 无

注意事项：

- 无。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        SetBool(v, False);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

false

####SetNumStr

```c
// 短命名
BOOL SetNumStr(Value *v, const char *num);
// 长命名
zzz_BOOL zzz_ValueSetNumStr(struct zzz_Value *v, const char *num);
```

作用：

设置值的类型为数字类型，并且赋值。

参数：

- v：值的地址
- num：值的值

返回：

- True：设置成功
- False：设置失败

注意事项：

- **num**必须为0结尾的字符串；
- 建议根据实际精度需要使用sprintf把数字变成字符串，为了防止越界，建议使用g而非f。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    // 这个数组不要太短，防止sprintf越界
    char buff[32];
    double d = -12345678912345561234568890.0;
    // double 最大有效数字为17，可以根据实际需求来。
    // 建议使用g，而不要使用f。
    sprintf(buff, "%.17g", d);
    SetNumStr(v, buff);
    const char *v_str = Stringify(v);
    if (v_str != 0)
        printf("%s\n", v_str);
    ReleaseAllocator(A);
}
```

输出如下：

-1.2345678912345561e+25

####SetNumFast

```c
// 短命名
BOOL SetNumFast(Value *v, const char *num);
// 长命名
zzz_BOOL zzz_ValueSetNumFast(struct zzz_Value *v, const char *num);
```

作用：

SetNumStr的快速版本，不对**num**进行拷贝。

参数：

- v：值的地址
- num：值的值

返回：

- True：设置成功
- False：设置失败

注意事项：

- **num**必须为0结尾的字符串；
- 在zzzJSON的生命周期内不能对num进行修改，否则会导致不可预测的结果;
- 建议根据实际精度需要使用sprintf把数字变成字符串，为了防止越界，建议使用g而非f。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    // 这个数组不要太短，防止sprintf越界
    char buff[32];
    double d = -12345678912345561234568890.0;
    // double 最大有效数字为17，可以根据实际需求来。
    // 建议使用g，而不要使用f。
    sprintf(buff, "%.17g", d);
    SetNumFast(v, buff);
    const char *v_str = Stringify(v);
    if (v_str != 0)
        printf("%s\n", v_str);
    ReleaseAllocator(A);
}
```

输出如下：

-1.2345678912345561e+25

#### SetNum

```c
// 短命名
BOOL SetNum(Value *v, const double d);
// 长命名
zzz_BOOL zzz_ValueSetNum(struct zzz_Value *v, const double d);
```

作用：

SetNumStr的简单版本，性能不佳。

参数：

- v：值的地址
- d：值的值

返回：

- True：设置成功
- False：设置失败

注意事项：

- 建议使用SetNumFast。

正确代码示例：

```c
#include "zzzjson.h"
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    double d = -12345678912345561234568890.0;
    SetNum(v, d);
    const char *v_str = Stringify(v);
    if (v_str != 0)
        printf("%s\n", v_str);
    ReleaseAllocator(A);
}
```

输出如下：

-1.2345678912345561e+25

#### SetStr 

```c
// 短命名
BOOL SetStr(Value *v, const char *str);
// 长命名
zzz_BOOL zzz_ValueSetStr(struct zzz_Value *v, const char *str);
```

作用：

设置值的类型为字符串类型，并且赋值。

参数：

- v：值的地址
- str：值的值

返回：

- True：设置成功
- False：设置失败

注意事项：

- str必须为0结尾的字符串。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        SetStr(v, "123str");
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

"123str"

#### SetStrFast

```c
// 短命名
BOOL SetStrFast(Value *v, const char *str);
// 长命名
zzz_BOOL zzz_ValueSetStrFast(struct zzz_Value *v, const char *str);
```

作用：

SetStr的快速版本，不对**str**进行拷贝。

参数：

- v：值的地址
- str：值的值

返回：

- True：设置成功
- False：设置失败

注意事项：

- str必须为0结尾的字符串。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        SetStrFast(v, "123str");
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

"123str"

#### SetKey 

```c
// 短命名
BOOL SetKey(Value *v, const char *key);
// 长命名
zzz_BOOL zzz_ValueSetKey(struct zzz_Value *v, const char *key);
```

作用：

设置值的关键字。

参数：

- v：值的地址
- key：关键字

返回：

- True：设置成功
- False：设置失败

注意事项：

- **key**必须为0结尾的字符串。

正确代码示例：

```c
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
        SetKey(vv, "key234");
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

{"key234":123}

#### SetKeyFast

```c
// 短命名
BOOL SetKeyFast(Value *v, const char *key);
// 长命名
zzz_BOOL zzz_ValueSetKeyFast(struct zzz_Value *v, const char *key);
```

作用：

SetKey的快速版本，不对**key**进行拷贝。

参数：

- v：值的地址
- key：值的值

返回：

- True：设置成功
- False：设置失败

注意事项：

- key必须为0结尾的字符串。

正确代码示例：

```c
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
        SetKeyFast(vv, "key234");
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

"123str"

#### SetArray 

```c
// 短命名
void SetArray(Value *v);
// 长命名
void zzz_ValueSetArray(struct zzz_Value *v);
```

作用：

把值的类型设置为数组。

参数：

- v：值的地址

返回：

- 无

注意事项：

- 使用该API之后，值将会变成一个空数组，不再保留原来的数据。

正确代码示例：

```c
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
        SetArray(v);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[]

#### SetObj 

```c
// 短命名
void SetObj(Value *v);
// 长命名
void zzz_ValueSetObj(struct zzz_Value *v);
```

作用：

把值的类型设置为对象。

参数：

- v：值的地址

返回：

- 无

注意事项：

- 使用该API之后，值将会变成一个空对象，不再保留原来的数据。

正确代码示例：

```c
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
        SetObj(v);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

{}

#### Set

```c
// 短命名
BOOL Set(Value *v, const Value *vv);
// 长命名
zzz_BOOL zzz_ValueSet(struct zzz_Value *v, const struct zzz_Value *vv);
```

作用：

替换一个值。

参数：

- v：值的地址
- vv：值的地址

返回：

- True：设置成功
- False：设置失败

注意事项：

- 无。

正确代码示例：

```c
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
        Value *vv = NewValue(A);
        SetArray(vv);
        Set(v, vv);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[]

####SetFast

```c
// 短命名
BOOL SetFast(Value *v, Value *vv);
// 长命名
zzz_BOOL zzz_ValueSetFast(struct zzz_Value *v, struct zzz_Value *vv);
```

作用：

Set的快速版本，没有拷贝。

参数：

- v：值的地址
- vv：值的地址

返回：

- True：设置成功
- False：设置失败

注意事项：

- vv会被从原来的JSON中剪切出来。

正确代码示例：

```c
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
        Value *vv = NewValue(A);
        SetArray(vv);
        SetFast(v, vv);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[]

####ObjAdd

```c
// 短命名
BOOL ObjAdd(Value *v, const Value *vv);
// 长命名
zzz_BOOL zzz_ValueObjAddFast(struct zzz_Value *v, struct zzz_Value *vv);
```

作用：

添加一个值到对象类型的值中。

参数：

- v：值的地址
- vv：值的地址

返回：

- True：添加成功
- False：添加失败

注意事项：

- 值vv必须有关键字。

正确代码示例：

```c
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
        Value *vv = NewValue(A);
        SetKey(vv, "key234");
        SetNumStr(vv, "234");
        ObjAdd(v, vv);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

{"key123":123,"key234":234}

#### ObjAddFast

```c
// 短命名
BOOL ObjAddFast(Value *v, Value *vv);
// 长命名
zzz_BOOL zzz_ValueObjAddFast(struct zzz_Value *v, struct zzz_Value *vv);
```

作用：

ObjAdd的快速版本，没有拷贝。

参数：

- v：值的地址
- vv：值的地址

返回：

- True：添加成功
- False：添加失败

注意事项：

- 值vv必须有关键字；
- vv会被从原来的JSON中剪切出来。

正确代码示例：

```c
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
        Value *vv = NewValue(A);
        SetKey(vv, "key234");
        SetNumStr(vv, "234");
        ObjAddFast(v, vv);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

{"key123":123,"key234":234}

####ArrayAdd

```c
// 短命名
BOOL ArrayAdd(Value *v, const Value *vv);
// 长命名
zzz_BOOL zzz_ValueArrayAdd(struct zzz_Value *v, const struct zzz_Value *vv);
```

作用：

添加一个值到数组类型的值中。

参数：

- v：值的地址
- vv：值的地址

返回：

- True：添加成功
- False：添加失败

注意事项：

- 无。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[1,2,3]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = NewValue(A);
        SetNumStr(vv, "4");
        ArrayAdd(v, vv);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[1,2,3,4]

#### ArrayAddFast

```c
// 短命名
BOOL ArrayAddFast(Value *v, Value *vv);
// 长命名
zzz_BOOL zzz_ValueArrayAddFast(struct zzz_Value *v, struct zzz_Value *vv);
```

作用：

ArrayAddFast的快速版本，没有拷贝。

参数：

- v：值的地址
- vv：值的地址

返回：

- True：添加成功
- False：添加失败

注意事项：

- vv的关键字会被清空；
- vv会被从原来的JSON中剪切出来。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[1,2,3]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = NewValue(A);
        SetNumStr(vv, "4");
        ArrayAddFast(v, vv);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[1,2,3,4]

####ArrayDel

```c
// 短命名
BOOL ArrayDel(Value *v, UINT32 index);
// 长命名
zzz_BOOL zzz_ValueArrayDel(struct zzz_Value *v, zzz_UINT32 index);
```

作用：

删除类型为数组的值的第index个值。

参数：

- v：值的地址
- index：下标

返回：

- True：删除成功
- False：删除失败

注意事项：

- index从0开始算。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[1,2,3]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        ArrayDel(v, 1);
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[1,3]

####ObjDel 

```c
// 短命名
BOOL ObjDel(Value *v, const char *key);
// 长命名
zzz_BOOL zzz_ValueObjDel(struct zzz_Value *v, const char *key);
```

作用：

删除类型为对象的值中关键字为key的值。

参数：

- v：值的地址
- key：关键字，必须以0结尾

返回：

- True：删除成功
- False：删除失败

注意事项：

- 无。

正确代码示例：

```c
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
        ObjDel(v, "key123");
        const char *v_str = Stringify(v);
        if (v_str != 0) printf("%s\n", v_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

{}

#### Move

```c
// 短命名
BOOL Move(Value *v);
// 长命名
zzz_BOOL zzz_ValueMove(struct zzz_Value *v);
```

作用：

剪切一个值。

参数：

- v：值的地址

返回：

- True：成功
- False：失败

注意事项：

- 需要判断返回值是不是0，防止导致不可预测的结果。

正确代码示例：

```c
#include "zzzjson.h"
#include <stdio.h>
int main()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "[123,456,789]";
    BOOL ret = ParseFast(v, json);
    if (ret == True)
    {
        Value *vv = ArrayGet(v, 1);
        Move(vv);
        const char *v_str = Stringify(v);
        const char *vv_str = Stringify(vv);
        if (v_str != 0) printf("%s\n", v_str);
        if (vv_str != 0) printf("%s\n", vv_str);
    }
    ReleaseAllocator(A);
}
```

输出如下：

[123,789]
456

## 五、内存配置

zzzJSON实现了统一的内存分配器，因此，可以根据实际需要，调整内存分配的参数，以获得更好的性能。

###ZZZ_DELTA

当内存不够时，向操作系统申请上次申请的ZZZ_DELTA倍的内存，必须大于1。默认值为：2，使用方法如下：

```c
#define ZZZ_DELTA 3
#include "zzzjson.h"
```

### ZZZ_ALLOCATORINITMEMSIZE

内存分配器首次分配内存的大小。默认值为：4096Bytes，使用方法如下：

```c
#define ZZZ_ALLOCATORINITMEMSIZE 1024
#include "zzzjson.h"
```

### ZZZ_STRINGINITMEMSIZE

字符串首次分配的大小，用于序列化生成的JSON文本，略大于序列化生成的JSON文本为最佳。默认值为：1024，使用方法如下：

```c
#define ZZZ_STRINGINITMEMSIZE 2048
#include "zzzjson.h"
```

##六、环境配置

zzzJSON默认情况下适应绝大部分环境，对于非常特殊的环境，需要进行一些适配，详细情况如下：

### ZZZ_MEMORY_MODE

分配内存的模型，主要涉及内存分配，内存释放和内存拷贝，默认使用glibc提供malloc、free和memcpy函数。另外还提供调试模式。

调试模式：只要用于跟踪内存的使用和释放，用于调试。

也可以根据使用环境，做一定的调整，代码如下：

```c
// 默认模式
#define ZZZ_MEMORY_MODE 1

// 调试模式
// #define ZZZ_MEMORY_MODE 2

// 自定义模式，需要自己修改代码
// #define ZZZ_MEMORY_MODE 3

#include "zzzjson.h"
```

### ZZZ_EXPECT_MODE

分支预测能大幅提高性能，gcc和clang都支持显示的分支预测，但是mscv不支持。目前这三种C语言的编译器使用最广泛，因此，只对这三种编译器做适配，其它编译器需要自行修改适配，代码如下：

```c
// 如果编译器是gcc或者clang，默认为显示分支预测
#define ZZZ_EXPECT_MODE 1 

// 如果是其它编译器，则默认不使用显示分支预测
// #define ZZZ_EXPECT_MODE 2

// 如果需要自定义分支预测，则
// #define ZZZ_EXPECT_MODE 3

#include "zzzjson.h"
```
## 七、编译运行

需要G++-5.4.0或者以上，低版本G++编译器不保证能成功编译 nlohmannjson ，而使用zzzJSON则对编译器没有要求。

在zzzJSON目录下运行以下命令：

```shell
./build.sh all
```

使用clang编译请修改build.sh中的CC，具体如下：

```shell
#CC=g++
CC=clang++
```



所有C/C++的JSON解析器均为2018/7/25日更新的代码，由于有些JSON代码库非常巨大，为了减少拉取时间，只保留了代码，要更新请自行更新。JSON解析器详细信息如下：

| 解析器名      | 地址                                     |
| ------------- | ---------------------------------------- |
| ArduinoJson   | https://github.com/bblanchon/ArduinoJson |
| cJSON         | https://github.com/DaveGamble/cJSON      |
| gason         | https://github.com/vivkin/gason          |
| nlohmann/json | https://github.com/nlohmann/json         |
| parson        | https://github.com/kgabis/parson         |
| picojson      | https://github.com/kazuho/picojson/      |
| rapidjson     | https://github.com/Tencent/rapidjson     |
| taocpp/json   | https://github.com/taocpp/json           |

## 八、计划与进度

- [x] C语言API
- [x] 功能测试
- [x] 正确性测试
- [x] 性能测试
- [x] 中文文档
- [ ] 图标
- [ ] 开源
- [ ] 英文文档
- [ ] C++封装
- [ ] Go封装
- [ ] Rust封装
- [ ] Go/Rust性能测试
- [ ] Python封装
- [ ] Java封装
- [ ] Python/Java性能测试