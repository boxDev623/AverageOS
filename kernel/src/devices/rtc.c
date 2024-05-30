#include "rtc.h"
#include "io.h"

#define IO_RTC_INDEX                    0x70
#define IO_RTC_TARGET                   0x71

#define REG_SEC                         0x00
#define REG_SEC_ALARM                   0x01
#define REG_MIN                         0x02
#define REG_MIN_ALARM                   0x03
#define REG_HOUR                        0x04
#define REG_HOUR_ALARM                  0x05
#define REG_WEEK_DAY                    0x06
#define REG_DAY                         0x07
#define REG_MONTH                       0x08
#define REG_YEAR                        0x09
#define REG_A                           0x0a
#define REG_B                           0x0b
#define REG_C                           0x0c
#define REG_D                           0x0d

#define REGA_UIP                        (1 << 7)    // Update In Progress

#define REGB_HOURFORM                   (1 << 1)    // Hour Format (0 = 12hr, 1 = 24hr)
#define REGB_DM                         (1 << 2)    // Data Mode (0 = BCD, 1 = Binary)

static uint8_t rtc_read(uint8_t addr)
{
    outportb(IO_RTC_INDEX, addr);
    return inportb(IO_RTC_TARGET);
}

static void rtc_write(uint8_t addr, uint8_t val)
{
    outportb(IO_RTC_INDEX, addr);
    outportb(IO_RTC_TARGET, val);
}

static uint8_t bcd_to_bin(uint8_t val)
{
    return (val & 0xf) + (val >> 4) * 10;
}

static uint8_t bin_to_bcd(uint8_t val)
{
    return ((val / 10) << 4) + (val % 10);
}

void rtc_get_time(date_time_t *dt)
{
    /*if (rtc_read(REG_A) & REGA_UIP)
    {
        pit_wait(3);
    }*/

    uint8_t sec = rtc_read(REG_SEC);
    uint8_t min = rtc_read(REG_MIN);
    uint8_t hour = rtc_read(REG_HOUR);
    uint8_t weekDay = rtc_read(REG_WEEK_DAY);
    uint8_t day = rtc_read(REG_DAY);
    uint8_t month = rtc_read(REG_MONTH);
    uint16_t year = rtc_read(REG_YEAR);

    uint8_t regb = rtc_read(REG_B);

    if (~regb & REGB_DM)
    {
        sec = bcd_to_bin(sec);
        min = bcd_to_bin(min);
        hour = bcd_to_bin(hour);
        day = bcd_to_bin(day);
        month = bcd_to_bin(month);
        year = bcd_to_bin(year);
    }

    year += 2000;

    weekDay--;

    dt->sec = sec;
    dt->min = min;
    dt->hour = hour;
    dt->day = day;
    dt->month = month;
    dt->year = year;
    dt->weekDay = weekDay;
    dt->tzOffset = 0;
}

void rtc_set_time(const date_time_t *dt)
{
    uint8_t sec = dt->sec;
    uint8_t min = dt->min;
    uint8_t hour = dt->hour;
    uint8_t day = dt->day;
    uint8_t month = dt->month;
    uint8_t year = dt->year - 2000;
    uint8_t weekDay = dt->weekDay + 1;

    if (sec >= 60 || min >= 60 || hour >= 24 || day > 31 || month > 12 || year >= 100 || weekDay > 7)
        return;

    uint8_t regb = rtc_read(REG_B);

    if (~regb & REGB_DM)
    {
        sec = bcd_to_bin(sec);
        min = bcd_to_bin(min);
        hour = bcd_to_bin(hour);
        day = bcd_to_bin(day);
        month = bcd_to_bin(month);
        year = bcd_to_bin(year);
    }

    /*if (rtc_read(REG_A) & REGA_UIP)
    {
        pit_wait(3);
    }*/

    rtc_write(REG_SEC, sec);
    rtc_write(REG_MIN, min);
    rtc_write(REG_HOUR, hour);
    rtc_write(REG_WEEK_DAY, weekDay);
    rtc_write(REG_DAY, day);
    rtc_write(REG_MONTH, month);
    rtc_write(REG_YEAR, year);
}