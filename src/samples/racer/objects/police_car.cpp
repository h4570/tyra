/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "police_car.hpp"

// ----
// Constructors/Destructors
// ----

PoliceCar::PoliceCar(TextureRepository *t_texRepo)
{
    consoleLog("Creating police car object");
    texRepo = t_texRepo;
    mesh.loadMD2("meshes/police_car/", "policecar", 0.45F, false);
    mesh.position.set(0.00F, 40.00F, 40.00F);
    mesh.rotation.x = -1.566F;
    mesh.rotation.z = 1.566F;
    mesh.shouldBeBackfaceCulled = false;
    mesh.shouldBeFrustumCulled = true;
    texRepo->addByMesh("meshes/police_car/", mesh, PNG);
    consoleLog("Police car object created!");
}

PoliceCar::~PoliceCar()
{
}

// ----
// Methods
// ----
