#include "Transmitter.h"
#include "TransmitterConfig.h"
#include "esphome/core/log.h"
#include "esphome/components/time/real_time_clock.h"

int actualHours, actualMinutes, actualSecond, actualDay, actualMonth, actualYear, DayOfW;
int impulseArray[60];
int impulseCount = 0;
int timeRunningContinuous = 0;

esphome::time::RealTimeClock* time_id;
Ticker tickerDecisec; // TBD at 100ms

static const char *TAG = "Transmitter"; 

void DcfOut()
{    
    switch (impulseCount++)
    {
    case 0:
        if (impulseArray[actualSecond] != 0)
        {
            digitalWrite(LEDBUILTIN, LOW);
            ledcWrite(0, 0);
        }
        break;
    case 1:
        if (impulseArray[actualSecond] == 1)
        {
            digitalWrite(LEDBUILTIN, HIGH);
            ledcWrite(0, 127);
        }
        break;
    case 2:        
        digitalWrite(LEDBUILTIN, HIGH);
        ledcWrite(0, 127);
        break;
    case 9:
        impulseCount = 0;

        // if (actualSecond == 1 || actualSecond == 15 || actualSecond == 21 || actualSecond == 29)
        //     ESP_LOGW(TAG, "-");
        // if (actualSecond == 36 || actualSecond == 42 || actualSecond == 45 || actualSecond == 50)
        //     ESP_LOGW(TAG, "-");
        // if (actualSecond == 28 || actualSecond == 35 || actualSecond == 58)
        //     ESP_LOGW(TAG, "P");

        // if (impulseArray[actualSecond] == 1)
        //     ESP_LOGW(TAG, "0");
        // if (impulseArray[actualSecond] == 2)
        //     ESP_LOGW(TAG, "1");

        if (actualSecond == 59)
        {
            // Serial.println();
            // show_time();
#ifndef CONTINUOUSMODE
            if ((dontGoToSleep == 0) or ((dontGoToSleep + onTimeAfterReset) < millis()))
                cronCheck();
#else
            // Serial.println("CONTINUOUS MODE NO CRON!!!");
            timeRunningContinuous++;
            if (timeRunningContinuous > 360)
                ESP.restart(); // 6 hours running, then restart all over
#endif
        }
        break;
    }
    // if (!getLocalTime(&timeinfo))
    // {
        // Serial.println("Error obtaining time...");
        // delay(3000);
        // ESP.restart();
    // }
    CodeTime();
}


int Bin2Bcd(int dato)
{
    int msb, lsb;
    if (dato < 10)
        return dato;
    msb = (dato / 10) << 4;
    lsb = dato % 10;
    return msb + lsb;
}

void CodeTime()
{    
    auto now = time_id->now();
    actualYear = now.year - 2000;
    actualMonth = now.month;
    actualDay = now.day_of_month;
    actualHours   = now.hour;
    actualMinutes   = now.minute;
    actualSecond   = now.second;

    if (actualMinutes >= 60)
    {
        actualMinutes = 0;
        actualHours++;
    }

    if (actualSecond == 60)
        actualSecond = 0;

    int n, Tmp, TmpIn;
    int ParityCount = 0;

    //we put the first 20 bits of each minute at a logical zero value
    for (n = 0; n < 20; n++)
        impulseArray[n] = 1;

    // set DST bit
    if (1)//timeinfo.tm_isdst == 0)
    {
        impulseArray[18] = 2; // CET or DST OFF
    }
    else
    {
        impulseArray[17] = 2; // CEST or DST ON
    }

    //bit 20 must be 1 to indicate active time
    impulseArray[20] = 2;

    //calculates the bits for the minutes
    TmpIn = Bin2Bcd(actualMinutes);
    for (n = 21; n < 28; n++)
    {
        Tmp = TmpIn & 1;
        impulseArray[n] = Tmp + 1;
        ParityCount += Tmp;
        TmpIn >>= 1;
    }
    if ((ParityCount & 1) == 0)
        impulseArray[28] = 1;
    else
        impulseArray[28] = 2;

    //calculates bits for the hours
    ParityCount = 0;
    TmpIn = Bin2Bcd(actualHours);
    for (n = 29; n < 35; n++)
    {
        Tmp = TmpIn & 1;
        impulseArray[n] = Tmp + 1;
        ParityCount += Tmp;
        TmpIn >>= 1;
    }
    if ((ParityCount & 1) == 0)
        impulseArray[35] = 1;
    else
        impulseArray[35] = 2;
    ParityCount = 0;

    //calculate the bits for the actual Day of Month
    TmpIn = Bin2Bcd(actualDay);
    for (n = 36; n < 42; n++)
    {
        Tmp = TmpIn & 1;
        impulseArray[n] = Tmp + 1;
        ParityCount += Tmp;
        TmpIn >>= 1;
    }
    TmpIn = Bin2Bcd(DayOfW);
    for (n = 42; n < 45; n++)
    {
        Tmp = TmpIn & 1;
        impulseArray[n] = Tmp + 1;
        ParityCount += Tmp;
        TmpIn >>= 1;
    }
    //calculates the bits for the actualMonth
    TmpIn = Bin2Bcd(actualMonth);
    for (n = 45; n < 50; n++)
    {
        Tmp = TmpIn & 1;
        impulseArray[n] = Tmp + 1;
        ParityCount += Tmp;
        TmpIn >>= 1;
    }
    //calculates the bits for actual year
    TmpIn = Bin2Bcd(actualYear); // 2 digit year
    for (n = 50; n < 58; n++)
    {
        Tmp = TmpIn & 1;
        impulseArray[n] = Tmp + 1;
        ParityCount += Tmp;
        TmpIn >>= 1;
    }
    //equal date
    if ((ParityCount & 1) == 0)
        impulseArray[58] = 1;
    else
        impulseArray[58] = 2;

    //last missing pulse
    impulseArray[59] = 0; // No pulse
}
