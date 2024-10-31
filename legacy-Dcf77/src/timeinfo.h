#include <NTPClient.h>

extern struct tm timeinfo;
extern NTPClient timeClient;

void getNTP();
void show_time();

#ifdef ESP8266
bool getLocalTime(struct tm *info, uint32_t ms = 5000);
#endif