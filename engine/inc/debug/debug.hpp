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
#include <debug.h>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <memory>

#include "file/file_utils.hpp"
#include "info/info.hpp"

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
      writeInLogFile(&ss);
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
      writeInLogFile(&ss1);
    } else {
      printf("%s", ss1.str().c_str());
    }

    writeAssertLines(args...);

    std::stringstream ss2;
    ss2 << "|\n";
    ss2 << "| File : " << file << ":" << line << "\n";
    ss2 << "====================================\n\n";

    if (Tyra::Info::writeLogsToFile) {
      writeInLogFile(&ss2);
    } else {
      printf("%s", ss2.str().c_str());
    }

    init_scr();
    for (;;) {
      scr_setXY(20, 10);
      scr_printf(ss1.str().c_str());
      writeAssertLinesInScreen(args...);
      scr_printf(ss2.str().c_str());
    }
  }

 private:
  static void writeInLogFile(std::stringstream* ss);

  template <typename Arg, typename... Args>
  static void writeAssertLines(Arg&& arg, Args&&... args) {
    std::stringstream ss;

    ss << "| " << std::forward<Arg>(arg) << "\n";
    using expander = int[];
    (void)expander{
        0, (void(ss << "| " << std::forward<Args>(args) << "\n"), 0)...};

    if (Tyra::Info::writeLogsToFile) {
      writeInLogFile(&ss);
    } else {
      printf("%s", ss.str().c_str());
    }
  }

  template <typename Arg, typename... Args>
  static void writeAssertLinesInScreen(Arg&& arg, Args&&... args) {
    std::stringstream ss;

    ss << "| " << std::forward<Arg>(arg) << "\n";
    using expander = int[];
    (void)expander{
        0, (void(ss << "| " << std::forward<Args>(args) << "\n"), 0)...};

    scr_printf(ss.str().c_str());
  }
};

#endif  // NDEBUG