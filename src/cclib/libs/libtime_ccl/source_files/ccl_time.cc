
#include "ccl_time.h"

namespace ccl {

// - number of days in month -
constexpr std::array<uint32_t,13> c_month_days = {31,28,31,30,31,30,31,31,30,31,30,31,0};
constexpr std::array<uint32_t,13> c_month_days_leap = {31,29,31,30,31,30,31,31,30,31,30,31,0};

// - number of days of month to end of year -
constexpr std::array<uint32_t,12> c_month_days_to_year_end = {0,31,59,90,120,151,181,212,243,273,304,334};

// === methods of structure datetime_s =========================================

datetime_s::datetime_s(time_s a_time) noexcept
{/*{{{*/
  uint64_t tmp = a_time.m_nanosec;

  // - retrieve time fields -
  m_nsec = tmp % 1000; tmp /= 1000;
  m_usec = tmp % 1000; tmp /= 1000;
  m_msec = tmp % 1000; tmp /= 1000;
  m_sec  = tmp % 60;   tmp /= 60;
  m_min  = tmp % 60;   tmp /= 60;
  m_hour = tmp % 24;   tmp /= 24;

  // - number of days plus days to epoch -
  uint32_t days = tmp + c_days_to_epoch;

  // - calculate week day -
  m_wday = (days % 7) + 1;

  // - calculate year -
  m_year = 1601;

  m_year += (days / c_days_in_400_years) * 400;
  days %= c_days_in_400_years;

  m_year += (days / c_days_in_100_years) * 100;
  days %= c_days_in_100_years;

  m_year += (days / c_days_in_4_years) * 4;
  days %= c_days_in_4_years;

  // - last year of 4 year period can be leap year -
  if (days < 3 * 365)
  {
    m_year += days / 365;
    days %= 365;
  }
  else
  {
    m_year += 3;
    days -= 3 * 365;
  }

  // - calculate month -
  m_month = 1;
  const uint32_t *month_days = is_leap_year(m_year) ? c_month_days_leap.data() : c_month_days.data();
  while (days >= *month_days)
  {
    ++m_month;
    days -= *month_days++;
  }

  // - calculate day -
  m_day = days + 1;
}/*}}}*/

auto datetime_s::nanosec() const -> uint64_t
{/*{{{*/
  if (m_year < 1970)
  {
    throw_error(DATETIME_CANNOT_CONVERT_TO_NANOSEC);
  }

  uint32_t tmp_year = m_year - 1601;

  // - number of days from epoch -
  uint64_t time = tmp_year*365
    + tmp_year/4
    - tmp_year/100
    + tmp_year/400
    + c_month_days_to_year_end[m_month - 1]
    + ((is_leap_year(m_year) && m_month > 2) ? 1 : 0)
    + m_day - 1
    - c_days_to_epoch;

  // - calculate time from 1.1.1970 at nanoseconds -
  time *= 24;   time += m_hour;
  time *= 60;   time += m_min;
  time *= 60;   time += m_sec;
  time *= 1000; time += m_msec;
  time *= 1000; time += m_usec;
  time *= 1000; time += m_nsec;

  // - set value to result time -
  return time;
}/*}}}*/

// === methods of structure time_s =============================================

time_s::time_s(uint32_t a_length,const char *a_data)
{/*{{{*/

  // - ERROR -
  if (a_length != 14)
  {
    throw_error(TIME_WRONG_INIT_STRING);
  }

  std::array<char,15> str;
  memcpy(str.data(),a_data,14);
  str[14] = '\0';

#define BIC_TIME_RETRIEVE_COMP(TARGET,START,END) \
{/*{{{*/\
  char tmp_char = str[END];\
  str[END] = '\0';\
\
  char *end_ptr;\
  TARGET = strtol(str.data() + START,&end_ptr,10);\
\
  /* - ERROR - */\
  if (end_ptr != str.data() + END)\
  {\
    throw_error(TIME_WRONG_INIT_STRING);\
  }\
\
  str[END] = tmp_char;\
}/*}}}*/

  datetime_s dt;

  dt.m_nsec = 0;
  dt.m_usec = 0;
  dt.m_msec = 0;

  BIC_TIME_RETRIEVE_COMP(dt.m_sec  ,12,14);
  BIC_TIME_RETRIEVE_COMP(dt.m_min  ,10,12);
  BIC_TIME_RETRIEVE_COMP(dt.m_hour ,8 ,10);
  BIC_TIME_RETRIEVE_COMP(dt.m_day  ,6 ,8);
  BIC_TIME_RETRIEVE_COMP(dt.m_month,4 ,6);
  BIC_TIME_RETRIEVE_COMP(dt.m_year ,0 ,4);

  // - ERROR -
  if (dt.m_year < 1970 || dt.m_month < 1 || dt.m_month > 12 || dt.m_day < 1 ||
      dt.m_day > (datetime_s::is_leap_year(dt.m_year) ? c_month_days_leap : c_month_days)[dt.m_month - 1] ||
      dt.m_hour >= 24 || dt.m_min >= 60 || dt.m_sec >= 60)
  {
    throw_error(TIME_WRONG_INIT_STRING);
  }

  // - convert datetime to time value -
  m_nanosec = dt.nanosec();
}/*}}}*/

} // namespace ccl

