//#include <String.h>
#include "RTC.h"
#include "Arduino_LED_Matrix.h"

int currentSecond;
int epochFrame;
const String WeekDays[8] = { " ", "Mon", "Tues", "Wed", "Thrs", "Fri", "Sat", "Sun" };
int hours, minutes, seconds, year, dayOfMonth;
String dayOfWeek, month;
ArduinoLEDMatrix matrix;

int hor = 0;

byte secondsON_OFF= 1;

// All the numbers for a 7-segment display
// byte Digits[5][30] =
// {
//   { 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1 },
//   { 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 },
//   { 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1 }
// };

byte Digits[5][20] =
{
  { 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1 },
  { 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 },
  { 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1 }
};


// The display on the Uno
byte ScreenBuffer[8][12] =
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
    outputTime(currentTime);

    secondsON_OFF? secondsON_OFF = 0 : secondsON_OFF = 1;

    displayDigit((int)(currentTime.getHour() / 10), 0, 0);
    displayDigit(currentTime.getHour() % 10, 3, 0);
    displayDigit((int)(currentTime.getMinutes() / 10), 6, 0);
    displayDigit(currentTime.getMinutes() % 10, 9, 0);

    // ScreenBuffer[0][2] = secondsON_OFF;
    // ScreenBuffer[0][4] = secondsON_OFF;
    // currentTime.get
    matrix.renderBitmap(ScreenBuffer, 8, 12);

    currentSecond = currentTime.getSeconds();
    moveDot();
  }


}
    

void moveDot()
{
  if (hor - 1 > -1)
  {
    ScreenBuffer[7][hor - 1] = 0;
  }
  else
  {
    ScreenBuffer[7][11] = 0;
  }

  ScreenBuffer[7][hor] = 1;

  hor++;
  if (hor > 11)
  {
    hor = 0;
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
void displayDigit(int d, int screen_x, int screen_y)
{
  // d = 4;
  // for (int x = 0; x < 3; x++)
  for (int y = 0; y < 2; y++)
  {
    // for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++)
    {
      // Serial.print(y + screen_x);
      // Serial.print(x - screen_y);
      // Serial.print("\n");
      ScreenBuffer[x - screen_y][y + screen_x] = Digits[x][y + d * 2];
      // ScreenBuffer[y - screen_x][x + screen_y] = Digits[x][y + d * 3];
    }
  }

  // Insert out matrix with the dimentions
  matrix.renderBitmap(ScreenBuffer, 8, 12);
}

void outputTime(RTCTime currentTime)
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
}

// byte Time[12][8] =
// {
//   { 0, 1, 1, 1, 1, 1, 1, 1 },
//   { 0, 1, 1, 1, 1, 1, 1, 1 },
//   { 0, 1, 1, 1, 1, 1, 1, 1 },
//   { 0, 1, 1, 1, 1, 1, 1, 1 },
//   { 0, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 1, 1, 1, 1, 1, 1, 1 },
//   { 1, 1, 1, 1, 1, 1, 1, 1 }
// };



// byte Digits[30][5] =
// {
//   { 1, 1, 1, 1, 1 },
//   { 1, 0, 0, 0, 1 },
//   { 1, 1, 1, 1, 1 },

//   { 0, 0, 0, 0, 0 },
//   { 0, 0, 0, 0, 0 },
//   { 1, 1, 1, 1, 1 },

//   { 1, 1, 1, 0, 1 },
//   { 1, 0, 1, 0, 1 },
//   { 1, 0, 1, 1, 1 },

//   { 1, 0, 1, 0, 1 },
//   { 1, 0, 1, 0, 1 },
//   { 1, 1, 1, 1, 1 },

//   { 0, 0, 1, 1, 1 },
//   { 0, 0, 1, 0, 0 },
//   { 1, 1, 1, 1, 1 },

//   { 1, 0, 1, 1, 1 },
//   { 1, 0, 1, 0, 1 },
//   { 1, 1, 1, 0, 1 },

//   { 1, 1, 1, 1, 1 },
//   { 1, 0, 1, 0, 1 },
//   { 1, 1, 1, 0, 1 },

//   { 0, 0, 0, 0, 1 },
//   { 0, 0, 0, 0, 1 },
//   { 1, 1, 1, 1, 1 },

//   { 1, 1, 1, 1, 1 },
//   { 1, 0, 1, 0, 1 },
//   { 1, 1, 1, 1, 1 },

//   { 0, 0, 1, 1, 1 },
//   { 0, 0, 1, 0, 1 },
//   { 1, 1, 1, 1, 1 }
// };
