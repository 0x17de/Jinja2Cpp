#ifndef TEST_TOOLS_H
#define TEST_TOOLS_H

#include <gtest/gtest.h>
#include <jinja2cpp/reflected_value.h>

struct InputOutputPair
{
    std::string tpl;
    std::string result;

    friend std::ostream& operator << (std::ostream& os, const InputOutputPair& rp)
    {
        os << rp.tpl << " -> " << rp.result;
        return os;
    }
};

template<typename Tag>
class InputOutputPairTest : public ::testing::TestWithParam<InputOutputPair>
{
};

struct TestStruct
{
    int64_t intValue;
    double dblValue;
    bool boolValue;
    std::string strValue;
    std::wstring wstrValue;
};

inline jinja2::ValuesMap PrepareTestData()
{
    jinja2::ValuesList testData;
    for (int n = 0; n < 10; ++ n)
    {
        TestStruct s;
        std::ostringstream str;
        std::wostringstream wstr;

        str << "test string " << n;
        wstr << L"test string " << n;

        s.intValue = n;
        s.dblValue = static_cast<double>(n) / 2;
        s.boolValue = n % 2 == 1;
        s.strValue = str.str();
        s.wstrValue = wstr.str();

        testData.push_back(jinja2::Reflect(std::move(s)));
    }

    return jinja2::ValuesMap {
        {"intValue", 3},
        {"intList", jinja2::ValuesList{9, 0, 8, 1, 7, 2, 6, 3, 5, 4}},
        {"doubleValue", 12.123f},
        {"doubleList", jinja2::ValuesList{9.5, 0.5, 8.5, 1.5, 7.5, 2.5, 6.4, 3.8, 5.2, -4.7}},
        {"stringValue", "rain"},
        {"stringList", jinja2::ValuesList{"string9", "string0", "string8", "string1", "string7", "string2", "string6", "string3", "string5", "string4"}},
        {"boolFalseValue", false},
        {"boolTrueValue", true},
        {"mapValue", jinja2::ValuesMap{
                {"intVal", 10},
                {"dblVal", 100.5},
                {"stringVal", "string100.5"},
                {"boolValue", true},
                {"reflectedList", testData}
            }},
        {"reflectedList", std::move(testData)}
    };
}

namespace jinja2
{
template<>
struct TypeReflection<TestStruct> : TypeReflected<TestStruct>
{
    static auto& GetAccessors()
    {
        static std::unordered_map<std::string, FieldAccessor> accessors = {
            {"intValue", [](const TestStruct& obj) {return obj.intValue;}},
            {"dblValue", [](const TestStruct& obj) {return obj.dblValue;}},
            {"boolValue", [](const TestStruct& obj) { return obj.boolValue;}},
            {"strValue", [](const TestStruct& obj) {return obj.strValue;}},
            {"wstrValue", [](const TestStruct& obj) {return obj.wstrValue;}},
        };

        return accessors;
    }
};
} // jinja2

#endif // TEST_TOOLS_H