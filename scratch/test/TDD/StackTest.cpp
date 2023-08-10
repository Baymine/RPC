#include "./Stack.h"
#include <gtest/gtest.h>

TEST(StackTest, PushPop){
    Stack s;
    s.push(1);
    EXPECT_EQ(s.pop(), 1);
}

TEST(StackTest, Size){
    Stack s;
    s.push(1); s.push(2);
    EXPECT_EQ(s.size(), 2);
}

int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

}