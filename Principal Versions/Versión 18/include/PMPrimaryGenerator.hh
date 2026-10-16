#ifndef PMPRIMARYGENERATOR_HH
#define PMPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PMPrimaryGenerator();
    ~PMPrimaryGenerator();
    
    // Función obligatoria que dispara la partícula al inicio de cada evento
    virtual void GeneratePrimaries(G4Event *anEvent);

private:
	G4ParticleGun *fParticleGun; // Puntero al cañón de partículas de Geant4
	G4ParticleGun *fParticleGun2;
};

#endif