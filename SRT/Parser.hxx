#pragma once

#include "TUESL/Files.hxx"

#include "SRT/Talk.hxx"
#include "SRT/Talks.hxx"
#include "SRT/Time.hxx"

#include <string_view>

#include <iostream>

namespace Srt {
   struct Parser {
    private:
      std::tuple<Time, Time> ExtractStartAndEndTime(std::string const& Line) const
          noexcept {
         // 00:02:46,432 --> 00:02:49,701

         auto const FirstGap  = Line.find(' ', 0 /*Init Start*/);
         Time const Start     = Line.substr(0, FirstGap);
         auto const SecondGap = Line.find(' ', FirstGap + 1 /*Ignore Gap 1*/);
         Time const End       = Line.substr(SecondGap + 1);

         return std::make_tuple(Start, End);
      }

      template <typename It>
      std::string ExtractText(It& CurrentLineIt, It const& End) const {
         std::string Text = "";
         while (CurrentLineIt != End) {
            auto const Line = *CurrentLineIt;
            // Read till Blank Line
            if (std::empty(Line))
               break;
            Text += Line + "\n";
            ++CurrentLineIt; // Next Line
         }
         return Text;
      }

      template <typename It>
      Srt::Talk ExtractTalk(It& CurrentLineIt, It const& EndIt) const {
         auto const Sequence = Srt::Utility::Convert<std::int64_t>(*CurrentLineIt);
         CurrentLineIt++; // Next Line
         auto const [Start, End] = ExtractStartAndEndTime(*CurrentLineIt);
         CurrentLineIt++; // Next Line

         auto const Text = ExtractText(CurrentLineIt, EndIt);

         return Srt::Talk{Sequence, Start, End, Text};
      }

    public:
      Parser(std::string_view const       Name,
             std::ios::ios_base::openmode OpenMode = std::fstream::in |
                                                     std::fstream::out) :
          File{Name, OpenMode} {}

      TUESL::Generator<Srt::Talk> ReadAsync() {
         auto Lines = File.ReadLines<>();
         for (auto LinesIt = std::begin(Lines); LinesIt != std::end(Lines); ++LinesIt) {
            co_yield ExtractTalk(LinesIt, std::end(Lines));
         }
      }

      Srt::Talks Read() {
         Srt::Talks Talks;
         for (auto const& Talk : ReadAsync()) {
            Talks += Talk;
         }
         return Talks;
      }

      friend Srt::Parser& operator>>(Srt::Parser& Parser, Srt::Talks& Talks) {
         Talks = Parser.Read();
         return Parser;
      }

      friend Srt::Parser& operator<<(Srt::Parser& Parser, Srt::Talks& Talks) {
         for (auto const& Talk : Talks) {
            Parser.File << Talk;
         }
         return Parser;
      }

    private:
      TUESL::File File;
   };
} // namespace Srt