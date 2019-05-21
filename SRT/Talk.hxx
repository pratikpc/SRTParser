#pragma once

#include <cassert>
#include <string>
#include <string_view>

#include "TUESL/Utility.hxx"
#include "Time.hxx"

namespace Srt {

   struct Talk {
    private:
      using String = std::string;
      using Index  = int64_t;

    public:
      Index  Sequence;
      Time   Start;
      Time   End;
      String Text;

    public:
      Talk(Index const            p_Sequence,
           Time const             p_Start,
           Time const             p_End,
           std::string_view const p_Text) :
          Sequence{p_Sequence},
          Start{p_Start},
          End{p_End},
          Text{p_Text} {
         assert(p_Start <= p_End);
      }

      friend Talk operator+(Talk const& Left, Time const& Offset) {
         return Talk{Left.Sequence, Left.Start + Offset, Left.End + Offset, Left.Text};
      }
      friend Talk operator-(Talk const& Left, Time const& Offset) {
         return Talk{Left.Sequence, Left.Start - Offset, Left.End - Offset, Left.Text};
      }

      template <typename CharT>
      friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os,
                                                   Srt::Talk const&           Talk) {
         // Do Not Write if item won't be shown on screen at all
         if (Talk.Start >= Talk.End)
            return os;
         return os << Talk.Sequence << '\n'                     // Line 1 -> Sequence
                   << Talk.Start << " --> " << Talk.End << '\n' // Line 2 -> Time
                   << Talk.Text // Line 3 -> End Text( Adds New Line While Parsing)
                   << '\n';     // Line 4:- New Line at End
      }

      Talk& operator+=(Time const& Offset) {
         Start = Start + Offset;
         End   = End + Offset;
         return *this;
      }
      Talk& operator-=(Time const& Offset) {
         Start = Start - Offset;
         End   = End - Offset;
         return *this;
      }
   };
} // namespace Srt