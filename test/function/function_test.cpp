#include <string>
#include <random>
#include "../test.h"
#define zzz_MEMORY_MODE 2
#include "../../zzzjson.h"

using namespace std;

void MemoryTest()
{
    Allocator *A = NewAllocator();
    for (SIZE i = 1; i <= zzz_AllocatorInitMemSize; ++i)
    {
        zzz_AllocatorAlloc(A, i);
    }
    ReleaseAllocator(A);
    cout << "Size:" << AllocMemorySize << " AllocCount:" << AllocMemoryCount << " FreeCount:" << FreeMemoryCount << endl;
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void StringTest()
{
    Allocator *A = NewAllocator();
    const char *str = "1234567890abcdefghijklmnopqrstuvwxyz`~!@#$%^&*()_+-=|\\}{:[];'?><,./\"";
    for (SIZE i = 0; i < sizeof(str); ++i)
    {
        zzz_String *zs = zzz_StringNew(A, zzz_StringInitMemSize);
        zzz_StringAppendStr(zs, str, i);
        CheckRet(__func__, __LINE__, string(zzz_StringStr(zs), zs->Pos) == string(str, i));
        zzz_StringAppendEnd(zs);
        CheckRet(__func__, __LINE__, *(zzz_StringStr(zs) + i) == 0);
    }
    for (SIZE i = sizeof(str) - 1;; --i)
    {
        zzz_String *zs = zzz_StringNew(A, zzz_StringInitMemSize);
        for (SIZE j = 0; j < i; ++j)
        {
            zzz_StringAppendChar(zs, *(str + j));
        }
        CheckRet(__func__, __LINE__, string(zzz_StringStr(zs), zs->Pos) == string(str, i));
        zzz_StringAppendEnd(zs);
        CheckRet(__func__, __LINE__, *(zzz_StringStr(zs) + i) == 0);
        if (i == 0)
            break;
    }
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SkinTest()
{
    const char s[] = "abc\t\n\r d";
    SIZE index = 0;
    SIZE skinSum = 0;
    for (SIZE i = 0; i < sizeof(s); ++i)
    {
        if (zzz_Skin(s[i]))
            ++skinSum;
    }
    CheckRet(__func__, __LINE__, skinSum == 4);
}
void PeekTest()
{
    const char s[] = "abc\t\n\r d";
    SIZE index = 0;

    CheckRet(__func__, __LINE__, zzz_Peek(s, &index) == 'a');
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_Peek(s, &index) == 'b');
    CheckRet(__func__, __LINE__, index == 2);

    CheckRet(__func__, __LINE__, zzz_Peek(s, &index) == 'c');
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_Peek(s, &index) == 'd');
    CheckRet(__func__, __LINE__, index == 8);
}
void ConsumeTest()
{
    const char s[] = "abc\t\n\r d";
    SIZE index = 0;

    CheckRet(__func__, __LINE__, zzz_Consume('a', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_Consume('a', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_Consume('b', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 2);

    CheckRet(__func__, __LINE__, zzz_Consume('c', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_Consume('c', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_Consume('d', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 3);

    index = 0;

    CheckRet(__func__, __LINE__, zzz_LikelyConsume('a', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_LikelyConsume('a', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_LikelyConsume('b', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 2);

    CheckRet(__func__, __LINE__, zzz_LikelyConsume('c', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_LikelyConsume('c', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_LikelyConsume('d', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 3);

    index = 0;

    CheckRet(__func__, __LINE__, zzz_UnLikelyConsume('a', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_UnLikelyConsume('a', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_UnLikelyConsume('b', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 2);

    CheckRet(__func__, __LINE__, zzz_UnLikelyConsume('c', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_UnLikelyConsume('c', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_UnLikelyConsume('d', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 3);
}
void PeekAndConsumeTest()
{
    const char s[] = "abc\t\n\r d";
    SIZE index = 0;

    CheckRet(__func__, __LINE__, zzz_LikelyPeekAndConsume('a', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_LikelyPeekAndConsume('a', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_LikelyPeekAndConsume('b', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 2);

    CheckRet(__func__, __LINE__, zzz_LikelyPeekAndConsume('c', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_LikelyPeekAndConsume('c', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 7);

    CheckRet(__func__, __LINE__, zzz_LikelyPeekAndConsume('d', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 8);

    index = 0;

    CheckRet(__func__, __LINE__, zzz_UnLikelyPeekAndConsume('a', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_UnLikelyPeekAndConsume('a', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 1);

    CheckRet(__func__, __LINE__, zzz_UnLikelyPeekAndConsume('b', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 2);

    CheckRet(__func__, __LINE__, zzz_UnLikelyPeekAndConsume('c', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 3);

    CheckRet(__func__, __LINE__, zzz_UnLikelyPeekAndConsume('c', s, &index) == zzz_False);
    CheckRet(__func__, __LINE__, index == 7);

    CheckRet(__func__, __LINE__, zzz_UnLikelyPeekAndConsume('d', s, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 8);
}
void ConsumeTrueFalseNullTest()
{
    SIZE index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeTrue(zzz_StrTrue, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 4);
    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeFalse(zzz_StrFalse, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 5);
    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNull(zzz_StrNull, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 4);
    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeTrue(zzz_StrNull, &index) == zzz_False);
    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeFalse(zzz_StrTrue, &index) == zzz_False);
    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNull(zzz_StrFalse, &index) == zzz_False);
}
void HexCodePointTest()
{
    char z = '0';
    char a = 'a';
    for (SIZE i = 0; i < 10; ++i)
    {
        CheckRet(__func__, __LINE__, zzz_HexCodePoint(z + i) == i);
    }
    for (SIZE i = 0; i < 6; ++i)
    {
        CheckRet(__func__, __LINE__, zzz_HexCodePoint(a + i) == i + 10);
    }
    for (SIZE i = 6; i < 10; ++i)
    {
        CheckRet(__func__, __LINE__, zzz_HexCodePoint(a + i) == 16);
    }
}
void ConsumeHexTest()
{
    const char *vaildHex0 = "fabb";
    const char *vaildHex1 = "1111";
    const char *vaildHex2 = "f10b";
    const char *invailHex = "fhbb";
    SIZE index = 0;
    SIZE cp = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeHex(vaildHex0, &index, &cp) == zzz_True);
    CheckRet(__func__, __LINE__, cp == 64187);
    CheckRet(__func__, __LINE__, index == 4);
    index = 0;
    cp = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeHex(vaildHex1, &index, &cp) == zzz_True);
    CheckRet(__func__, __LINE__, cp == 4369);
    CheckRet(__func__, __LINE__, index == 4);
    index = 0;
    cp = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeHex(vaildHex2, &index, &cp) == zzz_True);
    CheckRet(__func__, __LINE__, cp == 61707);
    CheckRet(__func__, __LINE__, index == 4);
    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeHex(invailHex, &index, &cp) == zzz_False);
}
void ConsumeStrTest()
{
    const char *vaildStr0 = "0198273645!@@#$%$wqfufewkgefyegrlfbdal[]'';;~~`\"";
    const char *vaildStr1 = "0198273645 \\t\\r\\n\\b\\f\\\" wqfufewkgefyegrlfbdal\"";
    const char *vaildStr2 = "0198273645 \\uD123\\ud222\\u1F00 \\ud801\\udc01 wqfufewkgefyegrlfbdal\"";
    const char *invailStr0 = "019827\t3645\"";
    const char *invailStr1 = "019827 \\ud801\\ud801 3645\"";
    const char *invailStr2 = "019827 \\udc01 3645\"";
    const char *invailStr3 = "019827";
    const char *vailStr4 = "\"";
    SIZE index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(vaildStr0, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildStr0));

    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(vaildStr1, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildStr1));

    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(vaildStr2, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildStr2));

    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(invailStr0, &index) == zzz_False);

    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(invailStr1, &index) == zzz_False);

    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(invailStr2, &index) == zzz_False);

    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(invailStr3, &index) == zzz_False);

    index = 0;
    CheckRet(__func__, __LINE__, zzz_ConsumeStr(vailStr4, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vailStr4));
}
void CheckStrTest()
{
    const char *vaildStr0 = "0198273645!@@#$%$wqfufewkgefyegrlfbdal[]'';;~~`";
    const char *vaildStr1 = "0198273645 \\t\\r\\n\\b\\f\\\" wqfufewkgefyegrlfbdal";
    const char *vaildStr2 = "0198273645 \\uD123\\ud222\\u1F00 \\ud801\\udc01 wqfufewkgefyegrlfbdal";
    const char *invailStr0 = "019827\t3645";
    const char *invailStr1 = "019827 \\ud801\\ud801 3645";
    const char *invailStr2 = "019827 \\udc01 3645";
    const char *invailStr3 = "019827";
    const char *vailStr4 = "";
    SIZE len = 0;
    CheckRet(__func__, __LINE__, zzz_CheckStr(vaildStr0, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildStr0));

    CheckRet(__func__, __LINE__, zzz_CheckStr(vaildStr1, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildStr1));

    CheckRet(__func__, __LINE__, zzz_CheckStr(vaildStr2, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildStr2));

    CheckRet(__func__, __LINE__, zzz_CheckStr(invailStr0, &len) == zzz_False);

    CheckRet(__func__, __LINE__, zzz_CheckStr(invailStr1, &len) == zzz_False);

    CheckRet(__func__, __LINE__, zzz_CheckStr(invailStr2, &len) == zzz_False);

    CheckRet(__func__, __LINE__, zzz_CheckStr(invailStr3, &len) == zzz_True);

    CheckRet(__func__, __LINE__, zzz_CheckStr(vailStr4, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vailStr4));
}
void ConsumeNumTest()
{
    const char *vaildNum0 = "0.1234e123";
    const char *vaildNum1 = "123000";
    const char *vaildNum2 = "123000E123";
    const char *vaildNum3 = "0";
    const char *vaildNum4 = "1.23000E123";
    const char *vaildNum5 = "-1.23000E123";
    const char *vaildNum6 = "1.23000E-123";
    const char *vaildNum7 = "0.0";
    const char *vaildNum8 = "0.03e-123";
    const char *vaildNum9 = "0.1e0123";
    const char *vaildNum10 = "0.00000";

    const char *invaildNum0 = "00";
    const char *invaildNum1 = ".";
    const char *invaildNum2 = ".123";
    const char *invaildNum3 = "01";
    const char *invaildNum4 = "0.1E1.23";
    const char *invaildNum5 = "+123";

    SIZE index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum0, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum0));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum1, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum1));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum2, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum2));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum3, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum3));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum4, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum4));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum5, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum5));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum6, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum6));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum7, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum7));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum8, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum8));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum9, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum9));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(vaildNum10, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == strlen(vaildNum10));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(invaildNum0, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 1);
    CheckRet(__func__, __LINE__, index != strlen(invaildNum0));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(invaildNum1, &index) == zzz_False);

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(invaildNum2, &index) == zzz_False);

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(invaildNum3, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 1);
    CheckRet(__func__, __LINE__, index != strlen(invaildNum3));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(invaildNum4, &index) == zzz_True);
    CheckRet(__func__, __LINE__, index == 5);
    CheckRet(__func__, __LINE__, index != strlen(invaildNum4));

    index = 1;
    CheckRet(__func__, __LINE__, zzz_ConsumeNum(invaildNum5, &index) == zzz_False);
}
void CheckNumTest()
{
    const char *vaildNum0 = "0.1234e123";
    const char *vaildNum1 = "123000";
    const char *vaildNum2 = "123000E123";
    const char *vaildNum3 = "0";
    const char *vaildNum4 = "1.23000E123";
    const char *vaildNum5 = "-1.23000E123";
    const char *vaildNum6 = "1.23000E-123";
    const char *vaildNum7 = "0.0";
    const char *vaildNum8 = "0.03e-123";
    const char *vaildNum9 = "0.1e0123";
    const char *vaildNum10 = "0.00000";

    const char *invaildNum0 = "00";
    const char *invaildNum1 = ".";
    const char *invaildNum2 = ".123";
    const char *invaildNum3 = "01";
    const char *invaildNum4 = "0.1E1.23";
    const char *invaildNum5 = "+123";

    SIZE len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum0, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum0));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum1, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum1));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum2, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum2));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum3, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum3));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum4, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum4));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum5, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum5));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum6, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum6));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum7, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum7));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum8, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum8));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum9, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum9));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(vaildNum10, &len) == zzz_True);
    CheckRet(__func__, __LINE__, len == strlen(vaildNum10));

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(invaildNum0, &len) == zzz_False);

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(invaildNum1, &len) == zzz_False);

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(invaildNum2, &len) == zzz_False);

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(invaildNum3, &len) == zzz_False);

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(invaildNum4, &len) == zzz_False);

    len = 1;
    CheckRet(__func__, __LINE__, zzz_CheckNum(invaildNum5, &len) == zzz_False);
}
void ParseAndStringifyTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "{}";
    CheckRet(__func__, __LINE__, ParseFast(v, json) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "true";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "false";
    CheckRet(__func__, __LINE__, Parse(v, json) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "null";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "123";
    CheckRet(__func__, __LINE__, ParseFast(v, json) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "\"str\"";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "\"\"";
    CheckRet(__func__, __LINE__, ParseFast(v, json) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    const char *json1 = "[true,false,null,[null,false,[],true],[],null";
    CheckRet(__func__, __LINE__, ParseFast(v, json1) == zzz_False);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

    Value *v1 = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v1, json1, strlen(json)) == zzz_False);
    CheckRet(__func__, __LINE__, string(Stringify(v1)) == string(""));

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetStringTest()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "\"str\"";
    const char *str;
    SIZE len;
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    str = GetStringFast(v, &len);
    CheckRet(__func__, __LINE__, str != 0);
    CheckRet(__func__, __LINE__, string(str, len) == string("str"));
    str = GetString(v);
    CheckRet(__func__, __LINE__, str != 0);
    CheckRet(__func__, __LINE__, strlen(str) == 3);
    CheckRet(__func__, __LINE__, string(str) == string("str"));

    json = "123";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    str = GetStringFast(v, &len);
    CheckRet(__func__, __LINE__, str == 0);

    json = "\"\\u007a\\u007a\\u007a\\u004A\\u0053\\u004F\\u004E\\u597D\\u68D2\\uD87E\\uDD21\"";
    CheckRet(__func__, __LINE__, ParseFast(v, json) == zzz_True);
    str = GetUnEscapeString(v);
    CheckRet(__func__, __LINE__, str != 0);
    CheckRet(__func__, __LINE__, string(str) == string("zzzJSON好棒爵"));

    json = "\"\\t \\n \\r \\t \\b \\\\ \\\" \\/ \"";
    CheckRet(__func__, __LINE__, ParseFast(v, json) == zzz_True);
    str = GetUnEscapeString(v);
    CheckRet(__func__, __LINE__, str != 0);
    CheckRet(__func__, __LINE__, string(str) == string("\t \n \r \t \b \\ \" / "));

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetNumTest()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "123";
    const char *num;
    SIZE len;
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    num = GetNumFast(v, &len);
    CheckRet(__func__, __LINE__, num != 0);
    CheckRet(__func__, __LINE__, string(num, len) == string("123"));
    num = GetNumStr(v);
    CheckRet(__func__, __LINE__, num != 0);
    CheckRet(__func__, __LINE__, strlen(num) == 3);
    CheckRet(__func__, __LINE__, string(num) == string("123"));
    const double *d = GetNum(v);
    CheckRet(__func__, __LINE__, d != 0);
    CheckRet(__func__, __LINE__, *d == 123);

    json = "[]";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    num = GetNumFast(v, &len);
    CheckRet(__func__, __LINE__, num == 0);

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetKeyTest()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "{\"key\":123}";
    const char *key;
    SIZE len;
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    key = GetKeyFast(ObjGet(v, "key"), &len);
    CheckRet(__func__, __LINE__, key != 0);
    CheckRet(__func__, __LINE__, string(key, len) == string("key"));

    key = GetKey(ObjGet(v, "key"));
    CheckRet(__func__, __LINE__, key != 0);
    CheckRet(__func__, __LINE__, strlen(key) == 3);
    CheckRet(__func__, __LINE__, string(key) == string("key"));

    Value *vv = ObjGet(v, "key");
    SetKey(vv, "key1");
    key = GetKey(vv);
    CheckRet(__func__, __LINE__, key != 0);
    CheckRet(__func__, __LINE__, strlen(key) == 4);
    CheckRet(__func__, __LINE__, string(key) == string("key1"));

    SetKey(vv, "key2");
    key = GetUnEscapeKey(vv);
    CheckRet(__func__, __LINE__, key != 0);
    CheckRet(__func__, __LINE__, strlen(key) == 4);
    CheckRet(__func__, __LINE__, string(key) == string("key2"));

    SetKey(vv, "key3");
    key = GetUnEscapeKey(vv);
    CheckRet(__func__, __LINE__, key != 0);
    CheckRet(__func__, __LINE__, strlen(key) == 4);
    CheckRet(__func__, __LINE__, string(key) == string("key3"));

    json = "[]";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    key = GetNumFast(v, &len);
    CheckRet(__func__, __LINE__, key == 0);

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetBoolTest()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "false";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    const zzz_BOOL *ret = GetBool(v);
    CheckRet(__func__, __LINE__, ret != 0);
    CheckRet(__func__, __LINE__, *ret == zzz_False);

    json = "true";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    ret = GetBool(v);
    CheckRet(__func__, __LINE__, ret != 0);
    CheckRet(__func__, __LINE__, *ret == zzz_True);

    json = "{}";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    ret = GetBool(v);
    CheckRet(__func__, __LINE__, ret == 0);

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void IsNullTest()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "null";
    zzz_BOOL b;
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    zzz_BOOL ret = IsNull(v);
    CheckRet(__func__, __LINE__, ret == zzz_True);

    json = "true";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    ret = IsNull(v);
    CheckRet(__func__, __LINE__, ret == zzz_False);

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ObjGetTest()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    const char *json = "{\"null\":null}";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *gv = ObjGet(v, "null");
    const JSONType *t;
    t = Type(gv);
    CheckRet(__func__, __LINE__, *t == zzz_JSONTypeNull);

    json = "{\"null\":123}";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    gv = ObjGet(v, "null");
    t = Type(gv);
    CheckRet(__func__, __LINE__, *t == zzz_JSONTypeNum);

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SizeTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "{}";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "true";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "false";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "null";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "123";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "\"str\"";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "\"\"";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 0);

    json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 6);

    json = "{\"null\":123}";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    CheckRet(__func__, __LINE__, Size(v) == 1);

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ArrayGetTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *gv = ArrayGet(v, 0);
    CheckRet(__func__, __LINE__, gv == 0);

    json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    gv = ArrayGet(v, 3);
    const JSONType *t;
    t = Type(gv);
    CheckRet(__func__, __LINE__, *t == zzz_JSONTypeArray);
    gv = ArrayGet(v, 30);
    CheckRet(__func__, __LINE__, gv == 0);

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void BeginAndNextTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *next = Begin(v);
    SIZE count = 0;
    while (next)
    {
        ++count;
        next = Next(next);
    }
    CheckRet(__func__, __LINE__, count == Size(v));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void CopyAndMoveTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[true,false,null,[{\"key\":123},null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *mv = ArrayGet(v, 3);
    CheckRet(__func__, __LINE__, mv != 0);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));
    CheckRet(__func__, __LINE__, Move(mv) == zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[true,false,null,[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, Size(v) == 5);
    CheckRet(__func__, __LINE__, string(Stringify(mv)) == string("[{\"key\":123},null,false,[],true]"));

    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *cv = Copy(ArrayGet(v, 3));
    CheckRet(__func__, __LINE__, cv != 0);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));
    CheckRet(__func__, __LINE__, Size(v) == 6);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("[{\"key\":123},null,false,[],true]"));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetNullTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *sv = ArrayGet(v, 0);
    SetNull(sv);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[null,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    Value *vv = NewValue(A);
    SetNull(vv);
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("null"));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetBoolTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *sv = ArrayGet(v, 0);
    SetBool(sv, zzz_False);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[false,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    Value *vv = NewValue(A);
    SetBool(vv, zzz_True);
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("true"));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetNumTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *sv = ArrayGet(v, 0);
    SetNumStrFast(sv, "123");
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[123,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetNumStrLenFast(sv, "123", 3);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[123,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetNumStr(sv, "1234");
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[1234,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetNumStrLen(sv, "1234", 4);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[1234,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetNum(sv, 12345);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[12345,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, SetNumStrFast(sv, "123abc") == zzz_False);
    CheckRet(__func__, __LINE__, SetNumStrLenFast(sv, "123abc", 6) == zzz_False);
    CheckRet(__func__, __LINE__, SetNumStrLenFast(sv, "123\0", 4) == zzz_False);
    CheckRet(__func__, __LINE__, SetNumStrLenFast(sv, "123\0", 3) == zzz_True);
    CheckRet(__func__, __LINE__, SetNumStr(sv, "123abc") == zzz_False);
    CheckRet(__func__, __LINE__, SetNumStrLen(sv, "123abc", 6) == zzz_False);
    CheckRet(__func__, __LINE__, SetNumStrLen(sv, "123\0", 4) == zzz_False);
    CheckRet(__func__, __LINE__, SetNumStrLen(sv, "123\0", 3) == zzz_True);
    Value *vv = NewValue(A);
    SetNumStrFast(vv, "123");
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("123"));
    SetNumStrLenFast(vv, "123", 3);
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("123"));
    SetNumStr(vv, "1234");
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("1234"));
    SetNumStrLen(vv, "1234", 4);
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("1234"));
    SetNum(vv, 12345);
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("12345"));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetStrTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *sv = ArrayGet(v, 0);
    SetStrFast(sv, "123");
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[\"123\",false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetStrLenFast(sv, "123", 3);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[\"123\",false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetStr(sv, "1234");
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[\"1234\",false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetStrLen(sv, "1234", 4);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[\"1234\",false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, SetStrFast(sv, "123ab\tc") == zzz_False);
    CheckRet(__func__, __LINE__, SetStrLenFast(sv, "123ab\tc", 7) == zzz_False);
    CheckRet(__func__, __LINE__, SetStr(sv, "123a\nbc") == zzz_False);
    CheckRet(__func__, __LINE__, SetStrLen(sv, "123a\nbc", 7) == zzz_False);
    CheckRet(__func__, __LINE__, SetStr(sv, "123a\\udc01bc") == zzz_False);
    CheckRet(__func__, __LINE__, SetStrLen(sv, "123a\\udc01bc", 12) == zzz_False);
    CheckRet(__func__, __LINE__, SetStr(sv, "123a\"aaa") == zzz_False);
    CheckRet(__func__, __LINE__, SetStrLen(sv, "123a\"aaa", 8) == zzz_False);
    Value *vv = NewValue(A);
    SetStrFast(vv, "123");
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("\"123\""));
    SetStrLenFast(vv, "123", 3);
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("\"123\""));
    SetStr(vv, "1234");
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("\"1234\""));
    SetStrLen(vv, "1234", 4);
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("\"1234\""));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetObjArrayTest()
{
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    SetArray(v);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[]"));
    SetObj(v);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("{}"));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetKeyTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[{\"key\":true},false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *sv = ArrayGet(v, 0);
    CheckRet(__func__, __LINE__, SetKeyFast(sv, "123") == zzz_False);
    sv = ObjGet(sv, "key");
    SetKeyFast(sv, "123");
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[{\"123\":true},false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    SetKey(sv, "1234");
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[{\"1234\":true},false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, SetKeyFast(sv, "123ab\tc") == zzz_False);
    CheckRet(__func__, __LINE__, SetKeyFast(sv, "123a\nbc") == zzz_False);
    CheckRet(__func__, __LINE__, SetKey(sv, "123a\\udc01bc") == zzz_False);
    CheckRet(__func__, __LINE__, SetKey(sv, "123a\"aaa") == zzz_False);
    CheckRet(__func__, __LINE__, string(Stringify(sv)) == string("true"));
    Value *vv = NewValue(A);
    SetKeyFast(vv, "123");
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("null"));
    SetKey(vv, "1234");
    CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("null"));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ValueSetTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[{\"key\":true},false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *gv = ArrayGet(v, 0);
    Value *cv = ArrayGet(v, 2);
    Set(cv, gv);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("{\"key\":true}"));
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[{\"key\":true},false,{\"key\":true},[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, string(Stringify(gv)) == string("{\"key\":true}"));
    SetFast(cv, gv);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("{\"key\":true}"));
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[false,{\"key\":true},[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, string(Stringify(gv)) == string(""));

    const char *json1 = "[1,2,3]";
    Value *nv = NewValue(A);
    CheckRet(__func__, __LINE__, Parse(nv, json1) == True);
    CheckRet(__func__, __LINE__, SetFast(ObjGet(cv, "key"), nv) == True);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("{\"key\":[1,2,3]}"));
    Value *av = NewValue(A);
    SetNum(av, 4);
    CheckRet(__func__, __LINE__, ArrayAdd(ObjGet(cv, "key"), av) == True);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("{\"key\":[1,2,3,4]}"));

    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ValueObjAddTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *gv = ObjGet(ArrayGet(v, 0), "key");
    Value *cv = ArrayGet(v, 2);
    ObjAdd(cv, gv);
    CheckRet(__func__, __LINE__, Size(ArrayGet(v, 0)) == 1);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("{\"key1\":true,\"key\":true}"));
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[{\"key\":true},false,{\"key1\":true,\"key\":true},[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, string(Stringify(gv)) == string("true"));
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    gv = ObjGet(ArrayGet(v, 0), "key");
    cv = ArrayGet(v, 2);
    ObjAddFast(cv, gv);
    CheckRet(__func__, __LINE__, Size(ArrayGet(v, 0)) == 0);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("{\"key1\":true,\"key\":true}"));
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[{},false,{\"key1\":true,\"key\":true},[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, Size(v) == 6);
    CheckRet(__func__, __LINE__, string(Stringify(gv)) == string(""));

    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    gv = ArrayGet(v, 0);
    cv = ArrayGet(v, 2);
    CheckRet(__func__, __LINE__, ObjAddFast(cv, gv) == zzz_False);
    CheckRet(__func__, __LINE__, ObjAdd(cv, gv) == zzz_False);
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ValueArrayAddTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[{\"key\":true},false,[],[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    Value *gv = ArrayGet(v, 0);
    Value *cv = ArrayGet(v, 2);
    ArrayAdd(cv, gv);
    CheckRet(__func__, __LINE__, Size(ArrayGet(v, 0)) == 1);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("[{\"key\":true}]"));
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[{\"key\":true},false,[{\"key\":true}],[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, string(Stringify(gv)) == string("{\"key\":true}"));
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    gv = ArrayGet(v, 0);
    cv = ArrayGet(v, 2);
    ArrayAddFast(cv, gv);
    CheckRet(__func__, __LINE__, Size(ArrayGet(v, 0)) == 0);
    CheckRet(__func__, __LINE__, string(Stringify(cv)) == string("[{\"key\":true}]"));
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[false,[{\"key\":true}],[null,false,[],true],[\"\",123,\"str\"],null]"));
    CheckRet(__func__, __LINE__, Size(v) == 5);
    CheckRet(__func__, __LINE__, string(Stringify(gv)) == string(""));

    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    gv = ArrayGet(v, 0);
    cv = ArrayGet(v, 1);
    CheckRet(__func__, __LINE__, ArrayAddFast(cv, gv) == zzz_False);
    CheckRet(__func__, __LINE__, ArrayAdd(cv, gv) == zzz_False);
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void DelTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *v = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == zzz_True);
    ArrayDel(v, 0);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"],null]"));
    ArrayDel(v, 4);
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"]]"));
    ObjDel(ArrayGet(v, 1), "key1");
    CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[false,{},[null,false,[],true],[\"\",123,\"str\"]]"));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetAndSet(Value *srcv, Value *desv)
{
    const JSONType *t;
    t = Type(srcv);
    switch (*t)
    {
    case JSONTYPEARRAY:
    {
        SetArray(desv);
        Value *next = Begin(srcv);
        while (next != 0)
        {
            Value *v = NewValue(desv->A);
            GetAndSet(next, v);
            ArrayAddFast(desv, v);
            next = Next(next);
        }
    }
    break;
    case JSONTYPEOBJ:
    {
        SetObj(desv);
        Value *next = Begin(srcv);
        while (next != 0)
        {
            Value *v = NewValue(desv->A);
            SetKeyFast(v, GetKey(next));
            GetAndSet(next, v);
            ObjAddFast(desv, v);
            next = Next(next);
        }
    }
    break;
    case JSONTYPEBOOL:
    {
        const zzz_BOOL *b = GetBool(srcv);
        if (b == 0)
            return;
        SetBool(desv, *b);
    }
    break;
    case JSONTYPENULL:
    {
        zzz_BOOL b;
        if (IsNull(srcv) == zzz_False)
            return;
        SetNull(desv);
    }
    break;
    case JSONTYPESTRING:
    {
        const char *str = GetString(srcv);
        if (str == 0)
            return;
        SetStrFast(desv, str);
    }
    break;
    case JSONTYPENUM:
    {
        const char *str = GetNumStr(srcv);
        if (str == 0)
            return;
        SetNumStrFast(desv, str);
    }
    break;
    }
}
void GetSetTest()
{
    Allocator *A = NewAllocator();
    const char *json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],true],[\"\",123,\"str\"],null]";
    Value *srcv = NewValue(A);
    Value *desv = NewValue(A);
    CheckRet(__func__, __LINE__, ParseLen(srcv, json, strlen(json)) == zzz_True);
    GetAndSet(srcv, desv);
    CheckRet(__func__, __LINE__, string(Stringify(desv)) == string(json));
    ReleaseAllocator(A);
    CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetSetTest1()
{
    string data_path = "test/conformance/data/roundtrip/roundtrip";
    for (int i = 1; i <= 37; ++i)
    {
        stringstream ss;
        ss << data_path;
        if (i < 10)
            ss << "0";
        ss << i;
        ss << ".json";
        string json = Read(ss.str());
        Allocator *A = NewAllocator();
        Value *v = NewValue(A);
        BOOL ret = ParseFast(v, json.c_str());

        Value *vv = Copy(v);
        const char *ret_json = Stringify(vv);
        CheckRet(__func__, __LINE__, ret == True);
        CheckRet(__func__, __LINE__, ret_json != 0);
        CheckRet(__func__, __LINE__, string(json.c_str()) == string(ret_json));

        Value *vvv = NewValue(A);
        GetAndSet(vv, vvv);
        ret_json = Stringify(vvv);
        CheckRet(__func__, __LINE__, ret == True);
        CheckRet(__func__, __LINE__, ret_json != 0);
        CheckRet(__func__, __LINE__, string(json.c_str()) == string(ret_json));
        ReleaseAllocator(A);
    }
}
uint32_t RandomNum()
{
    random_device rd;
    return rd();
}

string RandomStr()
{
    string ret_str;
    int len = RandomNum() % 52 + 1;
    for (int i = 0; i < len; ++i)
    {
        if (RandomNum() % 2)
            ret_str += 'a' + RandomNum() % 26;
        else
            ret_str += 'A' + RandomNum() % 26;
    }
    return ret_str;
}

BOOL GenRandomValue(Value *v, int *len, int level, int max_len, int max_level)
{
    if (level >= max_level)
    {
        return False;
    }
    if (*len >= max_len)
    {
        return False;
    }
    ++level;
    int type = RandomNum() % 7;
    if (*len == 0)
        type = 5 + type % 2;
    switch (type)
    {
    // null
    case 0:
        SetNull(v);
        break;
    // true
    case 1:
        SetBool(v, True);
        break;
    // false
    case 2:
        SetBool(v, False);
        break;
    // str
    case 3:
        SetStr(v, RandomStr().c_str());
        break;
    case 4:
        SetNum(v, RandomNum());
        break;
    // obj
    case 5:
    {
        SetObj(v);
        ++(*len);
        for (int i = 0; i < max_len; ++i)
        {
            Value *vv = NewValue(v->A);
            if (SetKey(vv, RandomStr().c_str()) == False)
            {
                CheckRet(__func__, __LINE__, False);
                return False;
            }
            if (GenRandomValue(vv, len, level, max_len, max_level) == True)
            {
                if (ObjAddFast(v, vv) == False)
                {
                    CheckRet(__func__, __LINE__, False);
                    return False;
                }
                else
                    ++(*len);
            }
            else
            {
                return True;
            }
        }
        break;
    }
    // array
    case 6:
    {
        SetArray(v);
        ++(*len);
        for (int i = 0; i < max_len; ++i)
        {
            Value *vv = NewValue(v->A);
            if (GenRandomValue(vv, len, level, max_len, max_level) == True)
            {
                if (ArrayAddFast(v, vv) == False)
                {
                    CheckRet(__func__, __LINE__, False);
                    return False;
                }
                else
                    ++(*len);
            }
            else
            {
                return True;
            }
        }
        break;
    }
    break;
    }
    return True;
}

void CoreTest()
{
    int max_len = 100000;
    int max_level = 100;
    int len = 0;
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    GenRandomValue(v, &len, 0, max_len, max_level);
    const char *json = Stringify(v);
    CheckRet(__func__, __LINE__, json != 0);
    ReleaseAllocator(A);
}
int main()
{
    printf("==================%s==================\n", "function_test");
    MemoryTest();
    StringTest();
    SkinTest();
    PeekTest();
    ConsumeTest();
    PeekAndConsumeTest();
    ConsumeTrueFalseNullTest();
    HexCodePointTest();
    ConsumeHexTest();
    ConsumeStrTest();
    CheckStrTest();
    ConsumeNumTest();
    CheckNumTest();
    ParseAndStringifyTest();
    GetStringTest();
    GetNumTest();
    GetKeyTest();
    GetBoolTest();
    IsNullTest();
    ObjGetTest();
    SizeTest();
    ArrayGetTest();
    BeginAndNextTest();
    CopyAndMoveTest();
    SetNullTest();
    SetBoolTest();
    SetNumTest();
    SetStrTest();
    SetObjArrayTest();
    SetKeyTest();
    ValueSetTest();
    ValueObjAddTest();
    ValueArrayAddTest();
    DelTest();
    GetSetTest();
    GetSetTest1();
    CoreTest();
    PrintResult();
    return 0;
}