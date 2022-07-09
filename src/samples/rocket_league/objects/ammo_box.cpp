/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "ammo_box.hpp"

// ----
// Constructors/Destructors
// ----

AmmoBox::AmmoBox(TextureRepository *t_texRepo)
{
    consoleLog("Creating beach ball object");
    texRepo = t_texRepo;
    mesh.loadMD2("meshes/ammobox/", "ammobox", .5F, true);
    mesh.position.set(20.00F, 20.00F, 40.00F);
    mesh.rotation.x = -1.566F;
    mesh.rotation.z = 1.566F;
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = true;
    texRepo->addByMesh("meshes/ammobox/", mesh, BMP);
    consoleLog("Beach ball object created!");
}

AmmoBox::~AmmoBox()
{
}

// ----
// Methods
// ----
