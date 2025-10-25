#include<Cli.h>

#include<print>
#include<iostream>

namespace cli {
  namespace colors {
    std::string set(int code) {
      return std::format(detail::escape, code);
    }
  }
  
  CommandLineInterface::CommandLineInterface(int argc, char const ** argv) {
    for (int arg = 1; arg < argc; ++arg) {
      arguments.emplace(argv[arg], "");
    }
  }

  bool CommandLineInterface::isDBRebuildRequested() const {
    auto rebuildArg = arguments.find("DatabaseRebuild");
    if (rebuildArg == arguments.end()) {
      return false;
    }
    std::println("{}{}WARNING: This will wipe the existing database!{}", colors::set(colors::bold), colors::set(colors::fgBrRed), colors::set(colors::reset));
    const std::string resetConfirm = "DeleteDatabase";
    std::print("{}To confirm, you must type '{}' exactly. > {}", colors::set(colors::fgYellow), resetConfirm, colors::set(colors::reset));
    std::string command;
    if (!std::getline(std::cin, command) || command != resetConfirm) {
      return false;
    }
    std::print("{}Please Double Confirm by typing '{}' again. > {}", colors::set(colors::fgYellow), resetConfirm, colors::set(colors::reset));
    std::string command2;
    if (!std::getline(std::cin, command2) || command2 != resetConfirm) {
      return false;
    }
    return true;
  }
}