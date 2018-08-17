#include "zzzjsonbeta_test.h"
#include "zzzjson_test.h"
#include "rapidjson_test.h"

using namespace std;

int main()
{
    string path = "test/performance/data/nativejson-benchmark/test.json";

    Test *zb_test = new zzzjsonbetaTest(path);
    Test *z_test = new zzzjsonTest(path);
    Test *r_test = new rapidjsonTest(path);

    int count = 1;
    for (int i = 0; i < count; ++i) {
        z_test->ParseTest();
    }
    for (int i = 0;i < count; ++i) {
        r_test->ParseTest();
    }


    for (int i = 0; i < count & 0; ++i)
    {
        if (i % 6 == 0)
        {
            z_test->ParseTest();
            zb_test->ParseTest();
            r_test->ParseTest();
        }
        else if (i % 6 == 1)
        {
            z_test->ParseTest();
            r_test->ParseTest();
            zb_test->ParseTest();
        }
        else if (i % 6 == 2)
        {
            zb_test->ParseTest();
            r_test->ParseTest();
            z_test->ParseTest();
        }
        else if (i % 6 == 3)
        {
            zb_test->ParseTest();
            z_test->ParseTest();
            r_test->ParseTest();
        }
        else if (i % 6 == 4)
        {
            r_test->ParseTest();
            z_test->ParseTest();
            zb_test->ParseTest();
        }
        else if (i % 6 == 5)
        {
            r_test->ParseTest();
            zb_test->ParseTest();
            z_test->ParseTest();
        }
    }

    z_test->PrintParse();
    zb_test->PrintParse();
    r_test->PrintParse();

    delete z_test;
    delete r_test;
    delete zb_test;
}