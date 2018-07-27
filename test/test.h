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
//        std::cout << "\033[32m" << func << "(" << line << "):succ"
//                  << "\033[37m" << std::endl;
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
    Test(const std::string &name, const std::vector<std::string> &paths)
    {
        m_name = name;
        for (int i = 0; i < paths.size(); ++i)
        {
            std::string json = Read(paths[i]);
            m_jsons.push_back(json);
        }
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
        for (int i = 0; i < m_jsons.size(); ++i)
        {
            unsigned long long ms = 0;
            if (Parse(m_jsons[i].c_str(), &ms) == false)
            {
                m_parse_status = false;
                break;
            }
            m_parse_time += ms;
        }

        return true;
    }
    bool StringifyTest()
    {

        for (int i = 0; i < m_jsons.size(); ++i)
        {
            unsigned long long ms = 0;
            if (Stringify(m_jsons[i].c_str(), &ms) == false)
            {
                m_stringify_status = false;
                break;
            }
            m_stringify_time += ms;
        }

        return true;
    }
    bool AllTest()
    {

        for (int i = 0; i < m_jsons.size(); ++i)
        {
            unsigned long long ms = 0;
            if (All(m_jsons[i].c_str(), &ms) == false)
            {
                m_all_status = false;
                break;
            }
            m_all_time += ms;
        }

        return true;
    }
    void PrintParseTest()
    {
        std::cout << m_name;
        for (int i = m_name.size(); i < 16; ++i)
            std::cout << " ";
        std::cout << "\t";
        if (m_parse_status == false)
        {
            std::cout << "NoTest" << std::endl;
        }
        else
        {
            std::cout << m_parse_time << "\tms" << std::endl;
        }
    }
    void PrintStringifyTest()
    {
        std::cout << m_name;
        for (int i = m_name.size(); i < 16; ++i)
            std::cout << " ";
        std::cout << "\t";
        if (m_stringify_status == false)
        {
            std::cout << "NoTest" << std::endl;
        }
        else
        {
            std::cout << m_stringify_time << "\tms" << std::endl;
        }
    }
    void PrintAllTest()
    {
        std::cout << m_name;
        for (int i = m_name.size(); i < 16; ++i)
            std::cout << " ";
        std::cout << "\t";
        if (m_all_status == false)
        {
            std::cout << "NoTest" << std::endl;
        }
        else
        {
            std::cout << m_all_time << "\tms" << std::endl;
        }
    }
    void PrintName() {
        printf("| ");
        printf("%s", m_name.c_str());
        for (int i = m_name.size(); i < 16; ++i) printf(" ");
    }
    void PrintNum(unsigned long long n) {
        std::stringstream ss;
        ss << n;
        std::string num;
        ss >> num;
        printf("| ");
        printf("%s", num.c_str());
        for (int i = num.size(); i < 16; ++i) printf(" ");
    }
    void PrintReslut() {
        PrintName();
        PrintNum(m_parse_time);
        PrintNum(m_stringify_time);
        PrintNum(m_all_time);
        printf(" |\n");
    }

  private:
    std::string m_name;
    std::vector<std::string> m_jsons;
    unsigned long long m_parse_time;
    unsigned long long m_stringify_time;
    unsigned long long m_all_time;
    bool m_parse_status;
    bool m_stringify_status;
    bool m_all_status;
};
#endif