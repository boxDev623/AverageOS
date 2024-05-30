#pragma once

#include <stdint.h>

typedef struct date_time
{
    int32_t sec;            // [0, 59]
    int32_t min;            // [0, 59]
    int32_t hour;           // [0, 23]
    int32_t day;            // [1, 31]
    int32_t month;          // [1, 12]
    int32_t year;           // [1970, 2038]
    int32_t weekDay;        // [0, 6] sunday = 0
    int32_t yearDay;        // [0, 365]
    int32_t tzOffset;       // offset in minutes
}
date_time_t;

void rtc_get_time(date_time_t *dt);
void rtc_set_time(const date_time_t *dt);