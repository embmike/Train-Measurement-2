// GTest: TEST -> ISTQB: Test Case
#include <gtest/gtest.h>
#include <array>

//Test object
#include "../src/matrix.hpp"

#pragma region Testset

#pragma endregion


TEST(matrix, TC001_vvmul)
{
  // Arrange
  std::array<int, 3> vec1 {1, 2, 3};
  std::array<int, 3> vec2 {2, 3, 4};
  int expected = 20;

  // Act 
  int occurred = vvmul(vec1, vec2);

  // Check
  EXPECT_EQ(occurred, expected);
}

TEST(matrix, TC001_mvmul)
{
  // Arrange
  std::array<int, 3> vec1 {1, 2, 3};
  std::array<std::array<int, 3>, 2> m1 {{
    {2, 3, 4},
    {3, 4, 5}
  }};
  std::array<int, 2> expected {20, 26};

  // Act
  std::array<int, 2> occurred = mvmul(m1, vec1);

  // Check
  EXPECT_EQ(occurred[0], expected[0]);
  EXPECT_EQ(occurred[1], expected[1]);
}
