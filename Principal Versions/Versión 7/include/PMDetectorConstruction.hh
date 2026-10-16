/**
 * @file PMDetectorConstruction.hh
 * @brief Encabezado para la construcción de la geometría y detectores de la simulación.
 */

#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "PMDetector.hh"

/**
 * @class PMDetectorConstruction
 * @brief Define y construye todo el escenario físico (mundo, cilindro de agua y rejilla de detectores).
 */
class PMDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    PMDetectorConstruction();
    virtual ~PMDetectorConstruction();

    // Función principal que construye toda la geometría del mundo físico
    virtual G4VPhysicalVolume *Construct();

private:
    // Puntero al volumen lógico de los detectores para enlazarlos con el sensor
    G4LogicalVolume *logicDetector;

    // Método obligatorio de Geant4 para registrar los detectores sensibles y campos
    virtual void ConstructSDandField();
};

#endif // PMDETECTORCONSTRUCTION_HH