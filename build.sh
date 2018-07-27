#/bin/bash
CC=g++
#CC=clang++

rm -rf build
mkdir build

if [ "$1" == "function_test" ];then
    $CC -o function_test test/function/function_test.cpp --std=c++11 -g -O0

elif [ "$1" == "conformance_test" ];then
    $CC -o conformance_test test/conformance/conformance_test.cpp --std=c++11 -g -O0

elif [ "$1" == "performance_test" ];then

    $CC -c -O3 -o build/jsoncpp_reader.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_reader.cpp -std=c++11;
    $CC -c -O3 -o build/json_value.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_value.cpp -std=c++11;
    $CC -c -O3 -o build/json_writer.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_writer.cpp -std=c++11;

    $CC -c -O3 -o build/cjson.o -Itest/performance/c++/cJSON test/performance/c++/cJSON/cJSON.c -std=c++11;

    $CC -c -O3 -o build/gason.o -Itest/performance/c++/gason/src test/performance/c++/gason/src/gason.cpp -std=c++11;

    $CC -c -O3 -o build/parson.o -Itest/performance/c++/parson test/performance/c++/parson/parson.c;

    $CC -o performance_test test/performance/c++/performance_test.cpp build/* -std=c++11 -O3 -g;

elif [ "$1" == "all" ];then


    $CC -o function_test test/function/function_test.cpp --std=c++11 -g -O3


    $CC -o conformance_test test/conformance/conformance_test.cpp --std=c++11 -g -O3
    

    $CC -c -O3 -o build/jsoncpp_reader.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_reader.cpp -std=c++11;
    $CC -c -O3 -o build/json_value.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_value.cpp -std=c++11;
    $CC -c -O3 -o build/json_writer.o -Itest/performance/c++/jsoncpp/include test/performance/c++/jsoncpp/src/lib_json/json_writer.cpp -std=c++11;

    $CC -c -O3 -o build/cjson.o -Itest/performance/c++/cJSON test/performance/c++/cJSON/cJSON.c -std=c++11;

    $CC -c -O3 -o build/gason.o -Itest/performance/c++/gason/src test/performance/c++/gason/src/gason.cpp -std=c++11;

    $CC -c -O2 -o build/parson.o -Itest/performance/c++/parson test/performance/c++/parson/parson.c;

    $CC -o performance_test test/performance/c++/performance_test.cpp build/* -std=c++11 -O3 -g;

    ./function_test
    ./conformance_test
    ./performance_test

fi
