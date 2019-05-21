#pragma once
#include <fstream>
#include <string>
#include <string_view>

#include "CoRoutines.hxx"

namespace TUESL {
   struct File {
    public:
      File(std::string_view const Name,
           int const              OpenMode = std::fstream::in | std::fstream::out) :
          Handle{std::data(Name), OpenMode} {}

      template <typename String = std::string>
      TUESL::Generator<String> ReadLines(int const From = 0) {
         Handle.seekp(From);
         String data;
         while (std::getline(Handle, data))
            co_yield data;
      }

      template <typename T>
      friend TUESL::File& operator<<(TUESL::File& File, T const Element) {
         File.Handle << Element;
         return File;
      }

    private:
      std::fstream Handle;
   };
} // namespace TUESL