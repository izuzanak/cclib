
#ifndef LIBS_LIBTIME_CCL_HEADER_FILES_CCL_TIME_H
#define LIBS_LIBTIME_CCL_HEADER_FILES_CCL_TIME_H

#include "ccl_base.h"

namespace ccl {

constexpr int DATETIME_CANNOT_CONVERT_TO_NANOSEC = 1;

constexpr int TIME_WRONG_INIT_STRING = 1;

constexpr int CLOCK_GET_RESOLUTION_ERROR = 1;
constexpr int CLOCK_GET_TIME_ERROR = 2;
constexpr int CLOCK_SET_TIME_ERROR = 3;

struct time_s;

// === definition of structure datetime_s ======================================

struct datetime_s
{/*{{{*/
  friend time_s;

private:
  static constexpr uint64_t c_days_to_epoch     = 134774;
  static constexpr uint64_t c_days_in_400_years = 146097;
  static constexpr uint64_t c_days_in_100_years = 36524;
  static constexpr uint64_t c_days_in_4_years   = 1461;

  uint16_t m_year = 0;
  uint8_t m_month = 0;
  uint8_t m_day = 0;
  uint8_t m_wday = 0;
  uint8_t m_hour = 0;
  uint8_t m_min = 0;
  uint8_t m_sec = 0;
  uint16_t m_msec = 0;
  uint16_t m_usec = 0;
  uint16_t m_nsec = 0;

  auto static is_leap_year(uint32_t a_year) noexcept -> bool
  {/*{{{*/
    return (a_year % 4 == 0 && a_year % 100 != 0) || (a_year % 400 == 0);
  }/*}}}*/

public:
  datetime_s() = default;
  explicit datetime_s(time_s a_time) noexcept;

  [[nodiscard]] auto nanosec() const -> uint64_t;
};/*}}}*/

// === definition of structure time_s ==========================================

struct time_s
{/*{{{*/
  friend datetime_s;

private:
  static constexpr uint64_t nanosec_in_second = 1000000000ULL;

  uint64_t m_nanosec = 0;

public:
  time_s() = default;
  explicit time_s(uint64_t a_nanosec) noexcept : m_nanosec{a_nanosec} {}
  explicit time_s(datetime_s a_datetime) noexcept : m_nanosec{a_datetime.nanosec()} {}
  time_s(unsigned a_length,const char *a_data);
  explicit time_s(const char *a_data) : time_s(strlen(a_data),a_data) {}
  explicit time_s(const std::string &a_string) : time_s(a_string.length(),a_string.data()) {}

  auto to_string(ccl::array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    datetime_s dt{*this};

    return a_trg->append_format("%4.4hu/%2.2hu/%2.2hu %2.2hu:%2.2hu:%2.2hu"
      ,dt.m_year,dt.m_month,dt.m_day,dt.m_hour,dt.m_min,dt.m_sec);
  }/*}}}*/

  [[nodiscard]] auto static now() -> time_s
  {/*{{{*/
    return clock_gettime(CLOCK_REALTIME);
  }/*}}}*/

  [[nodiscard]] auto static clock_getres(clockid_t a_clock_id) -> time_s
  {/*{{{*/
    struct timespec ts;

    if (::clock_getres(a_clock_id,&ts) != 0)
    {
      throw_error(CLOCK_GET_RESOLUTION_ERROR);
    }

    return time_s{ts.tv_sec*nanosec_in_second + ts.tv_nsec};
  }/*}}}*/

  [[nodiscard]] auto static clock_gettime(clockid_t a_clock_id) -> time_s
  {/*{{{*/
    struct timespec ts;

    if (::clock_gettime(a_clock_id,&ts) != 0)
    {
      throw_error(CLOCK_GET_TIME_ERROR);
    }

    return time_s{ts.tv_sec*nanosec_in_second + ts.tv_nsec};
  }/*}}}*/

  auto clock_settime(clockid_t a_clock_id) const -> void
  {/*{{{*/
    struct timespec ts;

    ts.tv_sec = m_nanosec / nanosec_in_second;
    ts.tv_nsec = m_nanosec % nanosec_in_second;

    if (::clock_settime(a_clock_id,&ts) != 0)
    {
      throw_error(CLOCK_SET_TIME_ERROR);
    }
  }/*}}}*/

  [[nodiscard]] auto nano() const noexcept -> uint64_t
  {/*{{{*/
    return m_nanosec;
  }/*}}}*/

  [[nodiscard]] auto micro() const noexcept -> uint64_t
  {/*{{{*/
    return m_nanosec/1000ULL;
  }/*}}}*/

  [[nodiscard]] auto milli() const noexcept -> uint64_t
  {/*{{{*/
    return m_nanosec/1000000ULL;
  }/*}}}*/

  [[nodiscard]] auto seconds() const noexcept -> uint64_t
  {/*{{{*/
    return m_nanosec/1000000000ULL;
  }/*}}}*/

  [[nodiscard]] auto minutes() const noexcept -> uint64_t
  {/*{{{*/
    return m_nanosec/60000000000ULL;
  }/*}}}*/

  [[nodiscard]] auto hours() const noexcept -> uint64_t
  {/*{{{*/
    return m_nanosec/3600000000000ULL;
  }/*}}}*/

  [[nodiscard]] auto days() const noexcept -> uint64_t
  {/*{{{*/
    return m_nanosec/86400000000000ULL;
  }/*}}}*/
};/*}}}*/

} // namespace ccl

#endif

