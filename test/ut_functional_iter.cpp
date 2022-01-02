// GTest: TEST -> ISTQB: Test Case
#include <gtest/gtest.h>

//Test object
#include "../src/functional_iter.hpp"

#pragma region Testset
int theIter { 0 };
void IterMe1()
{
  theIter++;
}

void IterMe2(std::size_t& iter)
{

}
#pragma endregion


TEST(functional_iter, TC001_for_each_iter__void)
{
  // Arrange
  std::size_t occurred = 0;
  std::size_t expected = 5;

  // Act 
  for_each_iter(occurred, expected, IterMe1);

  // Check
  EXPECT_EQ(occurred, expected);
}

TEST(functional_iter, TC002_for_each_iter__size_t )
{
  // Arrange
  std::size_t occurred = 0;
  std::size_t expected = 5;

  // Act 
  for_each_iter(occurred, expected, IterMe2);

  // Check
  EXPECT_EQ(occurred, expected);
}