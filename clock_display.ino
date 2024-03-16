//#include <String.h>
#include "RTC.h"
#include "Arduino_LED_Matrix.h"

int currentSecond;
const String WeekDays[8] = { " ", "Mon", "Tues", "Wed", "Thrs", "Fri", "Sat", "Sun" };
int hours, minutes, seconds, year, dayOfMonth;
String dayOfWeek, month;
ArduinoLEDMatrix matrix;

byte secondsON_OFF= 1;

// All the numbers for a 7-segment display
byte Digits[5][30] =
{
  { 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1 },
  { 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 },
  { 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1 }
};

// The display on the Uno
byte Time[8][12] =
{
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void setup()
{
  // put your setup code here, to run once:
  matrix.begin();
  Serial.begin(9600);
  RTC.begin();
  String timeStamp = __TIMESTAMP__;

  getCurrentTime(timeStamp, &dayOfWeek, &dayOfMonth, &year, &month, &hours, &minutes, &seconds);
  // getCurrentTime(timeStamp);

  RTCTime startTime(dayOfMonth, convertToMonth(month), year, hours, minutes, seconds,
                    convertToDOW(dayOfWeek), SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(startTime);
}

void loop()
{
  // put your main code here, to run repeatedly:
  RTCTime currentTime;
  RTC.getTime(currentTime);

  if (currentTime.getSeconds() != currentSecond)
  {
    Serial.print(WeekDays[DayOfWeek2int(currentTime.getDayOfWeek(), false)]);
    Serial.print(" ");
    Serial.print(currentTime.getDayOfMonth());
    Serial.print("-");
    Serial.print(Month2int(currentTime.getMonth()));
    Serial.print("-");
    Serial.print(currentTime.getYear());
    Serial.print(" ");
    Serial.print(currentTime.getHour());
    Serial.print(":");
    Serial.print(currentTime.getMinutes());
    Serial.print(":");
    Serial.println(currentTime.getSeconds());

    secondsON_OFF? secondsON_OFF = 0 : secondsON_OFF = 1;

    displayDigit((int)(currentTime.getHour() / 10), 0, 0);
    displayDigit(currentTime.getHour() % 10, 4, 0);
    displayDigit((int)(currentTime.getMinutes() / 10), 1, 6);
    displayDigit(currentTime.getMinutes() % 10, 5, 6);

    Time[0][2] = secondsON_OFF;
    Time[0][4] = secondsON_OFF;

    matrix.renderBitmap(Time, 8, 12);
    
    currentSecond = currentTime.getSeconds();
  }


}
    

// void getCurrentTime(String timeStamp)
// {

// }

void getCurrentTime(String timeStamp, String* dayOfWeek, int* dayOfMonth, int* year,
                    String* month, int* hours, int *minutes, int* seconds)
{
  *dayOfWeek = timeStamp.substring(0, 3);
  *month = timeStamp.substring(4, 7);
  *dayOfMonth = timeStamp.substring(8, 11).toInt();
  *year = timeStamp.substring(20, 24).toInt();
  *hours = timeStamp.substring(11, 13).toInt();
  *minutes = timeStamp.substring(14, 16).toInt();
  *seconds = timeStamp.substring(17, 19).toInt();
}

DayOfWeek convertToDOW(String dow)
{
  if (dow == String("Mon")) return DayOfWeek::MONDAY;
  if (dow == String("Tue")) return DayOfWeek::TUESDAY;
  if (dow == String("Wed")) return DayOfWeek::WEDNESDAY;
  if (dow == String("Thu")) return DayOfWeek::THURSDAY;
  if (dow == String("Fri")) return DayOfWeek::FRIDAY;
  if (dow == String("Sat")) return DayOfWeek::SATURDAY;
  if (dow == String("Sun")) return DayOfWeek::SUNDAY;
}

Month convertToMonth(String month)
{
  if (month == String("Jan")) return Month::JANUARY;
  if (month == String("Feb")) return Month::FEBRUARY;
  if (month == String("Mar")) return Month::MARCH;
  if (month == String("Apr")) return Month::APRIL;
  if (month == String("May")) return Month::MAY;
  if (month == String("Jun")) return Month::JUNE;
  if (month == String("Jul")) return Month::JULY;
  if (month == String("Aug")) return Month::AUGUST;
  if (month == String("Sep")) return Month::SEPTEMBER;
  if (month == String("Oct")) return Month::OCTOBER;
  if (month == String("Nov")) return Month::NOVEMBER;
  if (month == String("Dec")) return Month::DECEMBER;
}

//  Graphics
void displayDigit(int d, int s_x, int s_y)
{
  for (int y = 0; y < 3; y++)
  {
    for (int x = 0; x < 5; x++)
    {
      Time[y + s_x][11 - x - s_y] = Digits[x][y + d * 3];
    }
  }

  // Insert out matrix with the dimentions
  matrix.renderBitmap(Time, 8, 12);
}


