#include "arduinojson_test.h"
#include "zzzjson_test.h"
#include "rapidjson_test.h"
#include "rapidjsonfp_test.h"
#include "rapidjsonstr_test.h"
#include "jsoncpp_test.h"
#include "cjson_test.h"
#include "gason_test.h"
#include "picojson_test.h"
#include "parson_test.h"
#include "nlohmannjson_test.h"
#include "taocppjson_test.h"
#include "strlen_test.h"

#include <map>
#include <random>

using namespace std;

void Init(const string &path, vector<Test *> *tests)
{
    tests->push_back(new zzzjsonTest(path));
    tests->push_back(new rapidjsonTest(path));
    tests->push_back(new rapidjsonfpTest(path));
    tests->push_back(new rapidjsonstrTest(path));
    tests->push_back(new arduinojsonTest(path));
    tests->push_back(new cjsonTest(path));
    tests->push_back(new gasonTest(path));
    tests->push_back(new picojsonTest(path));
    tests->push_back(new parsonTest(path));
    tests->push_back(new jsoncppTest(path));
    tests->push_back(new nlohmannjsonTest(path));
    tests->push_back(new taocppjsonTest(path));
}

void Init(const vector<string> &jsons, vector<Test *> *tests)
{
    tests->push_back(new zzzjsonTest(jsons));
    tests->push_back(new rapidjsonTest(jsons));
    tests->push_back(new rapidjsonfpTest(jsons));
    tests->push_back(new rapidjsonstrTest(jsons));
//    tests->push_back(new arduinojsonTest(jsons));
    tests->push_back(new cjsonTest(jsons));
    //tests->push_back(new gasonTest(jsons));
    tests->push_back(new picojsonTest(jsons));
    //tests->push_back(new parsonTest(jsons));
    tests->push_back(new jsoncppTest(jsons));
    tests->push_back(new nlohmannjsonTest(jsons));
    tests->push_back(new taocppjsonTest(jsons));
}

void Free(vector<Test *> *tests)
{
    for (int i = 0; i < tests->size(); ++i)
    {
        delete (*tests)[i];
    }
}

vector<string> InitPath()
{
    vector<string> paths;
    paths.push_back("test/performance/data/nonum.json");
    paths.push_back("test/performance/data/nonum1.json");
    paths.push_back("test/performance/data/nonum2.json");
    paths.push_back("test/performance/data/nativejson-benchmark/canada.json");
    paths.push_back("test/performance/data/nativejson-benchmark/citm_catalog.json");
    paths.push_back("test/performance/data/nativejson-benchmark/twitter.json");
    paths.push_back("test/performance/data/fastjson/taobao/cart.json");
    paths.push_back("test/performance/data/fastjson/trade.json");
    paths.push_back("test/performance/data/json-iterator/large-file.json");
    return paths;
}

vector<string> GetLines(const string &path)
{
    string c = Read(path);
    vector<string> ls;
    string l;
    for (int i = 0; i < c.size(); ++i)
    {
        if (c[i] == '\n')
        {
            if (l != "")
            {
                ls.push_back(l);
                l = "";
            }
        }
        else
        {
            l += c[i];
        }
    }
    return ls;
}

vector<string> GetWords(const string &line)
{
    vector<string> ws;
    string w;
    for (int i = 0; i < line.size(); ++i)
    {
        if (line[i] == '\t' || line[i] == ' ')
        {
            if (w != "")
            {
                ws.push_back(w);
                w = "";
            }
        }
        else
        {
            w += line[i];
        }
    }
    return ws;
}

