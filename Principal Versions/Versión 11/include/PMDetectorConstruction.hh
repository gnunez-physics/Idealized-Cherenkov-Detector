#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"   
#include "G4Tubs.hh"
#include "G4Material.hh"

class G4LogicalVolume;
class G4GenericMessenger;

class PMDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    PMDetectorConstruction();
    virtual ~PMDetectorConstruction();

    // Función principal que construye toda la geometría del mundo físico
    virtual G4VPhysicalVolume *Construct();

private:
	// Método obligatorio de Geant4 para registrar los detectores sensibles y campos
	virtual void ConstructSDandField();

	// Variables para definir la cantidad de columnas y filas de la matriz de detectores
	G4int nCols, nRows;

	G4Box *solidWorld, *solidDetector, *solidGlass, *solidPhotocathode;
	G4Tubs *solidWaterCylinder;
	G4LogicalVolume *logicWorld, *logicWaterCylinder, *logicDetector, *logicGlass, *logicPhotocathode;
	G4VPhysicalVolume *physWorld, *physWaterCylinder, *physDetector, *physGlass, *physPhotocathode;

	// A continuación creamos un puntero para el mensajero de comando interactivos
	G4GenericMessenger *fMessenger;

	G4Material *H2O, *worldMat, *Pyrex, *Bialcalino;
	// G4Element *C;

	//Para poder modificar los materiales de mi simulación cuando quiera
	void DefineMaterials();
};

#endif
