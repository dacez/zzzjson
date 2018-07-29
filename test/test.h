#ifndef TEST_H
#define TEST_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>

static int SuccCount = 0;
static int FailCount = 0;

static void CheckRet(const char *func, unsigned int line, bool ret)
{
    if (ret == false)
    {
        std::cout << "\033[31m" << func << "(" << line << "):fail"
                  << "\033[37m" << std::endl;
        ++FailCount;
    }
    else
    {
        ++SuccCount;
    }
}

static void PrintResult()
{
    std::cout << "\033[32m"
              << "SuccCount:" << SuccCount << "\033[37m" << std::endl;
    if (FailCount == 0)
        std::cout << "\033[32m"
                  << "FailCount:" << FailCount << "\033[37m" << std::endl;
    else
        std::cout << "\033[31m"
                  << "FailCount:" << FailCount << "\033[37m" << std::endl;
}

static uint64_t Now()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

static std::string Read(const std::string &path)
{
    std::string ret_str;
    std::fstream f(path.c_str());
    if (f.is_open() == false)
        return "";
    std::streampos size = f.rdbuf()->pubseekoff(0, f.end);
    if (size <= 0)
        return "";
    char *buf = new char[size];
    f.rdbuf()->pubseekoff(0, f.beg);
    f.rdbuf()->sgetn(buf, size);
    f.close();
    ret_str.assign(buf, size);
    delete[] buf;
    return ret_str;
}

class Test
{
  public:
    Test(const std::string &name, const std::string &path)
    {
        m_name = name;
        m_path = path;

        std::string json = Read(path);
        m_json = json;

        m_parse_time = 0;
        m_stringify_time = 0;
        m_all_time = 0;

        m_parse_status = true;
        m_stringify_status = true;
        m_all_status = true;
    }
    virtual bool Parse(const char *json, unsigned long long *ms)
    {
        m_parse_status = false;
        return false;
    }
    virtual bool Stringify(const char *json, unsigned long long *ms)
    {
        m_stringify_status = false;
        return false;
    }
    virtual bool All(const char *json, unsigned long long *ms)
    {
        m_all_status = false;
        return false;
    }
    bool ParseTest()
    {
        unsigned long long ms = 0;
        if (Parse(m_json.c_str(), &ms) == false)
        {
            m_parse_status = false;
        }
        m_parse_time += ms;

        return true;
    }
    bool StringifyTest()
    {
        unsigned long long ms = 0;
        if (Stringify(m_json.c_str(), &ms) == false)
        {
            m_stringify_status = false;
        }
        m_stringify_time += ms;

        return true;
    }
    bool AllTest()
    {
        unsigned long long ms = 0;
        if (All(m_json.c_str(), &ms) == false)
        {
            m_all_status = false;
        }
        m_all_time += ms;

        return true;
    }
    void printStr(const std::string &s)
    {
        printf("%s", s.c_str());
        for (int i = s.size(); i < 32; ++i)
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
    void printName(const std::string &path) {
        std::string name;
        for (int i = 0; i < path.size(); ++i) {
            if (path[i] == '/') {
                name = "";
            } else {
                name += path[i];
            }
        }
        printStr(name);
    }
    void PrintParse()
    {
        printStr(m_name);
        printStr("Parse");
        printName(m_path);
        if (m_parse_status)
            printStr(m_parse_time);
        else
            printStr("Fail");
        printf("\n");
    }
    void PrintStringify()
    {
        printStr(m_name);
        printStr("Stringify");
        printName(m_path);
        if (m_stringify_status)
            printStr(m_stringify_time);
        else
            printStr("Fail");
        printf("\n");
    }
    void PrintAll()
    {
        printStr(m_name);
        printStr("All");
        printName(m_path);
        if (m_all_status)
            printStr(m_all_time);
        else
            printStr("Fail");
        printf("\n");
    }
    std::string Name() {
        return m_name;
    }

  private:
    std::string m_name;
    std::string m_json;
    std::string m_path;
    unsigned long long m_parse_time;
    unsigned long long m_stringify_time;
    unsigned long long m_all_time;
    bool m_parse_status;
    bool m_stringify_status;
    bool m_all_status;
};
#endif