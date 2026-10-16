#ifndef DETECTOR_HH
#define DETECTOR_HH

// Herramienta fundamental en Geant 4 para convertir un volumen geométrico en un detector real capaz de registrar datos
#include "G4VSensitiveDetector.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(G4String name);
    ~MySensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
};

#endif