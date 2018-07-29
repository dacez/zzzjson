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

using namespace std;

void Init(const string &path, vector<Test *> *tests) {
    tests->push_back(new strlenTest(path));
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

void Free(vector<Test *> *tests) {
    for (int i = 0; i < tests->size(); ++i) {
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

vector<string> GetLines(const string &path) {
    string c = Read(path);
    vector<string> ls;
    string l;
    for (int i = 0; i < c.size(); ++i) {
        if (c[i] == '\n') {
            if (l != "") {
                ls.push_back(l);
                l = "";
            }
        } else {
            l += c[i];
        }
    }
    return ls;
}

vector<string> GetWords(const string &line) {
    vector<string> ws;
    string w;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == '\t' || line[i] == ' ') {
            if (w != "") {
                ws.push_back(w);
                w = "";
            }
        } else {
            w += line[i];
        }
    }
    return ws;
}
void printStr(const std::string &s)
    {
        printf("%s", s.c_str());
        for (int i = s.size(); i < 16; ++i)
        {
            printf(" ");
        }
        printf("\t");
    }
void printStr(unsigned long long n)
    {
        std::stringstream ss;
        ss << n;
        std::string s;
        ss >> s;
        printStr(s);
    }
void PrintResult(const string &name, const string &path) {
    map<string, map<string, int> > m;
    auto ls = GetLines(path);
    for (const auto &l : ls) {
        auto ws = GetWords(l);
        if (ws.size() != 4) {
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
    for (const auto &t : m) {
        printf("|");
        printStr(t.first);
        auto mm = t.second;
        printf("|");
        printStr(mm["Parse"]);
        printf("|");
        printStr(mm["Stringify"]);
        printf("|");
        printStr(mm["All"]);
        printf("|\n");
    }
}

int main(int argc, char **argv)
{
    if (argc == 2 && string(argv[1]) == "statistic") {
        printf("==================performance_test==================\n");
        PrintResult("NoNumTest", "nonum.txt");
        PrintResult("NativeJSONBenchMark", "nativejsonbenchmark.txt");
        PrintResult("TaoBao", "taobao.txt");
        PrintResult("Normal", "normal.txt");
        return 0;
    } else if (argc != 4) {
        printf("performance_test name type file\n");
        return 1;
    }


    string name = argv[1];
    string type = argv[2];
    string file = argv[3];

    vector<string> paths = InitPath();

    string path;
    for (int i = 0; i < paths.size(); ++i) {
        if (paths[i].find(file) != string::npos) {path = paths[i];break;}
    }

    if (path == "") {
        printf("No File %s\n", file.c_str());
        return 1;
    }

    vector<Test *> tests;
    Init(path, &tests);

    Test *t = 0;
    for (int i = 0; i < tests.size(); ++i) {
        if (tests[i]->Name() == name) {
            t = tests[i];
            break;
        }
    }

    if (t == 0) {
        printf("No Test %s\n", name.c_str());
        return 1;        
    }

    if (type == "parse") {
        t->ParseTest();
        t->PrintParse();
    } else if (type == "stringify") {
        t->StringifyTest();
        t->PrintStringify();
    } else if (type == "all") {
        t->AllTest();
        t->PrintAll();
    }

    Free(&tests);
    return 0;
}