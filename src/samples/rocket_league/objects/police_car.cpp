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

#define POLICE_CAR_PRE_Z_ROTATION 0.0F
#define POLICE_CAR_PRE_X_ROTATION -1.566F

// ----
// Constructors/Destructors
// ----

PoliceCar::PoliceCar(TextureRepository *t_texRepo)
    : Car(POLICE_CAR_PRE_Z_ROTATION, -6.266F)
{
    consoleLog("Creating police car object");
    texRepo = t_texRepo;
    mesh.loadMD2("meshes/police_car/", "policecar", 0.45F, false);
    mesh.position.set(0.00F, 0.00F, 40.00F);
    mesh.rotation.x = POLICE_CAR_PRE_X_ROTATION;
    mesh.rotation.z = POLICE_CAR_PRE_Z_ROTATION;
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

void PoliceCar::update(const Pad &t_pad)
{
    // TODO, replace pad by AI
    drive(t_pad.lJoyV <= 100, t_pad.lJoyV >= 200, t_pad.lJoyH <= 100, t_pad.lJoyH >= 200);
}
