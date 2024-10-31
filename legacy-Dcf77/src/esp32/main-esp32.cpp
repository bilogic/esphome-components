/*
  based on this sketch: https://github.com/aknik/ESP32/blob/master/DFC77/DFC77_esp32_Solo.ino

  Some functions are inspired by work of G6EJD ( https://www.youtube.com/channel/UCgtlqH_lkMdIa4jZLItcsTg )

  Refactor by DeltaZero, converts to syncronous, added "cron" that you can bypass, see line 29
                                                    The cron does not start until 10 minutes from reset (see constant onTimeAfterReset)
  Every clock I know starts to listen to the radio at aproximatelly the hour o'clock, so cron takes this into account

  Alarm clocks from Junghans: Every hour (innecesery)
  Weather Station from Brigmton: 2 and 3 AM
  Chinesse movements and derivatives: 1 o'clock AM
*/

#include <WiFi.h>
#include <Ticker.h>
#include <Time.h>

#include "timeinfo.h"
#include "dcf77.h"
#include "transmitter-config.h" // modify this for different pin outs
#include "wifi-credentials.h"   // modify this for different pin outs

// cron (if you choose the correct values you can even run on batteries)
// If you choose really bad this minutes, everything goes wrong, so minuteToWakeUp must be greater than minuteToSleep
#define minuteToWakeUp 55 // Every hoursToWakeUp at this minute the ESP32 wakes up get time and star to transmit
#define minuteToSleep 8   // If it is running at this minute then goes to sleep and waits until minuteToWakeUp

byte hoursToWakeUp[] = {0, 1, 2, 3}; // you can add more hours to adapt to your needs
                                     // When the ESP32 wakes up, check if the actual hour is in the list and
                                     // runs or goes to sleep until next minuteToWakeUp

Ticker tickerDecisec; // TBD at 100ms

// complete array of pulses for a minute
// 0 = no pulse, 1=100ms, 2=200ms
long dontGoToSleep = 0;
const long onTimeAfterReset = 600000; // Ten minutes

void sleepForMinutes(int minutes)
{
    if (minutes < 2)
        return;
    uint64_t uSecToMinutes = 60000000;
    esp_sleep_enable_timer_wakeup(minutes * uSecToMinutes); // this timer works on uSecs, so 60M by minute
    // WiFi_off();
    Serial.print("- Sleeping for ");
    Serial.print(minutes);
    Serial.println(" minutes");
    Serial.flush();
    esp_deep_sleep_start();
}

void WiFi_on()
{
    Serial.print("- Connecting WiFi...");
    WiFi.begin(ssid, password);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        if (counter > 20)
            ESP.restart();
        Serial.print(".");
        counter++;
    }
    Serial.println();
    Serial.println("- WiFi connected");
}

void WiFi_off()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("- WiFi disconnected");
    Serial.flush();
}

void cronCheck()
{
    // is this hour in the list?
    boolean work = false;
    for (int n = 0; n < sizeof(hoursToWakeUp); n++)
    {
        // Serial.println(sizeof(hoursToWakeUp)); Serial.print(work); Serial.print(" "); Serial.print(n); Serial.print(" "); Serial.print(actualHours); Serial.print(" "); Serial.println(hoursToWakeUp[n]);
        if ((actualHours == hoursToWakeUp[n]) or (actualHours == (hoursToWakeUp[n] + 1)))
        {
            work = true;
            // is this the minute to go to sleep?
            if ((actualMinutes > minuteToSleep) and (actualMinutes < minuteToWakeUp))
            {
                // go to sleep (minuteToWakeUp - actualMinutes)
                Serial.print(".");
                sleepForMinutes(minuteToWakeUp - actualMinutes);
            }
            break;
        }
    }
    if (work == false)
    { // sleep until minuteToWakeUp (take into account the ESP32 can start for some reason between minuteToWakeUp and o'clock)
        if (actualMinutes >= minuteToWakeUp)
        {
            Serial.print("..");
            sleepForMinutes(minuteToWakeUp + 60 - actualMinutes);
        }
        else
        {
            // goto sleep for (minuteToWakeUp - actualMinutes) minutes
            Serial.print("...");
            sleepForMinutes(minuteToWakeUp - actualMinutes);
        }
    }
}

void setup()
{
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    Serial.begin(1000000);
    Serial.println();
    Serial.println("# DCF77 transmitter");

    Serial.print("- Transmitting signal on IO");
    Serial.println(ANTENNAPIN);

    if (esp_sleep_get_wakeup_cause() == 0)
        dontGoToSleep = millis();

    ledcSetup(0, 77500, 8);       // DCF77 frequency
    ledcAttachPin(ANTENNAPIN, 0); // This Pin, or another one you choose, has to be attached to the antenna

    pinMode(LEDBUILTIN, OUTPUT);
    digitalWrite(LEDBUILTIN, LOW); // LOW if LEDBUILTIN is inverted like in Wemos boards

    WiFi_on();
    getNTP();

    timeClient.setTimeOffset(8 * 3600);
    timeClient.begin();
    timeClient.update();

    WiFi_off();
    show_time();

    CodeTime(); // first conversion just for cronCheck
#ifndef CONTINUOUSMODE
    if ((dontGoToSleep == 0) or ((dontGoToSleep + onTimeAfterReset) < millis()))
        cronCheck(); // first check before start anything
#else
    Serial.println("- Transmitting signal continuously");
#endif

    // sync to the start of a second
    Serial.print("- Syncing... ");
    int startSecond = timeinfo.tm_sec;
    long count = 0;
    do
    {
        count++;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("- Error obtaining time...");
            delay(3000);
            ESP.restart();
        }
    } while (startSecond == timeinfo.tm_sec);

    tickerDecisec.attach_ms(100, DcfOut); // from now on calls DcfOut every 100ms
    Serial.print("Ok ");
    Serial.println(count);
}

void loop()
{
    // There is no code inside the loop. This is a syncronous program driven by the Ticker
}
