#include <experimental/generator>

namespace TUESL {
   template<typename T>
   using Generator = std::experimental::generator<T>;
}