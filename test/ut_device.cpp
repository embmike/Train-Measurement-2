// GTest: TEST -> ISTQB: Test Case
#include <gtest/gtest.h>

//Test object
#include "../src/device.hpp"

#pragma region Testset
class TestDevice : public Device
{
public:
  TestDevice(double vel_mean, double vel_stddev, double dt)
   : Device{vel_mean, vel_stddev, dt} {};

  void Set_Measurement(double m) {_measurement = m;}
  double Get_Measurement() const {return _measurement;}

  void Set_Position(double p) {_pose_x[0] = p;}
  double Get_Position() const {return _pose_x[0];}

  void Set_Velocity(double v) {_pose_x[1] = v;}
  double Get_Velocity() const {return _pose_x[1];}
};

namespace TestSet
{
    // Testgerät
    constexpr double velocity_mean { 80.0 };  // m/s
    constexpr double velocity_stddev { 8.0 }; // m/s
    constexpr double dt { 0.1 };    // 100ms
    TestDevice device {velocity_mean, velocity_stddev, dt};
    
    // Testlauf
    constexpr double time { 10.0 }; // 10s
    constexpr std::size_t samples { static_cast<uint32_t>(time / dt) };
    std::size_t counter { 0 };
}

bool IsInRange(double value, double min, double max)
{
  bool result { false };

  if (value >= min 
      && value <= max
     )
  {
    result = true;
  }

  return result;
}
#pragma endregion


TEST(device, TC001_Filter_Velocity)
{
  // Arrange
  double arg = 90.0;
  TestSet::device.Set_Measurement(arg);
  double expected = (TestSet::velocity_mean * 9 + arg) / 10;

  // Act
  double occurred = TestSet::device.Filter_Velocity();

  // Check
  EXPECT_EQ(occurred, expected);
}

TEST(device, TC002_Calculate_Position)
{
  // Arrange
  double arg = 90.0;
  TestSet::device.Set_Velocity(arg);
  double expected = arg * TestSet::dt;

  // Act
  double occurred = TestSet::device.Calculate_Position();

  // Check
  EXPECT_EQ(occurred, expected);
}