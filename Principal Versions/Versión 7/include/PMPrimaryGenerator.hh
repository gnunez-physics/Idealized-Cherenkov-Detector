/**
 * @file PMPrimaryGenerator.hh
 * @brief Encabezado para la configuración de la fuente de partículas primarias.
 */

#ifndef PMPRIMARYGENERATOR_HH
#define PMPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"

/**
 * @class PMPrimaryGenerator
 * @brief Gestiona la creación y disparo de las partículas primarias en cada evento.
 */
class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PMPrimaryGenerator();
    ~PMPrimaryGenerator();
    
    // Función obligatoria que dispara la partícula al inicio de cada evento
    virtual void GeneratePrimaries(G4Event *anEvent);

private:
    G4ParticleGun *fParticleGun; // Puntero al cañón de partículas de Geant4
};

#endif // PMPRIMARYGENERATOR_HH