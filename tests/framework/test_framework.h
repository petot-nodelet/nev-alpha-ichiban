#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
namespace nv::test {
struct TestCase { std::string name; std::function<void()> func; };
class TestRunner {
public:
    static TestRunner& instance() { static TestRunner runner; return runner; }
    void addTest(std::string name, std::function<void()> func) { tests_.push_back({std::move(name), std::move(func)}); }
    int run() {
        int passed = 0, failed = 0;
        std::cout << "[TEST] Running " << tests_.size() << " test(s)...\n";
        for (const auto& t : tests_) {
            currentTestFailed_ = false;
            try { t.func(); if (!currentTestFailed_) { std::cout << "  ✓ " << t.name << "\n"; passed++; } else { std::cout << "  ✗ " << t.name << "\n"; failed++; } }
            catch (const std::exception& e) { std::cout << "  ✗ " << t.name << " (exception: " << e.what() << ")\n"; failed++; }
            catch (...) { std::cout << "  ✗ " << t.name << " (unknown exception)\n"; failed++; }
        }
        std::cout << "\n[TEST] " << passed << " passed, " << failed << " failed, " << tests_.size() << " total\n";
        return failed == 0 ? 0 : 1;
    }
    void fail(const std::string& msg, const char* file, int line) {
        std::cerr << "    FAIL at " << file << ":" << line << " — " << msg << "\n"; currentTestFailed_ = true;
    }
private:
    std::vector<TestCase> tests_; bool currentTestFailed_ = false;
};
struct TestRegistrar { TestRegistrar(std::string name, std::function<void()> func) { TestRunner::instance().addTest(std::move(name), std::move(func)); } };
}
#define NV_TEST_CASE(name) static void nv_test_func_##name(); static nv::test::TestRegistrar nv_test_reg_##name(#name, nv_test_func_##name); static void nv_test_func_##name()
#define NV_ASSERT(cond) do { if (!(cond)) { nv::test::TestRunner::instance().fail(std::string("Assertion failed: ") + #cond, __FILE__, __LINE__); return; } } while (0)
#define NV_ASSERT_EQ(a, b) do { auto va_ = (a); auto vb_ = (b); if (!(va_ == vb_)) { std::ostringstream oss_; oss_ << "Expected " << #a << " == " << #b << ", got " << va_ << " vs " << vb_; nv::test::TestRunner::instance().fail(oss_.str(), __FILE__, __LINE__); return; } } while (0)
