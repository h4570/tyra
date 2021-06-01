/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "race_track.hpp"

// ----
// Constructors/Destructors
// ----

RaceTrack::RaceTrack(TextureRepository *t_texRepo)
{
    consoleLog("Creating race tack object");
    texRepo = t_texRepo;
    mesh.loadObj("meshes/football/", "football", 30.0F, true);
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = false;
    texRepo->addByMesh("meshes/football/", mesh, PNG);
    consoleLog("Race track object created!");
}

RaceTrack::~RaceTrack()
{
}

// ----
// Methods
// ----