void PrintResult(const string &name, const string &path)
{
    map<string, map<string, int>> m;
    auto ls = GetLines(path);
    for (const auto &l : ls)
    {
        auto ws = GetWords(l);
        if (ws.size() != 4)
        {
            printf("Fail\n");
            return;
        }
        stringstream ss;
        ss << ws[3];
        unsigned long long ms;
        ss >> ms;
        m[ws[0]][ws[1]] += ms;
    }
    printf("=============================%s=============================\n", name.c_str());
    for (const auto &t : m)
    {
        printf("|");
        PrintStr(t.first, 16);
        auto mm = t.second;
        printf("|");
        PrintStr(mm["Parse"], 16);
        printf("|");
        PrintStr(mm["Stringify"], 16);
        printf("|");
        PrintStr(mm["All"], 16);
        printf("|\n");
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

bool GenRandomValue(zzz_Value *v, int *len, int level, int max_len, int max_level)
{
    if (level >= max_level)
    {
        return false;
    }
    if (*len >= max_len)
    {
        return false;
    }
    ++level;
    int type = RandomNum() % 7;
    if (*len == 0)
        type = 5 + type % 2;
    switch (type)
    {
    // null
    case 0:
        zzz_ValueSetNull(v);
        break;
    // true
    case 1:
        zzz_ValueSetBool(v, true);
        break;
    // false
    case 2:
        zzz_ValueSetBool(v, false);
        break;
    // str
    case 3:
        zzz_ValueSetStr(v, RandomStr().c_str());
        break;
    case 4:
        zzz_ValueSetNum(v, RandomNum());
        break;
    // obj
    case 5:
    {
        zzz_ValueSetObj(v);
        ++(*len);
        for (int i = 0; i < max_len; ++i)
        {
            zzz_Value *vv = zzz_ValueNew(v->A);
            if (zzz_ValueSetKey(vv, RandomStr().c_str()) == false)
            {
                return false;
            }
            if (GenRandomValue(vv, len, level, max_len, max_level) == true)
            {
                if (zzz_ValueObjAddFast(v, vv) == false)
                {
                    return false;
                }
                else
                    ++(*len);
            }
            else
            {
                return true;
            }
        }
        break;
    }
    // array
    case 6:
    {
        zzz_ValueSetArray(v);
        ++(*len);
        for (int i = 0; i < max_len; ++i)
        {
            zzz_Value *vv = zzz_ValueNew(v->A);
            if (GenRandomValue(vv, len, level, max_len, max_level) == true)
            {
                if (zzz_ValueArrayAddFast(v, vv) == false)
                {
                    return false;
                }
                else
                    ++(*len);
            }
            else
            {
                return true;
            }
        }
        break;
    }
    break;
    }
    return true;
}

void GenRandomValue(zzz_Value *v, int max_len, int max_level)
{
    int len = 0;
    int level = 0;
    GenRandomValue(v, &len, level, max_len, max_level);
}

vector<string> Gen10000ShortJSON() {
    vector<string> ret_jsons;
    for (int i = 0; i < 10000; ++i) {
        zzz_Allocator *a = zzz_AllocatorNew();
        zzz_Value *v = zzz_ValueNew(a);
        GenRandomValue(v, 50, 10);
        const char *json = zzz_ValueStringify(v);
        if (json != 0) ret_jsons.push_back(json);
        zzz_AllocatorRelease(a);
    }
    return ret_jsons;
}

vector<string> Gen10000LongJSON() {
    vector<string> ret_jsons;
    for (int i = 0; i < 100; ++i) {
        zzz_Allocator *a = zzz_AllocatorNew();
        zzz_Value *v = zzz_ValueNew(a);
        GenRandomValue(v, 5000, 100);
        const char *json = zzz_ValueStringify(v);
        if (json != 0) ret_jsons.push_back(json);
        zzz_AllocatorRelease(a);
    }
    return ret_jsons;
}

void RandomShortJSONTest() {
    auto short_jsons = Gen10000ShortJSON();
    vector<Test *> tests;
    Init(short_jsons, &tests);

    for (const auto & t : tests) {
        t->ParseTest();
        t->StringifyTest();
        t->AllTest();

        t->PrintParse();
        t->PrintStringify();
        t->PrintAll();
    }

    Free(&tests);
}

void RandomLongJSONTest() {
    auto long_jsons = Gen10000LongJSON();
    vector<Test *> tests;
    Init(long_jsons, &tests);

    for (const auto & t : tests) {
        t->ParseTest();
        t->StringifyTest();
        t->AllTest();

        t->PrintParse();
        t->PrintStringify();
        t->PrintAll();
    }

    Free(&tests);
}

int main(int argc, char **argv)
{
    if (argc == 2 && string(argv[1]) == "statistic")
    {
        printf("\n==================performance_test==================\n");
        PrintResult("NoNumTest", "nonum.txt");
        PrintResult("NativeJSONBenchMark", "nativejsonbenchmark.txt");
        PrintResult("TaoBao", "taobao.txt");
        PrintResult("Normal", "normal.txt");
        PrintResult("RandomShort", "randomshortjsontest.txt");
        PrintResult("RandomLong", "randomlongjsontest.txt");
        return 0;
    } else if (argc == 2 && string(argv[1]) == "randomshortjsontest") {
        RandomShortJSONTest();
        return 0;
    } else if (argc == 2 && string(argv[1]) == "randomlongjsontest") {
        RandomLongJSONTest();
        return 0;
    } else if (argc != 4)
    {
        printf("performance_test name type file\n");
        return 1;
    }

    string name = argv[1];
    string type = argv[2];
    string file = argv[3];

    vector<string> paths = InitPath();

    string path;
    for (int i = 0; i < paths.size(); ++i)
    {
        if (paths[i].find(file) != string::npos)
        {
            path = paths[i];
            break;
        }
    }

    if (path == "")
    {
        printf("No File %s\n", file.c_str());
        return 1;
    }

    vector<Test *> tests;
    Init(path, &tests);

    Test *t = 0;
    for (int i = 0; i < tests.size(); ++i)
    {
        if (tests[i]->Name() == name)
        {
            t = tests[i];
            break;
        }
    }

    if (t == 0)
    {
        printf("No Test %s\n", name.c_str());
        return 1;
    }

    if (type == "parse")
    {
        t->ParseTest();
        t->PrintParse();
    }
    else if (type == "stringify")
    {
        t->StringifyTest();
        t->PrintStringify();
    }
    else if (type == "all")
    {
        t->AllTest();
        t->PrintAll();
    }

    Free(&tests);
    return 0;
}