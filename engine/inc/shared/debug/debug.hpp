/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#ifdef NDEBUG
#define TYRA_LOG(...) ((void)0)
#define TYRA_WARN(...) ((void)0)
#define TYRA_ERROR(...) ((void)0)
#define TYRA_TRAP(...) ((void)0)
#define TYRA_ASSERT(condition, ...) ((void)0)

#else  // IF Debug

#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <memory>

#include "shared/file/file_utils.hpp"
#include "shared/info/info.hpp"

#define TYRA_LOG(...) TyraDebug::writeLines("LOG: ", ##__VA_ARGS__, "\n")
#define TYRA_WARN(...) TyraDebug::writeLines("==WARN: ", ##__VA_ARGS__, "\n")
#define TYRA_ERROR(...) TyraDebug::writeLines("====ERR: ", ##__VA_ARGS__, "\n")
#define TYRA_TRAP(...) TyraDebug::trap(__FILE__, __LINE__, ##__VA_ARGS__)
#define TYRA_BREAKPOINT() TyraDebug::trap(__FILE__, __LINE__, "Breakpoint")
#define TYRA_ASSERT(condition, ...) \
  if (!(condition)) TyraDebug::trap(__FILE__, __LINE__, ##__VA_ARGS__)

class TyraDebug {
 public:
  template <typename Arg, typename... Args>
  static void writeLines(Arg&& arg, Args&&... args) {
    std::stringstream ss;

    ss << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(ss << std::forward<Args>(args)), 0)...};

    if (Tyra::Info::writeLogsToFile) {
      auto* logFile = getLogFile();
      *logFile << ss.str();
      logFile->flush();
    } else {
      printf("%s", ss.str().c_str());
    }
  }

  template <typename... Args>
  static void trap(const char* file, int line, Args... args) {
    std::stringstream ss1;
    ss1 << "\n";
    ss1 << "==============  TYRA  ==============\n";
    ss1 << "| Assertion failed!\n";
    ss1 << "|\n";

    if (Tyra::Info::writeLogsToFile) {
      auto* logFile = getLogFile();
      *logFile << ss1.str();
      logFile->flush();
    } else {
      printf("%s", ss1.str().c_str());
    }

    writeAssertLines(args...);

    std::stringstream ss2;
    ss2 << "|\n";
    ss2 << "| File : " << file << ":" << line << "\n";
    ss2 << "====================================\n\n";

    if (Tyra::Info::writeLogsToFile) {
      auto* logFile = getLogFile();
      *logFile << ss2.str();
      logFile->flush();
    } else {
      printf("%s", ss2.str().c_str());
    }

    for (;;) {
    }
  }

 private:
  static std::unique_ptr<std::ofstream> logFile;
  static std::ofstream* getLogFile();

  template <typename Arg, typename... Args>
  static void writeAssertLines(Arg&& arg, Args&&... args) {
    std::stringstream ss;

    ss << "| " << std::forward<Arg>(arg) << "\n";
    using expander = int[];
    (void)expander{
        0, (void(ss << "| " << std::forward<Args>(args) << "\n"), 0)...};

    if (Tyra::Info::writeLogsToFile) {
      auto* logfile = getLogFile();
      *logfile << ss.str();
      logFile->flush();
    } else {
      printf("%s", ss.str().c_str());
    }
  }
};

#endif  // NDEBUG