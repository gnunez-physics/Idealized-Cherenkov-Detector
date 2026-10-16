/**
 * @file PMDetector.hh
 * @brief Encabezado para el detector sensible encargado de registrar impactos.
 */

#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

/**
 * @class MySensitiveDetector
 * @brief Registra los impactos y la posición de las partículas en la matriz de detectores.
 */
class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(G4String name);
    ~MySensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
};

#endif // DETECTOR_HH