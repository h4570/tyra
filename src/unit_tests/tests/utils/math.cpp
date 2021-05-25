/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <catch.hpp>
#include <utils/math.hpp>

SCENARIO("min(a,b) greater 'a' should return 'b'", "[math.cpp]")
{
    REQUIRE(Math::min(10, 5) == 5);
}

SCENARIO("min(a,b) greater 'b' should return 'a'", "[math.cpp]")
{
    REQUIRE(Math::min(-5, 5) == -5);
}
