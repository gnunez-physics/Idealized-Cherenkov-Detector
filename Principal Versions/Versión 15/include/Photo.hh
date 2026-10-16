#ifndef PHOTO_HH
#define PHOTO_HH

#include "G4VSensitiveDetector.hh"

class PMPhotoDetector : public G4VSensitiveDetector
{
public:
    PMPhotoDetector(G4String name);
    ~PMPhotoDetector();

private:
    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);


	// Versión 15 añadido
	G4PhysicsFreeVector *quEff;
};

#endif