#include <src/Board.h>
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(BoardState, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("board", "state");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
