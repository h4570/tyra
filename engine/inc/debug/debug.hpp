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
#include <utility>

#define TYRA_LOG(...) Debug::writeLines("LOG: ", ##__VA_ARGS__, "\n")
#define TYRA_WARN(...) Debug::writeLines("==WARN: ", ##__VA_ARGS__, "\n")
#define TYRA_ERROR(...) Debug::writeLines("====ERR: ", ##__VA_ARGS__, "\n")
#define TYRA_TRAP(...) Debug::trap(__FILE__, __LINE__, ##__VA_ARGS__)
#define TYRA_BREAKPOINT() Debug::trap(__FILE__, __LINE__, "Breakpoint")
#define TYRA_ASSERT(condition, ...) \
  if (!(condition)) Debug::trap(__FILE__, __LINE__, ##__VA_ARGS__)

class Debug {
 public:
  template <typename Arg, typename... Args>
  static void writeLines(Arg&& arg, Args&&... args) {
    std::stringstream ss;

    ss << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(ss << std::forward<Args>(args)), 0)...};

    printf("%s", ss.str().c_str());
  }

  template <typename... Args>
  static void trap(const char* file, int line, Args... args) {
    printf("\n");
    printf("==============  TYRA  ==============\n");
    printf("| Assertion failed!\n");
    printf("|\n");
    writeAssertLines(args...);
    printf("|\n");
    printf("| File : %s:%d\n", file, line);
    printf("====================================\n\n");
    for (;;) {
    }
  }

 private:
  template <typename Arg, typename... Args>
  static void writeAssertLines(Arg&& arg, Args&&... args) {
    std::stringstream ss;

    ss << "| " << std::forward<Arg>(arg) << "\n";
    using expander = int[];
    (void)expander{
        0, (void(ss << "| " << std::forward<Args>(args) << "\n"), 0)...};

    printf("%s", ss.str().c_str());
  }
};

#endif  // NDEBUG