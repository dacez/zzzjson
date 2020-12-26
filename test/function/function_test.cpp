#include "../test.h"
#include <random>
#include <string>
#define zj_MEMORY_MODE 2
#include "../../zzzjson.h"

using namespace std;
using namespace zj;

void MemoryTest() {
  Allocator *A = NewAllocator();
  for (zj_Size i = 1; i <= zj_AllocatorInitMemSize; ++i) {
    zj_allocatorAlloc(A, i);
  }
  ReleaseAllocator(A);
  cout << "Size:" << AllocMemorySize << " AllocCount:" << AllocMemoryCount
       << " FreeCount:" << FreeMemoryCount << endl;
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void StringTest() {
  Allocator *A = NewAllocator();
  const char *str =
      "1234567890abcdefghijklmnopqrstuvwxyz`~!@#$%^&*()_+-=|\\}{:[];'?><,./\"";
  for (zj_Size i = 0; i < sizeof(str); ++i) {
    zj_string *zs = zj_newString(A, zj_StringInitMemSize);
    zj_appendStr(zs, str, i);
    CheckRet(__func__, __LINE__, string(zj_str(zs), zs->Pos) == string(str, i));
    zj_appendEnd(zs);
    CheckRet(__func__, __LINE__, *(zj_str(zs) + i) == 0);
  }
  for (zj_Size i = sizeof(str) - 1;; --i) {
    zj_string *zs = zj_newString(A, zj_StringInitMemSize);
    for (zj_Size j = 0; j < i; ++j) {
      zj_appendChar(zs, *(str + j));
    }
    CheckRet(__func__, __LINE__, string(zj_str(zs), zs->Pos) == string(str, i));
    zj_appendEnd(zs);
    CheckRet(__func__, __LINE__, *(zj_str(zs) + i) == 0);
    if (i == 0)
      break;
  }
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SkinTest() {
  const char s[] = "abc\t\n\r d";
  zj_Size index = 0;
  zj_Size skinSum = 0;
  for (zj_Size i = 0; i < sizeof(s); ++i) {
    if (zj_skin(s[i]))
      ++skinSum;
  }
  CheckRet(__func__, __LINE__, skinSum == 4);
}
void PeekTest() {
  const char s[] = "abc\t\n\r d";
  zj_Size index = 0;

  CheckRet(__func__, __LINE__, zj_peek(s, &index) == 'a');
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_peek(s, &index) == 'b');
  CheckRet(__func__, __LINE__, index == 2);

  CheckRet(__func__, __LINE__, zj_peek(s, &index) == 'c');
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__, zj_peek(s, &index) == 'd');
  CheckRet(__func__, __LINE__, index == 8);
}
void ConsumeTest() {
  const char s[] = "abc\t\n\r d";
  zj_Size index = 0;

  CheckRet(__func__, __LINE__, zj_consume('a', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_consume('a', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_consume('b', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 2);

  CheckRet(__func__, __LINE__, zj_consume('c', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__, zj_consume('c', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__, zj_consume('d', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 3);

  index = 0;

  CheckRet(__func__, __LINE__, zj_likelyConsume('a', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_likelyConsume('a', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_likelyConsume('b', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 2);

  CheckRet(__func__, __LINE__, zj_likelyConsume('c', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__, zj_likelyConsume('c', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__, zj_likelyConsume('d', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 3);

  index = 0;

  CheckRet(__func__, __LINE__, zj_unLikelyConsume('a', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_unLikelyConsume('a', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_unLikelyConsume('b', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 2);

  CheckRet(__func__, __LINE__, zj_unLikelyConsume('c', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__, zj_unLikelyConsume('c', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__, zj_unLikelyConsume('d', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 3);
}
void PeekAndConsumeTest() {
  const char s[] = "abc\t\n\r d";
  zj_Size index = 0;

  CheckRet(__func__, __LINE__, zj_likelyPeekAndConsume('a', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__,
           zj_likelyPeekAndConsume('a', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__, zj_likelyPeekAndConsume('b', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 2);

  CheckRet(__func__, __LINE__, zj_likelyPeekAndConsume('c', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__,
           zj_likelyPeekAndConsume('c', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 7);

  CheckRet(__func__, __LINE__, zj_likelyPeekAndConsume('d', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 8);

  index = 0;

  CheckRet(__func__, __LINE__,
           zj_unLikelyPeekAndConsume('a', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__,
           zj_unLikelyPeekAndConsume('a', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 1);

  CheckRet(__func__, __LINE__,
           zj_unLikelyPeekAndConsume('b', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 2);

  CheckRet(__func__, __LINE__,
           zj_unLikelyPeekAndConsume('c', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 3);

  CheckRet(__func__, __LINE__,
           zj_unLikelyPeekAndConsume('c', s, &index) == false);
  CheckRet(__func__, __LINE__, index == 7);

  CheckRet(__func__, __LINE__,
           zj_unLikelyPeekAndConsume('d', s, &index) == true);
  CheckRet(__func__, __LINE__, index == 8);
}
void ConsumetruefalseNullTest() {
  zj_Size index = 1;
  CheckRet(__func__, __LINE__, zj_consumeTrue(zj_strTrue, &index) == true);
  CheckRet(__func__, __LINE__, index == 4);
  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeFalse(zj_strFalse, &index) == true);
  CheckRet(__func__, __LINE__, index == 5);
  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNull(zj_strNull, &index) == true);
  CheckRet(__func__, __LINE__, index == 4);
  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeTrue(zj_strNull, &index) == false);
  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeFalse(zj_strTrue, &index) == false);
  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNull(zj_strFalse, &index) == false);
}
void HexCodePointTest() {
  char z = '0';
  char a = 'a';
  for (zj_Size i = 0; i < 10; ++i) {
    CheckRet(__func__, __LINE__, zj_hexCodePoint(z + i) == i);
  }
  for (zj_Size i = 0; i < 6; ++i) {
    CheckRet(__func__, __LINE__, zj_hexCodePoint(a + i) == i + 10);
  }
  for (zj_Size i = 6; i < 10; ++i) {
    CheckRet(__func__, __LINE__, zj_hexCodePoint(a + i) == 16);
  }
}
void ConsumeHexTest() {
  const char *vaildHex0 = "fabb";
  const char *vaildHex1 = "1111";
  const char *vaildHex2 = "f10b";
  const char *invailHex = "fhbb";
  zj_Size index = 0;
  zj_Size cp = 0;
  CheckRet(__func__, __LINE__, zj_consumeHex(vaildHex0, &index, &cp) == true);
  CheckRet(__func__, __LINE__, cp == 64187);
  CheckRet(__func__, __LINE__, index == 4);
  index = 0;
  cp = 0;
  CheckRet(__func__, __LINE__, zj_consumeHex(vaildHex1, &index, &cp) == true);
  CheckRet(__func__, __LINE__, cp == 4369);
  CheckRet(__func__, __LINE__, index == 4);
  index = 0;
  cp = 0;
  CheckRet(__func__, __LINE__, zj_consumeHex(vaildHex2, &index, &cp) == true);
  CheckRet(__func__, __LINE__, cp == 61707);
  CheckRet(__func__, __LINE__, index == 4);
  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeHex(invailHex, &index, &cp) == false);
}
void ConsumeStrTest() {
  const char *vaildStr0 = "0198273645!@@#$%$wqfufewkgefyegrlfbdal[]'';;~~`\"";
  const char *vaildStr1 =
      "0198273645 \\t\\r\\n\\b\\f\\\" wqfufewkgefyegrlfbdal\"";
  const char *vaildStr2 =
      "0198273645 \\uD123\\ud222\\u1F00 \\ud801\\udc01 wqfufewkgefyegrlfbdal\"";
  const char *invailStr0 = "019827\t3645\"";
  const char *invailStr1 = "019827 \\ud801\\ud801 3645\"";
  const char *invailStr2 = "019827 \\udc01 3645\"";
  const char *invailStr3 = "019827";
  const char *vailStr4 = "\"";
  zj_Size index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(vaildStr0, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildStr0));

  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(vaildStr1, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildStr1));

  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(vaildStr2, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildStr2));

  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(invailStr0, &index) == false);

  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(invailStr1, &index) == false);

  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(invailStr2, &index) == false);

  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(invailStr3, &index) == false);

  index = 0;
  CheckRet(__func__, __LINE__, zj_consumeStr(vailStr4, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vailStr4));
}
void CheckStrTest() {
  const char *vaildStr0 = "0198273645!@@#$%$wqfufewkgefyegrlfbdal[]'';;~~`";
  const char *vaildStr1 =
      "0198273645 \\t\\r\\n\\b\\f\\\" wqfufewkgefyegrlfbdal";
  const char *vaildStr2 =
      "0198273645 \\uD123\\ud222\\u1F00 \\ud801\\udc01 wqfufewkgefyegrlfbdal";
  const char *invailStr0 = "019827\t3645";
  const char *invailStr1 = "019827 \\ud801\\ud801 3645";
  const char *invailStr2 = "019827 \\udc01 3645";
  const char *invailStr3 = "019827";
  const char *vailStr4 = "";
  zj_Size len = 0;
  bool need_escape = false;
  CheckRet(__func__, __LINE__, zj_checkStr(vaildStr0, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildStr0));

  CheckRet(__func__, __LINE__, zj_checkStr(vaildStr1, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildStr1));

  CheckRet(__func__, __LINE__, zj_checkStr(vaildStr2, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildStr2));

  CheckRet(__func__, __LINE__, zj_checkStr(invailStr0, &len) == false);

  CheckRet(__func__, __LINE__, zj_checkStr(invailStr1, &len) == false);

  CheckRet(__func__, __LINE__, zj_checkStr(invailStr2, &len) == false);

  CheckRet(__func__, __LINE__, zj_checkStr(invailStr3, &len) == true);

  CheckRet(__func__, __LINE__, zj_checkStr(vailStr4, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vailStr4));
}
void ConsumeNumTest() {
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

  zj_Size index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum0, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum0));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum1, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum1));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum2, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum2));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum3, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum3));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum4, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum4));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum5, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum5));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum6, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum6));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum7, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum7));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum8, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum8));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum9, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum9));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(vaildNum10, &index) == true);
  CheckRet(__func__, __LINE__, index == strlen(vaildNum10));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(invaildNum0, &index) == true);
  CheckRet(__func__, __LINE__, index == 1);
  CheckRet(__func__, __LINE__, index != strlen(invaildNum0));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(invaildNum1, &index) == false);

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(invaildNum2, &index) == false);

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(invaildNum3, &index) == true);
  CheckRet(__func__, __LINE__, index == 1);
  CheckRet(__func__, __LINE__, index != strlen(invaildNum3));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(invaildNum4, &index) == true);
  CheckRet(__func__, __LINE__, index == 5);
  CheckRet(__func__, __LINE__, index != strlen(invaildNum4));

  index = 1;
  CheckRet(__func__, __LINE__, zj_consumeNum(invaildNum5, &index) == false);
}
void CheckNumTest() {
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

  zj_Size len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum0, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum0));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum1, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum1));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum2, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum2));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum3, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum3));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum4, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum4));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum5, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum5));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum6, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum6));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum7, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum7));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum8, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum8));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum9, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum9));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(vaildNum10, &len) == true);
  CheckRet(__func__, __LINE__, len == strlen(vaildNum10));

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(invaildNum0, &len) == false);

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(invaildNum1, &len) == false);

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(invaildNum2, &len) == false);

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(invaildNum3, &len) == false);

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(invaildNum4, &len) == false);

  len = 1;
  CheckRet(__func__, __LINE__, zj_checkNum(invaildNum5, &len) == false);
}
void EscapeStrTest() {
  const char *str = "极其朴素的代码，丰富的文档和注释，没有任何花销的技巧，简单"
                    "易读好修改是zzzJSON的第一宗旨，因为放心地使"
                    "用开源代码的前提是能够驾驭开源代码，zzzJSON使用最朴素的语"
                    "法，没有任何花销，简单易读，丰富的文档和注释"
                    "使使用者在需要增加新特性时，能够轻松修改，最大限度地降低使"
                    "用者的心智负担。极致的解析和序列化速度是"
                    "zzzJSON的灵魂，在解析和序列化方面，zzzJSON的表现非常惊艳。"
                    "读时解析是zzzJSON的特色，在业务开发过程中，"
                    "经常需要修改一个大JSON的一个小部分，或者获取其中一个值，读"
                    "时解析在解析JSON文本时只判断数字的正确性，而"
                    "不把数字转换成数字类型，最大限度地减少解析数字造成的性能开"
                    "销。使用时可以根据业务要求，选择不同精度的解析"
                    "函数，甚至使用超过双精度浮点型的精度。";
  zj_Allocator *A = zj_NewAllocator();
  const char *des = zj_escapeStr(str, A);
  CheckRet(__func__, __LINE__, string(des) == string(str));

  const char *str1 = "极其朴素的代码，丰富的文档和注释，没有任何花销的技巧，简"
                     "单易读好修改是zzzJSON的第一宗旨，因为放心地使"
                     "用开源代码的前提是能够驾驭开源代码，zzzJSON使用最朴素的语"
                     "法，没有任何花销，简单易读，丰富的文档和注释"
                     "使使用者在需要增加新特性时，能够轻松修改，最大限度地降低"
                     "使用者的心智负担。极致的解析和序列化速度是"
                     "zzzJSON的灵魂，在解析和序列化方面，zzzJSON的表现非常惊艳"
                     "。读时解析是zzzJSON的特色，在业务开发过程中，"
                     "经常需要修改一个大JSON的一个小部分，或者获取其中一个值，"
                     "读时解析在解析JSON文本时只判断数字的正确性，而"
                     "不把数字转换成数字类型，最大限度地减少解析数字造成的性能"
                     "开销。使用时可以根据业务要求，选择不同精度的解析"
                     "函数，甚至使用超过双精度浮点型的精度。\r\t \n\b \f \\ \"";
  const char *str2 =
      "极其朴素的代码，丰富的文档和注释，没有任何花销的技巧，简单易读好修改是zz"
      "zJSON的第一宗旨，因为放心地使"
      "用开源代码的前提是能够驾驭开源代码，zzzJSON使用最朴素的语法，没有任何花"
      "销，简单易读，丰富的文档和注释"
      "使使用者在需要增加新特性时，能够轻松修改，最大限度地降低使用者的心智负担"
      "。极致的解析和序列化速度是"
      "zzzJSON的灵魂，在解析和序列化方面，zzzJSON的表现非常惊艳。读时解析是zzzJ"
      "SON的特色，在业务开发过程中，"
      "经常需要修改一个大JSON的一个小部分，或者获取其中一个值，读时解析在解析JS"
      "ON文本时只判断数字的正确性，而"
      "不把数字转换成数字类型，最大限度地减少解析数字造成的性能开销。使用时可以"
      "根据业务要求，选择不同精度的解析"
      "函数，甚至使用超过双精度浮点型的精度。\\r\\t \\n\\b \\f \\\\ \\\"";

  const char *des1 = zj_escapeStr(str1, A);
  CheckRet(__func__, __LINE__, string(des1) == string(str2));

  zj_ReleaseAllocator(A);
}
void ParseAndStringifyTest() {
  Allocator *A = NewAllocator();
  const char *json = "[]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "{}";
  CheckRet(__func__, __LINE__, ParseFast(v, json) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "true";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "false";
  CheckRet(__func__, __LINE__, Parse(v, json) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "null";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "123";
  CheckRet(__func__, __LINE__, ParseFast(v, json) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "\"str\"";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "\"\"";
  CheckRet(__func__, __LINE__, ParseFast(v, json) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  const char *json1 = "[true,false,null,[null,false,[],true],[],null";
  CheckRet(__func__, __LINE__, ParseFast(v, json1) == false);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));

  Value *v1 = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v1, json1, strlen(json)) == false);
  CheckRet(__func__, __LINE__, string(Stringify(v1)) == string(""));

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetStringTest() {
  Allocator *A = NewAllocator();
  Value *v = NewValue(A);
  const char *json = "\"str\"";
  const char *str;
  zj_Size len;
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  str = GetStrFast(v, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == string("str"));
  str = GetStr(v);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, strlen(str) == 3);
  CheckRet(__func__, __LINE__, string(str) == string("str"));

  json = "123";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  str = GetStrFast(v, &len);
  CheckRet(__func__, __LINE__, str == 0);

  json = "\"\\u007a\\u007a\\u007a\\u004A\\u0053\\u004F\\u004E\\u597D\\u68D2\\uD"
         "87E\\uDD21\"";
  CheckRet(__func__, __LINE__, ParseFast(v, json) == true);
  str = GetUnEscapeStr(v);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str) == string("zzzJSON好棒爵"));

  json = "\"\\t \\n \\r \\t \\b \\\\ \\\" \\/ \"";
  CheckRet(__func__, __LINE__, ParseFast(v, json) == true);
  str = GetUnEscapeStr(v);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__,
           string(str) == string("\t \n \r \t \b \\ \" / "));

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetNumTest() {
  Allocator *A = NewAllocator();
  Value *v = NewValue(A);
  const char *json = "123";
  const char *num;
  zj_Size len;
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
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
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  num = GetNumFast(v, &len);
  CheckRet(__func__, __LINE__, num == 0);

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetKeyTest() {
  Allocator *A = NewAllocator();
  Value *v = NewValue(A);
  const char *json = "{\"key\":123}";
  const char *key;
  zj_Size len;
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
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
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  key = GetNumFast(v, &len);
  CheckRet(__func__, __LINE__, key == 0);

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetBoolTest() {
  Allocator *A = NewAllocator();
  Value *v = NewValue(A);
  const char *json = "false";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  const bool *ret = GetBool(v);
  CheckRet(__func__, __LINE__, ret != 0);
  CheckRet(__func__, __LINE__, *ret == false);

  json = "true";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  ret = GetBool(v);
  CheckRet(__func__, __LINE__, ret != 0);
  CheckRet(__func__, __LINE__, *ret == true);

  json = "{}";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  ret = GetBool(v);
  CheckRet(__func__, __LINE__, ret == 0);

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void IsNullTest() {
  Allocator *A = NewAllocator();
  Value *v = NewValue(A);
  const char *json = "null";
  bool b;
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  bool ret = IsNull(v);
  CheckRet(__func__, __LINE__, ret == true);

  json = "true";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  ret = IsNull(v);
  CheckRet(__func__, __LINE__, ret == false);

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ObjGetTest() {
  Allocator *A = NewAllocator();
  Value *v = NewValue(A);
  const char *json = "{\"null\":null}";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *gv = ObjGet(v, "null");
  const JSONType *t;
  t = Type(gv);
  CheckRet(__func__, __LINE__, *t == JSONTypeNull);

  json = "{\"null\":123}";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  gv = ObjGet(v, "null");
  t = Type(gv);
  CheckRet(__func__, __LINE__, *t == JSONTypeNumber);

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SizeTest() {
  Allocator *A = NewAllocator();
  const char *json = "[]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "{}";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "true";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "false";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "null";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "123";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "\"str\"";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "\"\"";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 0);

  json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 6);

  json = "{\"null\":123}";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 1);

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ArrayGetTest() {
  Allocator *A = NewAllocator();
  const char *json = "[]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *gv = ArrayGet(v, 0);
  CheckRet(__func__, __LINE__, gv == 0);

  json = "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  gv = ArrayGet(v, 3);
  const JSONType *t;
  t = Type(gv);
  CheckRet(__func__, __LINE__, *t == JSONTypeArray);
  gv = ArrayGet(v, 30);
  CheckRet(__func__, __LINE__, gv == 0);

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void BeginAndNextTest() {
  Allocator *A = NewAllocator();
  const char *json =
      "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *next = Begin(v);
  zj_Size count = 0;
  while (next) {
    ++count;
    next = Next(next);
  }
  CheckRet(__func__, __LINE__, count == zj_SizeOf(v));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void CopyAndMoveTest() {
  Allocator *A = NewAllocator();
  const char *json = "[true,false,null,[{\"key\":123},null,false,[],true],["
                     "\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *mv = ArrayGet(v, 3);
  CheckRet(__func__, __LINE__, mv != 0);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));
  CheckRet(__func__, __LINE__, Move(mv) == true);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[true,false,null,[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 5);
  CheckRet(__func__, __LINE__,
           string(Stringify(mv)) ==
               string("[{\"key\":123},null,false,[],true]"));

  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *cv = Copy(ArrayGet(v, 3));
  CheckRet(__func__, __LINE__, cv != 0);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string(json));
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 6);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) ==
               string("[{\"key\":123},null,false,[],true]"));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetNullTest() {
  Allocator *A = NewAllocator();
  const char *json =
      "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *sv = ArrayGet(v, 0);
  SetNull(sv);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[null,false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  Value *vv = NewValue(A);
  SetNull(vv);
  CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("null"));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetBoolTest() {
  Allocator *A = NewAllocator();
  const char *json =
      "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *sv = ArrayGet(v, 0);
  SetBool(sv, false);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[false,false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  Value *vv = NewValue(A);
  SetBool(vv, true);
  CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("true"));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetNumTest() {
  Allocator *A = NewAllocator();
  const char *json =
      "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *sv = ArrayGet(v, 0);
  SetNumStrFast(sv, "123");
  CheckRet(
      __func__, __LINE__,
      string(Stringify(v)) ==
          string(
              "[123,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
  SetNumStrLenFast(sv, "123", 3);
  CheckRet(
      __func__, __LINE__,
      string(Stringify(v)) ==
          string(
              "[123,false,null,[null,false,[],true],[\"\",123,\"str\"],null]"));
  SetNumStr(sv, "1234");
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[1234,false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  SetNumStrLen(sv, "1234", 4);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[1234,false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  SetNum(sv, 12345);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[12345,false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__, SetNumStrFast(sv, "123abc") == false);
  CheckRet(__func__, __LINE__, SetNumStrLenFast(sv, "123abc", 6) == false);
  CheckRet(__func__, __LINE__, SetNumStrLenFast(sv, "123\0", 4) == false);
  CheckRet(__func__, __LINE__, SetNumStrLenFast(sv, "123\0", 3) == true);
  CheckRet(__func__, __LINE__, SetNumStr(sv, "123abc") == false);
  CheckRet(__func__, __LINE__, SetNumStrLen(sv, "123abc", 6) == false);
  CheckRet(__func__, __LINE__, SetNumStrLen(sv, "123\0", 4) == false);
  CheckRet(__func__, __LINE__, SetNumStrLen(sv, "123\0", 3) == true);
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

  SetDouble(vv, 12345.233);
  CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("12345.233"));
  SetInt(vv, -12345);
  CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("-12345"));
  SetLong(vv, -123456);
  CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("-123456"));
  SetLongLong(vv, -1234567890987);
  CheckRet(__func__, __LINE__,
           string(Stringify(vv)) == string("-1234567890987"));

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetStrTest() {
  Allocator *A = NewAllocator();
  const char *json =
      "[true,false,null,[null,false,[],true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *sv = ArrayGet(v, 0);
  SetStrFast(sv, "123");
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[\"123\",false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  SetStrLenFast(sv, "123", 3);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[\"123\",false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  SetStr(sv, "1234");
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[\"1234\",false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  SetStrLen(sv, "1234", 4);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[\"1234\",false,null,[null,false,[],"
                                          "true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__, SetStrFast(sv, "123ab\tc") == false);
  CheckRet(__func__, __LINE__, SetStrEscape(sv, "123ab\tc") == true);
  zj_Size len = 0;
  const char *str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123ab\\tc");

  CheckRet(__func__, __LINE__, SetStrLenFast(sv, "123ab\tc", 7) == false);
  CheckRet(__func__, __LINE__, SetStrLenEscape(sv, "123ab\tc", 7) == true);
  str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123ab\\tc");

  CheckRet(__func__, __LINE__, SetStr(sv, "123a\nbc") == false);
  CheckRet(__func__, __LINE__, SetStrEscape(sv, "123a\nbc") == true);
  str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123a\\nbc");

  CheckRet(__func__, __LINE__, SetStrLen(sv, "123a\nbc", 7) == false);
  CheckRet(__func__, __LINE__, SetStrLenEscape(sv, "123a\nbc", 7) == true);
  str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123a\\nbc");

  CheckRet(__func__, __LINE__, SetStr(sv, "123a\\udc01bc") == false);
  CheckRet(__func__, __LINE__, SetStrEscape(sv, "123a\\udc01bc") == true);
  str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123a\\\\udc01bc");

  CheckRet(__func__, __LINE__, SetStrLen(sv, "123a\\udc01bc", 12) == false);
  CheckRet(__func__, __LINE__,
           SetStrLenEscape(sv, "123a\\udc01bc", 12) == true);
  str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123a\\\\udc01bc");

  CheckRet(__func__, __LINE__, SetStr(sv, "123a\"aaa") == false);
  CheckRet(__func__, __LINE__, SetStrEscape(sv, "123a\"aaa") == true);
  str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123a\\\"aaa");

  CheckRet(__func__, __LINE__, SetStrLen(sv, "123a\"aaa", 8) == false);
  CheckRet(__func__, __LINE__, SetStrLenEscape(sv, "123a\"aaa", 8) == true);
  str = GetStrFast(sv, &len);
  CheckRet(__func__, __LINE__, str != 0);
  CheckRet(__func__, __LINE__, string(str, len) == "123a\\\"aaa");

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
void SetObjArrayTest() {
  Allocator *A = NewAllocator();
  Value *v = NewValue(A);
  SetArray(v);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string("[]"));
  SetObj(v);
  CheckRet(__func__, __LINE__, string(Stringify(v)) == string("{}"));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void SetKeyTest() {
  Allocator *A = NewAllocator();
  const char *json = "[{\"key\":true},false,null,[null,false,[],true],[\"\","
                     "123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *sv = ArrayGet(v, 0);
  CheckRet(__func__, __LINE__, SetKeyFast(sv, "123") == false);
  sv = ObjGet(sv, "key");
  SetKeyFast(sv, "123");
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[{\"123\":true},false,null,[null,false,[],true],[\"\","
                      "123,\"str\"],null]"));
  SetKey(sv, "1234");
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[{\"1234\":true},false,null,[null,false,[],true],[\"\","
                      "123,\"str\"],null]"));

  CheckRet(__func__, __LINE__, SetKeyFast(sv, "123ab\tc") == false);
  CheckRet(__func__, __LINE__, SetKeyEscape(sv, "123ab\tc") == true);
  zj_Size len = 0;
  const char *key = GetKeyFast(sv, &len);
  CheckRet(__func__, __LINE__, key != 0);
  CheckRet(__func__, __LINE__, string(key, len) == "123ab\\tc");
  CheckRet(__func__, __LINE__, SetKey(sv, "123a\"aaa") == false);
  CheckRet(__func__, __LINE__, SetKeyEscape(sv, "123a\"aaa") == true);
  key = GetKeyFast(sv, &len);
  CheckRet(__func__, __LINE__, key != 0);
  CheckRet(__func__, __LINE__, string(key, len) == "123a\\\"aaa");
  CheckRet(__func__, __LINE__, string(Stringify(sv)) == string("true"));

  CheckRet(__func__, __LINE__, SetKeyLen(sv, "123ab\tc", 7) == false);
  CheckRet(__func__, __LINE__, SetKeyLenEscape(sv, "123ab\tc", 7) == true);
  len = 0;
  key = GetKeyFast(sv, &len);
  CheckRet(__func__, __LINE__, key != 0);
  CheckRet(__func__, __LINE__, string(key, len) == "123ab\\tc");

  CheckRet(__func__, __LINE__, SetKeyLenFast(sv, "123a\"aaa", 8) == false);
  CheckRet(__func__, __LINE__, SetKeyLenEscape(sv, "123a\"aaa", 8) == true);
  key = GetKeyFast(sv, &len);
  CheckRet(__func__, __LINE__, key != 0);
  CheckRet(__func__, __LINE__, string(key, len) == "123a\\\"aaa");
  CheckRet(__func__, __LINE__, string(Stringify(sv)) == string("true"));

  CheckRet(__func__, __LINE__, SetKey(sv, "123a\\udc01bc") == false);
  CheckRet(__func__, __LINE__, SetKeyEscape(sv, "123a\\udc01bc") == true);
  key = GetKeyFast(sv, &len);
  CheckRet(__func__, __LINE__, key != 0);
  CheckRet(__func__, __LINE__, string(key, len) == "123a\\\\udc01bc");
  CheckRet(__func__, __LINE__, string(Stringify(sv)) == string("true"));

  Value *vv = NewValue(A);
  SetKeyFast(vv, "123");
  CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("null"));
  SetKey(vv, "1234");
  CheckRet(__func__, __LINE__, string(Stringify(vv)) == string("null"));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ValueSetTest() {
  Allocator *A = NewAllocator();
  const char *json = "[{\"key\":true},false,null,[null,false,[],true],[\"\","
                     "123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *gv = ArrayGet(v, 0);
  Value *cv = ArrayGet(v, 2);
  Set(cv, gv);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("{\"key\":true}"));
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[{\"key\":true},false,{\"key\":true},[null,false,[],"
                      "true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__,
           string(Stringify(gv)) == string("{\"key\":true}"));
  SetFast(cv, gv);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("{\"key\":true}"));
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[false,{\"key\":true},[null,false,[]"
                                          ",true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__, string(Stringify(gv)) == string(""));

  const char *json1 = "[1,2,3]";
  Value *nv = NewValue(A);
  CheckRet(__func__, __LINE__, Parse(nv, json1) == true);
  CheckRet(__func__, __LINE__, SetFast(ObjGet(cv, "key"), nv) == true);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("{\"key\":[1,2,3]}"));
  Value *av = NewValue(A);
  SetNum(av, 4);
  CheckRet(__func__, __LINE__, ArrayAdd(ObjGet(cv, "key"), av) == true);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("{\"key\":[1,2,3,4]}"));

  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ValueObjAddTest() {
  Allocator *A = NewAllocator();
  const char *json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],"
                     "true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *gv = ObjGet(ArrayGet(v, 0), "key");
  Value *cv = ArrayGet(v, 2);
  ObjAdd(cv, gv);
  CheckRet(__func__, __LINE__, zj_SizeOf(ArrayGet(v, 0)) == 1);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("{\"key1\":true,\"key\":true}"));
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[{\"key\":true},false,{\"key1\":true,\"key\":true},["
                      "null,false,[],true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__, string(Stringify(gv)) == string("true"));
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  gv = ObjGet(ArrayGet(v, 0), "key");
  cv = ArrayGet(v, 2);
  ObjAddFast(cv, gv);
  CheckRet(__func__, __LINE__, zj_SizeOf(ArrayGet(v, 0)) == 0);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("{\"key1\":true,\"key\":true}"));
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[{},false,{\"key1\":true,\"key\":true},[null,false,[],"
                      "true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 6);
  CheckRet(__func__, __LINE__, string(Stringify(gv)) == string(""));

  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  gv = ArrayGet(v, 0);
  cv = ArrayGet(v, 2);
  CheckRet(__func__, __LINE__, ObjAddFast(cv, gv) == false);
  CheckRet(__func__, __LINE__, ObjAdd(cv, gv) == false);
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void ValueArrayAddTest() {
  Allocator *A = NewAllocator();
  const char *json =
      "[{\"key\":true},false,[],[null,false,[],true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  Value *gv = ArrayGet(v, 0);
  Value *cv = ArrayGet(v, 2);
  ArrayAdd(cv, gv);
  CheckRet(__func__, __LINE__, zj_SizeOf(ArrayGet(v, 0)) == 1);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("[{\"key\":true}]"));
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[{\"key\":true},false,[{\"key\":true}],[null,false,[],"
                      "true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__,
           string(Stringify(gv)) == string("{\"key\":true}"));
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  gv = ArrayGet(v, 0);
  cv = ArrayGet(v, 2);
  ArrayAddFast(cv, gv);
  CheckRet(__func__, __LINE__, zj_SizeOf(ArrayGet(v, 0)) == 0);
  CheckRet(__func__, __LINE__,
           string(Stringify(cv)) == string("[{\"key\":true}]"));
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[false,[{\"key\":true}],[null,false,"
                                          "[],true],[\"\",123,\"str\"],null]"));
  CheckRet(__func__, __LINE__, zj_SizeOf(v) == 5);
  CheckRet(__func__, __LINE__, string(Stringify(gv)) == string(""));
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  gv = ArrayGet(v, 0);
  cv = ArrayGet(v, 1);
  CheckRet(__func__, __LINE__, ArrayAddFast(cv, gv) == false);
  CheckRet(__func__, __LINE__, ArrayAdd(cv, gv) == false);
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void DelTest() {
  Allocator *A = NewAllocator();
  const char *json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],"
                     "true],[\"\",123,\"str\"],null]";
  Value *v = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(v, json, strlen(json)) == true);
  ArrayDel(v, 0);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[false,{\"key1\":true},[null,false,["
                                          "],true],[\"\",123,\"str\"],null]"));
  ArrayDel(v, 4);
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) == string("[false,{\"key1\":true},[null,false,["
                                          "],true],[\"\",123,\"str\"]]"));
  ObjDel(ArrayGet(v, 1), "key1");
  CheckRet(__func__, __LINE__,
           string(Stringify(v)) ==
               string("[false,{},[null,false,[],true],[\"\",123,\"str\"]]"));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetAndSet(Value *srcv, Value *desv) {
  const JSONType *t;
  t = Type(srcv);
  switch (*t) {
  case JSONTypeArray: {
    SetArray(desv);
    Value *next = Begin(srcv);
    while (next != 0) {
      Value *v = NewValue(desv->A);
      GetAndSet(next, v);
      ArrayAddFast(desv, v);
      next = Next(next);
    }
  } break;
  case JSONTypeObject: {
    SetObj(desv);
    Value *next = Begin(srcv);
    while (next != 0) {
      Value *v = NewValue(desv->A);
      SetKeyFast(v, GetKey(next));
      GetAndSet(next, v);
      ObjAddFast(desv, v);
      next = Next(next);
    }
  } break;
  case JSONTypeBool: {
    const bool *b = GetBool(srcv);
    if (b == 0)
      return;
    SetBool(desv, *b);
  } break;
  case JSONTypeNull: {
    bool b;
    if (IsNull(srcv) == false)
      return;
    SetNull(desv);
  } break;
  case JSONTypeString: {
    const char *str = GetStr(srcv);
    if (str == 0)
      return;
    SetStrFast(desv, str);
  } break;
  case JSONTypeNumber: {
    const char *str = GetNumStr(srcv);
    if (str == 0)
      return;
    SetNumStrFast(desv, str);
  } break;
  }
}
void GetSetTest() {
  Allocator *A = NewAllocator();
  const char *json = "[{\"key\":true},false,{\"key1\":true},[null,false,[],"
                     "true],[\"\",123,\"str\"],null]";
  Value *srcv = NewValue(A);
  Value *desv = NewValue(A);
  CheckRet(__func__, __LINE__, ParseLen(srcv, json, strlen(json)) == true);
  GetAndSet(srcv, desv);
  CheckRet(__func__, __LINE__, string(Stringify(desv)) == string(json));
  ReleaseAllocator(A);
  CheckRet(__func__, __LINE__, AllocMemoryCount == FreeMemoryCount);
}
void GetSetTest1() {
  string data_path = "test/conformance/data/roundtrip/roundtrip";
  for (int i = 1; i <= 37; ++i) {
    stringstream ss;
    ss << data_path;
    if (i < 10)
      ss << "0";
    ss << i;
    ss << ".json";
    string json = Read(ss.str());
    Allocator *A = NewAllocator();
    Value *v = NewValue(A);
    bool ret = ParseFast(v, json.c_str());

    Value *vv = Copy(v);
    const char *ret_json = Stringify(vv);
    CheckRet(__func__, __LINE__, ret == true);
    CheckRet(__func__, __LINE__, ret_json != 0);
    CheckRet(__func__, __LINE__, string(json.c_str()) == string(ret_json));

    Value *vvv = NewValue(A);
    GetAndSet(vv, vvv);
    ret_json = Stringify(vvv);
    CheckRet(__func__, __LINE__, ret == true);
    CheckRet(__func__, __LINE__, ret_json != 0);
    CheckRet(__func__, __LINE__, string(json.c_str()) == string(ret_json));
    ReleaseAllocator(A);
  }
}
uint32_t RandomNum() {
  random_device rd;
  return rd();
}

string RandomStr() {
  string ret_str;
  int len = RandomNum() % 52 + 1;
  for (int i = 0; i < len; ++i) {
    if (RandomNum() % 2)
      ret_str += 'a' + RandomNum() % 26;
    else
      ret_str += 'A' + RandomNum() % 26;
  }
  return ret_str;
}

bool GenRandomValue(Value *v, int *len, int level, int max_len, int max_level) {
  if (level >= max_level) {
    return false;
  }
  if (*len >= max_len) {
    return false;
  }
  ++level;
  int type = RandomNum() % 7;
  if (*len == 0)
    type = 5 + type % 2;
  switch (type) {
  // null
  case 0:
    SetNull(v);
    break;
  // true
  case 1:
    SetBool(v, true);
    break;
  // false
  case 2:
    SetBool(v, false);
    break;
  // str
  case 3:
    SetStr(v, RandomStr().c_str());
    break;
  case 4:
    SetNum(v, RandomNum());
    break;
  // obj
  case 5: {
    SetObj(v);
    ++(*len);
    for (int i = 0; i < max_len; ++i) {
      Value *vv = NewValue(v->A);
      if (SetKey(vv, RandomStr().c_str()) == false) {
        CheckRet(__func__, __LINE__, false);
        return false;
      }
      if (GenRandomValue(vv, len, level, max_len, max_level) == true) {
        if (ObjAddFast(v, vv) == false) {
          CheckRet(__func__, __LINE__, false);
          return false;
        } else
          ++(*len);
      } else {
        return true;
      }
    }
    break;
  }
  // array
  case 6: {
    SetArray(v);
    ++(*len);
    for (int i = 0; i < max_len; ++i) {
      Value *vv = NewValue(v->A);
      if (GenRandomValue(vv, len, level, max_len, max_level) == true) {
        if (ArrayAddFast(v, vv) == false) {
          CheckRet(__func__, __LINE__, false);
          return false;
        } else
          ++(*len);
      } else {
        return true;
      }
    }
    break;
  } break;
  }
  return true;
}

void CoreTest() {
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
int main() {
  printf("==================%s==================\n", "function_test");
  MemoryTest();
  StringTest();
  SkinTest();
  PeekTest();
  ConsumeTest();
  PeekAndConsumeTest();
  ConsumetruefalseNullTest();
  HexCodePointTest();
  ConsumeHexTest();
  ConsumeStrTest();
  CheckStrTest();
  ConsumeNumTest();
  CheckNumTest();
  EscapeStrTest();
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
