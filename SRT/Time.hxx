#pragma once
#include <cstdint>
#include <iomanip>
#include <sstream>

#include "TUESL/Utility.hxx"
#include <chrono>
#include <date/date.h>

namespace Srt {
   struct Time {
    private:
      using TimeT =
          std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

    public:
      TimeT m_time;

    public:
      constexpr Time(std::int8_t const  p_Hour,
                     std::int8_t const  p_Minute,
                     std::int8_t const  p_Second,
                     std::int16_t const p_MilliS) :
          // Convert to Milliseconds
          Time{p_MilliS + p_Second * 1000 + p_Minute * 1000 * 60 +
               p_Hour * 1000 * 60 * 60} {}
      explicit constexpr Time(std::int32_t const p_MilliS) :
          Time{TimeT::duration{p_MilliS}} {}
      explicit constexpr Time(typename TimeT::duration const&& p_Time) :
          m_time{std::move(p_Time)} {}

      constexpr Time(std::nullptr_t = nullptr) : Time{0, 0, 0, 0} {}
      template <typename CharT>
      Time(std::basic_string<CharT> p_Time) : Time{Parse(p_Time)} {}

      template <typename CharT>
      static Time Parse(std::basic_string<CharT> p_Time) {
         // 02:04:05,0405
         // Replace , for Easier Parsing
         std::replace(std::rbegin(p_Time), std::rend(p_Time), CharT(','), CharT(':'));
         std::basic_stringstream<CharT> streamTime{p_Time};

         std::int16_t MilliS;
         std::int8_t  Minute;
         std::int8_t  Second;
         std::int8_t  Hour;

         // Let time be in the form
         // 02:04:05:0405
         Utility::Stream::Parse(streamTime, CharT(':'), Hour, Minute, Second, MilliS);

         return Time{Hour, Minute, Second, MilliS};
      }

      constexpr friend Time operator+(const Time& left, const Time& right) noexcept {
         auto const TimeV =
             left.m_time.time_since_epoch() + right.m_time.time_since_epoch();
         return Time{std::move(TimeV)};
      }

      constexpr friend Time operator-(const Time& left, const Time& right) noexcept {
         if (left < right)
            return nullptr;
         auto const TimeV =
             left.m_time.time_since_epoch() - right.m_time.time_since_epoch();
         return Time{std::move(TimeV)};
      }

      template <typename CharT>
      friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os,
                                                   Time const&                Time) {
         // 02:04:05:0405

         // Millis with Dot. Change it
         auto s = date::format("%H:%M:%S", Time.m_time);
         std::replace(std::begin(s), std::end(s), '.', ',');
         return os << s;
      }

      constexpr friend bool operator<(const Time& left, const Time& right) noexcept {
         return left.m_time < right.m_time;
      }
      constexpr friend bool operator>(const Time& left, const Time& right) noexcept {
         return left.m_time > right.m_time;
      }
      constexpr friend bool operator<=(const Time& left, const Time& right) noexcept {
         return left.m_time <= right.m_time;
      }
      constexpr friend bool operator>=(const Time& left, const Time& right) noexcept {
         return left.m_time >= right.m_time;
      }
   };
} // namespace Srt
