#pragma once

#include <algorithm>
#include <istream>
#include <string_view>
#include <utility>

namespace Srt {
   namespace Utility {
      template <typename To,
                typename CharT,
                typename = std::enable_if_t<std::is_integral_v<To>>>
      constexpr To Convert(std::basic_string<CharT> const& p_From, int const Base = 10) {
         if constexpr (std::is_same_v<To, signed int> or std::is_same_v<To, signed char> or
                       std::is_same_v<To, signed short>)
            return static_cast<To>(std::stoi(p_From));
         if constexpr (std::is_same_v<To, signed long>)
            return static_cast<To>(std::stol(p_From));
         if constexpr (std::is_same_v<To, signed long long>)
            return static_cast<To>(std::stoll(p_From));
         if constexpr (std::is_same_v<To, unsigned int> or
                       std::is_same_v<To, unsigned char> or
                       std::is_same_v<To, unsigned short> or
                       std::is_same_v<To, unsigned long>)
            return static_cast<To>(std::stoul(p_From));
         if constexpr (std::is_same_v<To, unsigned long long>)
            return static_cast<To>(std::stoull(p_From));
      }

      namespace Stream {
         template <typename CharT, typename T>
         void Parse(std::basic_istream<CharT>& p_Stream,
                    CharT const                p_Separator,
                    T&                         p_Elem /*Out*/) {
            std::basic_string<CharT> str;
            std::getline(p_Stream, str, p_Separator);
            p_Elem = Convert<T>(str);
         }
         template <typename CharT, typename T1, typename... Ts>
         void Parse(std::basic_istream<CharT>& p_Stream,
                    CharT const                p_Separator,
                    T1&                        p_Elem1 /*Out*/,
                    Ts&... p_Rest /*Outs*/) {
            Parse(p_Stream, p_Separator, p_Elem1);
            Parse(p_Stream, p_Separator, p_Rest...);
         }

         template <typename CharT, typename T>
         void ParseWithSeparators(std::basic_istream<CharT>& p_Stream,
                                  std::pair<CharT, T>&       SwE) {
            Parse(p_Stream, SwE.first /*Separator*/, SwE.second /*Elem*/);
         }
         template <typename CharT, typename T1, typename... Ts>
         void ParseWithSeparators(std::basic_istream<CharT>& p_Stream,
                                  std::pair<CharT, T1>&       SwE,
                                  Ts&... p_Rest /*Outs*/) {
            ParseWithSeparators(p_Stream, SwE);
            ParseWithSeparators(p_Stream, p_Rest...);
         }
      } // namespace Stream

   } // namespace Utility
} // namespace Srt