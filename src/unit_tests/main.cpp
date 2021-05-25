/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

// To run unit tests
// 1. Make sure that your $(PCSX2_PATH) and $(WSL_TYRA_PATH) Makefile variables are correct
// 2. make && make run-pcsx2 && make show
// 3. Click 'X' in PCSX2 when screen will be on memory card selection stage
// 4. Voila :)

const int CUSTOM_ARGC = 5;
char const *CUSTOM_ARGV[] = {
    "",
    "--reporter",
    "console",
    "--out",
    "test-result.txt"};

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_NO_POSIX_SIGNALS

#include "catch.hpp"
