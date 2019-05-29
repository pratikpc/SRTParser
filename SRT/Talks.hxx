#include <vector>

#include "Talk.hxx"

namespace Srt {
   class Talks {
    private:
      std::vector<Srt::Talk> m_Talks;

    public:
      Talks& operator+=(Srt::Talk const& talk) {
         m_Talks.push_back(talk);
         return *this;
      }
      constexpr auto size() const noexcept {
         return std::size(m_Talks);
      }

      void Sort () {
         std::sort(std::begin(m_Talks),
                   std::end(m_Talks),
                   [](auto const& left, auto const& right) {
                      return std::less<>()(left.Sequence, right.Sequence);
                   });
      }

      auto begin() const noexcept {
         return std::begin(m_Talks);
      }
      auto end() const noexcept {
         return std::end(m_Talks);
      }
      auto begin() noexcept {
         return std::begin(m_Talks);
      }
      auto end() noexcept {
         return std::end(m_Talks);
      }

      Talks& operator+=(Time& Time) {
         for (auto& talk : m_Talks)
            talk += Time;
         return *this;
      }
      Talks& operator-=(Time& Time) {
         for (auto& talk : m_Talks)
            talk -= Time;
         return *this;
      }
   };
} // namespace Srt