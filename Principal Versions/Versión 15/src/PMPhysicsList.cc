#include "PMPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

PMPhysicsList::PMPhysicsList()
{
    // Interruptores
    G4bool enableStandardEM  = true; // Activa la física electromagnética 
    G4bool enableOptical     = true; // Activa la física óptica (fotones Cherenkov y centelleo)

    // 1. Registro de física electromagnética estándar
    if (enableStandardEM) {
        RegisterPhysics(new G4EmStandardPhysics());
    }

    // 2. Registro de física óptica (necesaria para ver los "conos verdes")
    if (enableOptical) {
        RegisterPhysics(new G4OpticalPhysics());
    }
}

// Destructor estándar (sin uso de memoria dinámica manual por ahora)
PMPhysicsList::~PMPhysicsList()
{
}