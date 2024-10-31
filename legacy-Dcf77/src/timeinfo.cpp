#include "timeinfo.h"

#ifdef ESP32
#include <WiFi.h> // provides Serial and struct tm
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

bool getLocalTime(struct tm *info, uint32_t ms)
{
    uint32_t start = millis();
    time_t now;
    while ((millis() - start) <= ms)
    {
        time(&now);
        localtime_r(&now, info);
        if (info->tm_year > (2016 - 1900))
        {
            return true;
        }
        delay(10);
    }
    return false;
}
#endif

#include <NTPClient.h>

struct tm timeinfo;
const char *ntpServer = "es.pool.ntp.org"; // enter your closer pool or pool.ntp.org
// const char *TZ_INFO = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"; // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)
const char *TZ_INFO = "SGT-8";

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, ntpServer, 3600, 60000);

void getNTP()
{
    Serial.print("GetNTP ");
    int i = 0;
    do
    {
        i++;
        if (i > 40)
        {
            ESP.restart();
        }
        configTime(0, 0, ntpServer);
        setenv("TZ", TZ_INFO, 1);
        delay(500);
    } while (!getLocalTime(&timeinfo));
    Serial.println("Ok");
}

void show_time()
{
    Serial.print("TimeClient: ");
    Serial.println(timeClient.getFormattedTime());
    // Serial.print(&timeinfo, "Time now: %B %d %Y %H:%M:%S (%A) %Z ");
    if (timeinfo.tm_isdst == 0)
    {
        Serial.println("DST=OFF");
    }
    else
    {
        Serial.println("DST=ON");
    }
}
