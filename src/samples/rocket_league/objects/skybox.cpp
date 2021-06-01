/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "skybox.hpp"

// ----
// Constructors/Destructors
// ----

Skybox::Skybox(TextureRepository *t_texRepo)
{
    consoleLog("Creating skybox object");
    texRepo = t_texRepo;
    mesh.loadObj("meshes/skybox/", "skybox", 700.0F, false);
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = false;
    texRepo->addByMesh("meshes/skybox/", mesh, BMP);
    consoleLog("Skybox object created!");
}

Skybox::~Skybox()
{
}

// ----
// Methods
// ----

void Skybox::update(const Vector3 &playerPosition)
{
    mesh.position.set(playerPosition);
}