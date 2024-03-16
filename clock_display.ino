//#include <String.h>
#include "RTC.h"

int currentSecond;
const String WeekDays[8] = { " ", "Mon", "Tues", "Wed", "Thrs", "Fri", "Sat", "Sun" };

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  RTC.begin();
  RTCTime startTime(17, Month::MARCH, 2023, 9, 45, 00,
                    DayOfWeek::SATURDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(startTime);
}

void loop()
{
  // put your main code here, to run repeatedly:
  RTCTime currentTime;
  RTC.getTime(currentTime);

  if (currentTime.getSeconds() != currentSecond)
  {
    //Serial.print(WeekDays[DayOfWeek2int(currentTime.getDayOfWeek(), false)]);
    Serial.print(" ");
    Serial.print(currentTime.getDayOfMonth());
    Serial.print(" ");
    Serial.print(Month2int(currentTime.getMonth()));
    Serial.print(" ");
    Serial.print(currentTime.getYear());
    Serial.print(" ");
    Serial.print(currentTime.getHour());
    Serial.print(" ");
    Serial.print(currentTime.getMinutes());
    Serial.print(" ");
    Serial.println(currentTime.getSeconds());
    currentSecond = currentTime.getSeconds();
  }
    







}
