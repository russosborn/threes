#include <gtest/gtest.h>
#include <Utils.h>

#include <string>


// Demonstrate some basic assertions.
TEST(RegisterTest, StrSplit) {

  std::string fullStringNoDelim("fullStringNoDelim");
  std::string delim(";");

  auto testv1( ro::strsplit(fullStringNoDelim, delim) );
  EXPECT_EQ(testv1.size(), 1);
  EXPECT_EQ(testv1[0], fullStringNoDelim);

  std::string singleSplit("fullString;OneDelim");
  auto testv2( ro::strsplit(singleSplit, delim) );
  EXPECT_EQ(testv2.size(), 2);
  EXPECT_EQ(testv2[0], "fullString");
  EXPECT_EQ(testv2[1], "OneDelim");

  std::string fancyDelim("fullStringAAOneDelimAATwoDelim");
  auto testv3( ro::strsplit(fancyDelim, "AA") );
  EXPECT_EQ(testv3.size(), 3);
  EXPECT_EQ(testv3[0], "fullString");
  EXPECT_EQ(testv3[1], "OneDelim");
  EXPECT_EQ(testv3[2], "TwoDelim");
  
}
