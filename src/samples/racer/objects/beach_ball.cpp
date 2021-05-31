/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "beach_ball.hpp"

// ----
// Constructors/Destructors
// ----

BeachBall::BeachBall(TextureRepository *t_texRepo)
{
    consoleLog("Creating beach ball object");
    texRepo = t_texRepo;
    mesh.loadObj("meshes/ball/", "ball", .5F, true);
    mesh.position.set(20.00F, 60.00F, 0.00F);
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = true;
    texRepo->addByMesh("meshes/ball/", mesh, PNG);
    consoleLog("Beach ball object created!");
}

BeachBall::~BeachBall()
{
}

// ----
// Methods
// ----
