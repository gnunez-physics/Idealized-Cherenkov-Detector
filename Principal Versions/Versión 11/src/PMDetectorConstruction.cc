#include "PMDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4PVPlacement.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"

// Librerías necesarias para la limpieza de memoria geométrica
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

// Librería que permite crear comandos interactivos personalizados en Geant4
#include "G4GenericMessenger.hh"

#include "PMDetector.hh"

PMDetectorConstruction::PMDetectorConstruction() 
{
	// Definir mensajero para configurar parámetros del detector desde macros
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");

	nCols = 100;
	nRows = 100;
	
	DefineMaterials();
}

PMDetectorConstruction::~PMDetectorConstruction() 
{
}

void PMDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();

	// Materiales
	H2O = new G4Material("H2O", 1.000*g/cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);

	// Borosilicato
	// Se le llama Pyrex porque es la marca registrada para este tipo de vidrio, como llamarle nova a la nova
	Pyrex = new G4Material("Pyrex", 2.23*g/cm3, 4);
	Pyrex->AddElement(nist->FindOrBuildElement("B"), 0.040064);  // Fracción de átomos o masa según prefieras
	Pyrex->AddElement(nist->FindOrBuildElement("Si"), 0.372175);
	Pyrex->AddElement(nist->FindOrBuildElement("Na"), 0.017906);
	Pyrex->AddElement(nist->FindOrBuildElement("O"), 0.569855);
	//La suma de todos los porcentajes da 1

	// Fotocátodo (Bialcalino: K2CsSb)
	// Capa delgada encargada de convertir los fotones ópticos (Cherenkov) en fotoelectrones
	Bialcalino = new G4Material("Bialcalino", 3.56*g/cm3, 3);
	Bialcalino->AddElement(nist->FindOrBuildElement("K"), 2);
	Bialcalino->AddElement(nist->FindOrBuildElement("Cs"), 1);
	Bialcalino->AddElement(nist->FindOrBuildElement("Sb"), 1);

	worldMat = nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
	// Esto evita que Geant4 colapse en memoria al reconstruir la geometría al vuelo
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	// Variable para solapamiento
	G4bool checkOverlaps = true;

	// Universo
	G4double xWorld = 0.5 * m;
	G4double yWorld = 0.5 * m;
	G4double zWorld = 0.5 * m;

	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

	// Propiedades ópticas
	
	G4double photonEnergy[2] = { 1.0 * eV, 4.0 * eV };
	G4double rindexWorld[2] = { 1.0, 1.0 };

	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", photonEnergy, rindexWorld, 2);

	logicWorld->GetMaterial()->SetMaterialPropertiesTable(mptWorld);

	// Cilindro de agua
	G4LogicalVolume *logicWaterCylinder = nullptr;

	solidWaterCylinder = new G4Tubs(
		"solidWaterCylinder",
		0. * cm,            
		0.01 * m,       
		0.005 * m,      
		0. * deg, 360. * deg
	);

	logicWaterCylinder = new G4LogicalVolume(solidWaterCylinder, H2O, "logicWaterCylinder");

	physWaterCylinder = new G4PVPlacement(
		0, G4ThreeVector(0., 0., 0.),
		logicWaterCylinder, "physWaterCylinder",
		logicWorld, false, 0, checkOverlaps
	);

	// Para el agua
	G4double rindexWater[2] = { 1.33, 1.33 };
	G4double absorptionWater[2] = { 2.0 * m, 2.0 * m };
	G4MaterialPropertiesTable *mptWater = new G4MaterialPropertiesTable();

	mptWater->AddProperty("RINDEX", photonEnergy, rindexWater, 2);
	mptWater->AddProperty("ABSLENGTH", photonEnergy, absorptionWater, 2);

	logicWaterCylinder->GetMaterial()->SetMaterialPropertiesTable(mptWater);

	// Matriz de detectores
	
	// Tamaño total que ocupará la rejilla en los ejes X e Y
	G4double totalGridSizeX = 1. * m;
	G4double totalGridSizeY = 1. * m;

	// Dimensiones individuales de cada celda para garantizar cuadrados exactos
	G4double sizeX = totalGridSizeX / nRows;
	G4double sizeY = totalGridSizeY / nCols;
	G4double thicknessZ = 0.005 * m; // Espesor en Z

	// G4Box requiere las MITADES de las dimensiones (half-widths)
	solidDetector = new G4Box("solidDetector", 0.5 * sizeX, 0.5 * sizeY, 0.5 * thicknessZ);
	// Rejillas de aire
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	for (G4int i = 0; i < nRows; i++) {
		for (G4int j = 0; j < nCols; j++) 
		{ 
			// Cálculo de posición simétrica centrada sin solapamientos
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;
			G4double posZ = -0.1 * m;

			physDetector = new G4PVPlacement(
				0, 
				G4ThreeVector(posX, posY, posZ), 
				logicDetector, 
				"physDetector", 
				logicWorld, 
				false, 
				j + i * nCols, 
				checkOverlaps
			);
		}
	}

	// Vidrio que cubre al fotocatodo
	solidGlass = new G4Box("solidGlass", 0.02 * m, 0.02 * m, 0.0001 * m);
	logicGlass = new G4LogicalVolume(solidGlass, Pyrex, "logicGlass");
	physGlass = new G4PVPlacement(
		0, G4ThreeVector(0.06 *m, 0.06 *m, -0.05 *m),
		logicGlass, "physGlass",
		logicWorld, false, 0, checkOverlaps
	);

	// Propiedades ópticas para el vidrio
	G4double rindexGlass[2] = {1.47, 1.47};
	G4double absorptionGlass[2] = {0.3 *m, 0.3 *m};

	G4MaterialPropertiesTable *mptGlass = new G4MaterialPropertiesTable();
	mptGlass->AddProperty("RINDEX", photonEnergy, rindexGlass, 2);
	mptGlass->AddProperty("ABSLENGTH", photonEnergy, absorptionGlass, 2);
	
	logicGlass->GetMaterial()->SetMaterialPropertiesTable(mptGlass);

	// Fotocátodo que transforma la radiación Cherenkov en electrones 
	solidPhotocathode = new G4Box("solidPhotocathode", 0.02 * m, 0.02 * m, 0.0001 * m);
	// Prueba para ver que ocurre si el espesor aumenta, ya que con tan poco espesor del Fotocatodo los Hits prácticamente no cambiaron de una circunferencia
	// solidPhotocathode = new G4Box("solidPhotocathode", 0.02 * m, 0.02 * m, 0.0045 * m);
	logicPhotocathode = new G4LogicalVolume(solidPhotocathode, Bialcalino, "logicPhotocathode");
	physPhotocathode = new G4PVPlacement(
		0, G4ThreeVector(0.07 *m, 0.07 *m, -0.055 *m),		
		logicPhotocathode, "physPhotocathode",
		logicWorld, false, 0, checkOverlaps
	);

	// Propiedades ópticas para el fotocatodo
	G4double PhotocathodeEnergy[2] = { 2.03*eV, 4.13*eV }; // Espectro visible/UV cercano

	// Cantidad de partículas que se atrapan se convierten en señal eléctrica
	G4double PhotocathodeEfficiency[2] = { 0.20, 0.25 };   // 20% - 25% de eficiencia cuántica típica
	G4double PhotocathodeReflectivity[2] = { 0.0, 0.0 };  // Asumimos que no refleja, absorbe para convertir
	G4double PhotocathodeRindex[2] = {2.4, 2.4};

	G4MaterialPropertiesTable* mptPhotocathode = new G4MaterialPropertiesTable();
	mptPhotocathode->AddProperty("RINDEX", PhotocathodeEnergy, PhotocathodeRindex, 2);
	mptPhotocathode->AddProperty("EFFICIENCY", PhotocathodeEnergy, PhotocathodeEfficiency, 2);
	mptPhotocathode->AddProperty("REFLECTIVITY", PhotocathodeEnergy, PhotocathodeReflectivity, 2);

	logicPhotocathode->GetMaterial()->SetMaterialPropertiesTable(mptPhotocathode);
	
	return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
	if (logicDetector != nullptr) {
		MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
		logicDetector->SetSensitiveDetector(sensDet);
	}
}