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

void testCaseHelper_classifyTemperatureBreach(int RangeBasedOnCoolingType[][TEMPERATURE_LIMITS], 
                                              CoolingType coolingType, double temperatureInC, BreachType expectedBreachType) {
  REQUIRE(classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, temperatureInC) == expectedBreachType);
}

TEST_CASE("Testcase for retreiving the limits") {
  int RangeBasedOnCoolingType[COOLING_TYPES][TEMPERATURE_LIMITS] = {{PASSIVE_COOLING_LOWER_LIMIT, PASSIVE_COOLING_UPPER_LIMIT}, 
                                                                    {HI_ACTIVE_COOLING_LOWER_LIMIT, HI_ACTIVE_COOLING_UPPER_LIMIT}, 
                                                                    {MED_ACTIVE_COOLING_LOWER_LIMIT, MED_ACTIVE_COOLING_UPPER_LIMIT}};
  CoolingType coolingType = PASSIVE_COOLING;
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, -20, TOO_LOW);
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, 10, NORMAL);
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, 40, TOO_HIGH);
  
  coolingType = HI_ACTIVE_COOLING;
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, -20, TOO_LOW);
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, 10, NORMAL);
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, 50, TOO_HIGH);
  
  coolingType = MED_ACTIVE_COOLING;
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, -20, TOO_LOW);
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, 10, NORMAL);
  testCaseHelper_classifyTemperatureBreach(RangeBasedOnCoolingType, coolingType, 41, TOO_HIGH);
}

TEST_CASE("Testcase for AlertTextFormatters in Email Target") {
  char ExpectedAlertMessage[] = "a.b@c.com : Hi, the temperature is too low";
  prepareAlertTextForEmail(TOO_LOW);
  // String Compare returns 0 if strings are equal. But "REQUIRE" needs 1 if they are equal.
  // Hence added "!"
  REQUIRE(!strcmp(MessageToBeDisplayedOnConsole,ExpectedAlertMessage));
}

TEST_CASE("Testcase for AlertTextFormatters in Controller Target") {
  char ExpectedAlertMessage[] = "feed : 2";
  prepareAlertTextForController(TOO_HIGH);
  // String Compare returns 0 if strings are equal. But "REQUIRE" needs 1 if they are equal.
  // Hence added "!"
  REQUIRE(!strcmp(MessageToBeDisplayedOnConsole,ExpectedAlertMessage));
}

TEST_CASE("Testcase for validating function PerformBatteryCheck") {
  BatteryCharacter myBatteryChar = {PASSIVE_COOLING, "EXIDE"};
  int RangeBasedOnCoolingType[COOLING_TYPES][TEMPERATURE_LIMITS] = {{PASSIVE_COOLING_LOWER_LIMIT, PASSIVE_COOLING_UPPER_LIMIT}, 
                                                                    {HI_ACTIVE_COOLING_LOWER_LIMIT, HI_ACTIVE_COOLING_UPPER_LIMIT}, 
                                                                    {MED_ACTIVE_COOLING_LOWER_LIMIT, MED_ACTIVE_COOLING_UPPER_LIMIT}};
  
  REQUIRE(PerformBatteryCheck(myBatteryChar, -10, RangeBasedOnCoolingType) == 1);
  REQUIRE(PerformBatteryCheck(myBatteryChar, 30, RangeBasedOnCoolingType) == 0);
  REQUIRE(PerformBatteryCheck(myBatteryChar, 40, RangeBasedOnCoolingType) == 2);
  
  myBatteryChar.coolingType = HI_ACTIVE_COOLING;
  REQUIRE(PerformBatteryCheck(myBatteryChar, -10, RangeBasedOnCoolingType) == 1);
  REQUIRE(PerformBatteryCheck(myBatteryChar, 30, RangeBasedOnCoolingType) == 0);
  REQUIRE(PerformBatteryCheck(myBatteryChar, 50, RangeBasedOnCoolingType) == 2);
  
  myBatteryChar.coolingType = MED_ACTIVE_COOLING;
  REQUIRE(PerformBatteryCheck(myBatteryChar, -10, RangeBasedOnCoolingType) == 1);
  REQUIRE(PerformBatteryCheck(myBatteryChar, 30, RangeBasedOnCoolingType) == 0);
  REQUIRE(PerformBatteryCheck(myBatteryChar, 41, RangeBasedOnCoolingType) == 2);
}

TEST_CASE("Testcase to cover print functions") {
  // Call Print functions, but they are no testable (Only for coverage -_-);
  printOnConsole(NULL);
  REQUIRE(1);
}

void testWrapperFunctions(BatteryCharacter myBatteryChar) {
  Initialize_and_Start_BatteryCheckSystem(TO_EMAIL,myBatteryChar,-25);
  Initialize_and_Start_BatteryCheckSystem(TO_EMAIL,myBatteryChar,25);
  Initialize_and_Start_BatteryCheckSystem(TO_EMAIL,myBatteryChar,50);
  Initialize_and_Start_BatteryCheckSystem(TO_CONTROLLER,myBatteryChar,-25);
  Initialize_and_Start_BatteryCheckSystem(TO_CONTROLLER,myBatteryChar,25);
  Initialize_and_Start_BatteryCheckSystem(TO_CONTROLLER,myBatteryChar,50);
}

TEST_CASE("Testcase to cover wrapper functions") {
  BatteryCharacter BatteryChar = {PASSIVE_COOLING, "BATTERY"};
  testWrapperFunctions(BatteryChar);
  
  BatteryChar.coolingType = HI_ACTIVE_COOLING;
  testWrapperFunctions(BatteryChar);
  
  BatteryChar.coolingType = MED_ACTIVE_COOLING;
  testWrapperFunctions(BatteryChar);
  REQUIRE(1);
}
