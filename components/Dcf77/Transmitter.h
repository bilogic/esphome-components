#pragma once


extern int actualHours, actualMinutes, actualSecond, actualDay, actualMonth, actualYear, DayOfW;
extern int impulseArray[60];
extern int impulseCount;
extern int timeRunningContinuous;


int Bin2Bcd(int dato);
void CodeTime();
void DcfOut();