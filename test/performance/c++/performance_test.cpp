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

using namespace std;

void RunTest(const string &test_name, int count, const vector<string> &paths)
{
    printf("%s:\n", test_name.c_str());
    vector<Test *> tests;
    tests.push_back(new strlenTest(paths));
    if (test_name != "PureStringTest") tests.push_back(new ArduinoJSONTest(paths));
    tests.push_back(new cJSONTest(paths));
    tests.push_back(new gasonTest(paths));
    tests.push_back(new picoJSONTest(paths));
    if (test_name != "PureStringTest") tests.push_back(new parsonTest(paths));
    tests.push_back(new JSONCPPTest(paths));
    tests.push_back(new nlohmannJSONTest(paths));
    tests.push_back(new taocppJSONTest(paths));
    tests.push_back(new rapidJSONTest(paths));
    tests.push_back(new rapidJSONFPTest(paths));
    tests.push_back(new rapidJSONSTRTest(paths));
    tests.push_back(new zzzJSONTest(paths));

    for (int c = 0; c < count; ++c)
    {
        for (int i = 0; i < tests.size(); ++i)
            tests[i]->AllTest();
        for (int i = 0; i < tests.size(); ++i)
            tests[i]->ParseTest();
        for (int i = 0; i < tests.size(); ++i)
            tests[i]->StringifyTest();
    }

    printf("%s\n", "| Name            | Parse           | Stringify       | All              |");
    for (int i = 0; i < tests.size(); ++i)
        tests[i]->PrintReslut();
}

int main()
{
    printf("==========================%s==========================\n", "performance_test");

    {
        int purestring_count = 1;
        vector<string> purestring_paths;

        purestring_paths.push_back("test/performance/data/nonum.json");
        RunTest("PureStringTest", purestring_count, purestring_paths);
    }

    {
        int nativejsonbenchmark_count = 1;
        vector<string> nativejsonbenchmark_paths;

        nativejsonbenchmark_paths.push_back("test/performance/data/nativejson-benchmark/canada.json");
        nativejsonbenchmark_paths.push_back("test/performance/data/nativejson-benchmark/citm_catalog.json");
        nativejsonbenchmark_paths.push_back("test/performance/data/nativejson-benchmark/twitter.json");

        RunTest("NativeJSONBenchMarkTest", nativejsonbenchmark_count, nativejsonbenchmark_paths);
    }

    {
        int taobao_count = 1;
        vector<string> taobao_paths;

        taobao_paths.push_back("test/performance/data/fastjson/taobao/cart.json");
        taobao_paths.push_back("test/performance/data/fastjson/trade.json");

        RunTest("taobaoTest", taobao_count, taobao_paths);
    }

    {
        int normaljson_count = 1;
        vector<string> normaljson_paths;

        normaljson_paths.push_back("test/performance/data/nativejson-benchmark/canada.json");
        normaljson_paths.push_back("test/performance/data/nativejson-benchmark/citm_catalog.json");
        normaljson_paths.push_back("test/performance/data/nativejson-benchmark/twitter.json");

        normaljson_paths.push_back("test/performance/data/json-iterator/large-file.json");

        normaljson_paths.push_back("test/performance/data/fastjson/taobao/cart.json");
        normaljson_paths.push_back("test/performance/data/fastjson/trade.json");

        RunTest("NormalJSONTest", normaljson_count, normaljson_paths);
    }
    return 0;
}