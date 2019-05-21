// SRTTimeManager.cpp : Defines the entry point for the application.
//

#include "SRT/Parser.hxx"

#include <iostream>

using namespace std;

int main() {
   Srt::Parser parser{
       R"(Path/To/Srt)"};
   for (auto const obj : parser.Read())
      std::cout << obj;
   return 0;
}
