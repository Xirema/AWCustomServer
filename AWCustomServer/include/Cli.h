#pragma once
#include<string>
#include<string_view>
#include<map>
#include<format>

namespace cli {
  namespace colors {
    namespace detail {
      constexpr std::string escape = "\033[{}m";
    }
    constexpr int 
      reset = 0,
      bold = 1,
      dim = 2,
      italics = 3,
      underlined = 4,
      blinking = 5,
      reverse = 7,
      hidden = 8,
      strikethrough = 9,

      fgBlack = 30,     bgBlack = 40,
      fgRed = 31,       bgRed = 41,
      fgGreen = 32,     bgGreen = 42,
      fgYellow = 33,    bgYellow = 43,
      fgBlue = 34,      bgBlue = 44,
      fgMagenta = 35,   bgMagenta = 45,
      fgCyan = 36,      bgCyan = 46,
      fgWhite = 37,     bgWhite = 47,
      fgDefault = 39,   bgDefault = 49,

      fgBrBlack = 90,   bgBrBlack = 100,
      fgBrRed = 91,     bgBrRed = 101,
      fgBrGreen = 92,   bgBrGreen = 102,
      fgBrYellow = 93,  bgBrYellow = 103,
      fgBrBlue = 94,    bgBrBlue = 104,
      fgBrMagenta = 95, bgBrMagenta = 105,
      fgBrCyan = 96,    bgBrCyan = 106,
      fgBrWhite = 97,   bgBrWhite = 107

      ;


    std::string set(int code);
  }
  class CommandLineInterface {
    std::map<std::string, std::string> arguments;
  public:
    CommandLineInterface(int argc, char const ** argv);

    bool isDBRebuildRequested() const;
  };
}