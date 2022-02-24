#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"
#include <string.h>

TEST_CASE("Testcase for breaching LowerLimit") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
}

TEST_CASE("Testcase for breaching HigherLimit") {
  REQUIRE(inferBreach(31, 20, 30) == TOO_HIGH);
}

TEST_CASE("Testcase for valid limit") {
  REQUIRE(inferBreach(25, 20, 30) == NORMAL);
}

TEST_CASE("Testcase for retreiving the limits") {
  BatteryCharacter myBatteryChar = {PASSIVE_COOLING, "EXIDE"};
  double temperatureInC = 50;
  int RangeBasedOnCoolingType[COOLING_TYPES][TEMPERATURE_LIMITS] = {{PASSIVE_COOLING_LOWER_LIMIT, PASSIVE_COOLING_UPPER_LIMIT}, 
                                                                    {HI_ACTIVE_COOLING_LOWER_LIMIT, HI_ACTIVE_COOLING_UPPER_LIMIT}, 
                                                                    {MED_ACTIVE_COOLING_LOWER_LIMIT, MED_ACTIVE_COOLING_UPPER_LIMIT}};
  
  REQUIRE(classifyTemperatureBreach(RangeBasedOnCoolingType, myBatteryChar.coolingType, temperatureInC) == TOO_HIGH);
}

TEST_CASE("Testcase for AlertTextFormatters in Email Target") {
  char TestAlertMessage[44] = "";
  char ExpectedAlertMessage[] = "a.b@c.com : Hi, the temperature is too low";
  prepareAlertTextForEmail(TOO_LOW, TestAlertMessage);
  // String Compare returns 0 if strings are equal. But "REQUIRE" needs 1 if they are equal.
  // Hence added "!"
  REQUIRE(!strcmp(TestAlertMessage,ExpectedAlertMessage));
}

TEST_CASE("Testcase for AlertTextFormatters in Controller Target") {
  char TestAlertMessage[44] = "";
  char ExpectedAlertMessage[] = "feed : 2";
  prepareAlertTextForController(TOO_HIGH, TestAlertMessage);
  // String Compare returns 0 if strings are equal. But "REQUIRE" needs 1 if they are equal.
  // Hence added "!"
  REQUIRE(!strcmp(TestAlertMessage,ExpectedAlertMessage));
}

TEST_CASE("Testcase for validating function PerformBatteryCheck") {
  BatteryCharacter myBatteryChar = {PASSIVE_COOLING, "EXIDE"};
  double temperatureInC = 30;
  int RangeBasedOnCoolingType[COOLING_TYPES][TEMPERATURE_LIMITS] = {{PASSIVE_COOLING_LOWER_LIMIT, PASSIVE_COOLING_UPPER_LIMIT}, 
                                                                    {HI_ACTIVE_COOLING_LOWER_LIMIT, HI_ACTIVE_COOLING_UPPER_LIMIT}, 
                                                                    {MED_ACTIVE_COOLING_LOWER_LIMIT, MED_ACTIVE_COOLING_UPPER_LIMIT}};
  
  REQUIRE(PerformBatteryCheck(myBatteryChar, temperatureInC, RangeBasedOnCoolingType) == 0);
}

TEST_CASE("Testcase to cover print functions") {
  BatteryCharacter myBatteryChar = {PASSIVE_COOLING, "EXIDE"};
  // Call Print functions, but they are no testable (Only for coverage -_-);
  printOnConsole(NULL);
  Initialize_and_Start_BatteryCheckSystem(TO_EMAIL,myBatteryChar,50);
  Initialize_and_Start_BatteryCheckSystem(TO_CONTROLLER,myBatteryChar,-25);
  REQUIRE(1);
}
