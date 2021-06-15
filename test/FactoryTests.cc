#include <gtest/gtest.h>
#include <Utils.h>

#include <string>
#include <cctype>
#include <algorithm>

struct StrBase {
  StrBase(const std::string& str)
    : m_str(str) {}

  virtual std::string msg() = 0;

  const std::string m_str;
};
static ro::ObjectFromStrFactory<StrBase> s_StrBaseFactory;
using StrBaseCreator = ro::ObjectFromStrFactory<StrBase>::ObjectCreator;
using StrBasePtr = ro::ObjectFromStrFactory<StrBase>::ObjectPtr;

struct ToLower : public StrBase {
  ToLower(const std::string& str)
    : StrBase(str) {}

  virtual std::string msg() override {
    std::string strCopy(m_str);
    std::transform(strCopy.begin(), strCopy.end(), strCopy.begin(),
		   [](unsigned char c){ return std::tolower(c); });
    return strCopy;
  }
};

struct Raw : public StrBase {
  Raw(const std::string& str)
    : StrBase(str) {}

  virtual std::string msg() override {
    return m_str;
  }
};

StrBasePtr createToLower(const std::string& cfg) { return( StrBasePtr(new ToLower(cfg)) ); }
StrBasePtr createRaw(const std::string& cfg) { return( StrBasePtr(new Raw(cfg)) ); }

// Demonstrate some basic assertions.
TEST(RegisterTest, FactoryTests) {
  s_StrBaseFactory.registerCreator("raw"    , createRaw);
  s_StrBaseFactory.registerCreator("tolower", createToLower);

  std::string   rawStr("ThisIsARawString");
  std::string lowerStr("thisisarawstring");
  auto raw = s_StrBaseFactory.create("raw", rawStr);
  auto lower = s_StrBaseFactory.create("tolower", rawStr);
  
  // Expect equality.
  EXPECT_EQ(raw->msg(), rawStr);
  EXPECT_EQ(lower->msg(), lowerStr);
  
}
