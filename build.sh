#/bin/bash
CC=g++
#CC=clang++

function CompilePerformance()
{
    rm -rf build
    mkdir build

    $CC -c -O3 -o build/jsoncpp_reader.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_reader.cpp -std=c++11;
    $CC -c -O3 -o build/json_value.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_value.cpp -std=c++11;
    $CC -c -O3 -o build/json_writer.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_writer.cpp -std=c++11;

    $CC -c -O3 -o build/cjson.o -Itest/performance/c++/cJSON test/performance/c++/cJSON/cJSON.c -std=c++11;

    $CC -c -O3 -o build/gason.o -Itest/performance/c++/gason/src test/performance/c++/gason/src/gason.cpp -std=c++11;

    $CC -c -O3 -o build/parson.o -Itest/performance/c++/parson test/performance/c++/parson/parson.c;

    $CC -o performance_test test/performance/c++/performance_test.cpp build/* -std=c++11 -O3 -g;
}

function PerformanceTestOne()
{
    files=$3
    for f in ${files[*]}; do
        ./performance_test $1 parse $f >> $2
        ./performance_test $1 stringify $f >> $2
        ./performance_test $1 all $f >> $2
        echo -n .
    done
}

function PerformanceNoNum()
{
    output=nonum.txt
    files=(nonum.json nonum1.json nonum2.json)
    rm -rf $output
    PerformanceTestOne zzzjson $output ${files[*]}
    PerformanceTestOne rapidjson $output ${files[*]}
    PerformanceTestOne rapidjsonfp $output ${files[*]}
    PerformanceTestOne rapidjsonstr $output ${files[*]}
#    PerformanceTestOne arduinojson $output ${files[*]}
    PerformanceTestOne cjson $output ${files[*]}
    PerformanceTestOne gason $output ${files[*]}
    PerformanceTestOne picojson $output ${files[*]}
#    PerformanceTestOne parson $output ${files[*]}
    PerformanceTestOne jsoncpp $output ${files[*]}
    PerformanceTestOne nlohmannjson $output ${files[*]}
    PerformanceTestOne taocppjson $output ${files[*]}
}

function PerformanceNativeJSONBenchmark()
{
    output=nativejsonbenchmark.txt
    files=(canada.json citm_catalog.json twitter.json)
    rm -rf $output
    PerformanceTestOne zzzjson $output ${files[*]}
    PerformanceTestOne rapidjson $output ${files[*]}
    PerformanceTestOne rapidjsonfp $output ${files[*]}
    PerformanceTestOne rapidjsonstr $output ${files[*]}
    PerformanceTestOne arduinojson $output ${files[*]}
    PerformanceTestOne cjson $output ${files[*]}
    PerformanceTestOne gason $output ${files[*]}
    PerformanceTestOne picojson $output ${files[*]}
    PerformanceTestOne parson $output ${files[*]}
    PerformanceTestOne jsoncpp $output ${files[*]}
    PerformanceTestOne nlohmannjson $output ${files[*]}
    PerformanceTestOne taocppjson $output ${files[*]}
}

function PerformanceTaoBao()
{
    output=taobao.txt
    files=(cart.json trade.json)
    rm -rf $output
    PerformanceTestOne zzzjson $output ${files[*]}
    PerformanceTestOne rapidjson $output ${files[*]}
    PerformanceTestOne rapidjsonfp $output ${files[*]}
    PerformanceTestOne rapidjsonstr $output ${files[*]}
    PerformanceTestOne arduinojson $output ${files[*]}
    PerformanceTestOne cjson $output ${files[*]}
    PerformanceTestOne gason $output ${files[*]}
    PerformanceTestOne picojson $output ${files[*]}
    PerformanceTestOne parson $output ${files[*]}
    PerformanceTestOne jsoncpp $output ${files[*]}
    PerformanceTestOne nlohmannjson $output ${files[*]}
    PerformanceTestOne taocppjson $output ${files[*]}
}

function PerformanceNormal()
{
    output=normal.txt
    files=(canada.json citm_catalog.json twitter.json cart.json trade.json large-file.json)
    rm -rf $output
    PerformanceTestOne zzzjson $output ${files[*]}
    PerformanceTestOne rapidjson $output ${files[*]}
    PerformanceTestOne rapidjsonfp $output ${files[*]}
    PerformanceTestOne rapidjsonstr $output ${files[*]}
    PerformanceTestOne arduinojson $output ${files[*]}
    PerformanceTestOne cjson $output ${files[*]}
    PerformanceTestOne gason $output ${files[*]}
    PerformanceTestOne picojson $output ${files[*]}
    PerformanceTestOne parson $output ${files[*]}
    PerformanceTestOne jsoncpp $output ${files[*]}
    PerformanceTestOne nlohmannjson $output ${files[*]}
    PerformanceTestOne taocppjson $output ${files[*]}
}

function PerformanceTest
{
    PerformanceNoNum
    PerformanceNativeJSONBenchmark
    PerformanceTaoBao
    PerformanceNormal
}

if [ "$1" == "function_test" ];then
    rm -rf function_test
    $CC -o function_test test/function/function_test.cpp --std=c++11 -g -O0

elif [ "$1" == "conformance_test" ];then
    rm -rf conformance_test
    $CC -o conformance_test test/conformance/conformance_test.cpp --std=c++11 -g -O0

elif [ "$1" == "performance_test" ];then
    CompilePerformance

elif [ "$1" == "all" ];then

    $CC -o function_test test/function/function_test.cpp --std=c++11 -g -O3

    $CC -o conformance_test test/conformance/conformance_test.cpp --std=c++11 -g -O3
    
    CompilePerformance

    ./function_test

    ./conformance_test

    PerformanceTest

    ./performance_test statistic

fi