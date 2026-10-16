/**
 * @file PMPhysicsList.hh
 * @brief Encabezado para la lista de modelos físicos de la simulación.
 */

#ifndef PMPHYSICSLISTS_HH
#define PMPHYSICSLISTS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

/**
 * @class PMPhysicsList
 * @brief Define los procesos físicos (electromagnéticos y ópticos) aplicados en el mundo virtual.
 */
class PMPhysicsList : public G4VModularPhysicsList
{
public:
    PMPhysicsList();
    ~PMPhysicsList();
};

#endif // PMPHYSICSLISTS_HH