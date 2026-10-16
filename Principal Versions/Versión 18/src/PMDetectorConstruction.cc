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

// Librerías para superficies ópticas y fronteras reflectantes
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

// Librerías necesarias para la limpieza de memoria geométrica
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "PMDetector.hh"

PMDetectorConstruction::PMDetectorConstruction() 
{
	nCols = 2;
	nRows = 2;
	
	DefineMaterials();
}

PMDetectorConstruction::~PMDetectorConstruction() 
{
}

void PMDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();

	// 1. Materiales Base
	H2O = new G4Material("H2O", 1.000*g/cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);

	// 2. Borosilicato (Vidrio)
	Pyrex = new G4Material("Pyrex", 2.23*g/cm3, 4);
	Pyrex->AddElement(nist->FindOrBuildElement("B"), 0.040064); 
	Pyrex->AddElement(nist->FindOrBuildElement("Si"), 0.372175);
	Pyrex->AddElement(nist->FindOrBuildElement("Na"), 0.017906);
	Pyrex->AddElement(nist->FindOrBuildElement("O"), 0.569855);

	// 3. Fotocátodo (Bialcalino: K2CsSb)
	Bialcalino = new G4Material("Bialcalino", 3.56*g/cm3, 3);
	Bialcalino->AddElement(nist->FindOrBuildElement("K"), 2);
	Bialcalino->AddElement(nist->FindOrBuildElement("Cs"), 1);
	Bialcalino->AddElement(nist->FindOrBuildElement("Sb"), 1);

	// 4. Tyvek (Polietileno de alta densidad)
	tyvekLocal = new G4Material("TyvekReal", 0.96*g/cm3, 2);
	tyvekLocal->AddElement(nist->FindOrBuildElement("C"), 2);
	tyvekLocal->AddElement(nist->FindOrBuildElement("H"), 4);

	// 5. Resina Epóxica (Laboratorio)
	Epoxica = new G4Material("Epoxica", 1.15 * g/cm3, 3);
	Epoxica->AddElement(nist->FindOrBuildElement("C"), 21);
	Epoxica->AddElement(nist->FindOrBuildElement("H"), 24);
	Epoxica->AddElement(nist->FindOrBuildElement("O"), 4);

	worldMat = nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
	// =====================================================================
	// INTERRUPTORES DE LAS ESTRUCTURAS (true = aparece, false = se omite)
	// =====================================================================
	G4bool useWaterCylinder = true;
	G4bool useDetectorGrid  = false;
	G4bool useGlass         = true;
	G4bool useResina        = true;
	G4bool usePhotocathode  = true;

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

	// Propiedades ópticas del mundo
	G4double photonEnergy[2] = { 1.0 * eV, 4.0 * eV };
	G4double rindexWorld[2] = { 1.0, 1.0 };

	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", photonEnergy, rindexWorld, 2);

	logicWorld->GetMaterial()->SetMaterialPropertiesTable(mptWorld);

	// 1. CILINDRO DE AGUA
	logicWaterCylinder = nullptr;
	if (useWaterCylinder) {
		solidWaterCylinder = new G4Tubs(
			"solidWaterCylinder",
			0. * cm,             
			0.2 * m,       
			0.2 * m,      
			0. * deg, 360. * deg
		);

		logicWaterCylinder = new G4LogicalVolume(solidWaterCylinder, H2O, "logicWaterCylinder");

		physWaterCylinder = new G4PVPlacement(
			0, G4ThreeVector(0., 0., 0.),
			logicWaterCylinder, "physWaterCylinder",
			logicWorld, false, 0, checkOverlaps
		);

		G4double rindexWater[2] = { 1.33, 1.33 };
		G4double absorptionWater[2] = { 2.0 * m, 2.0 * m };
		G4MaterialPropertiesTable *mptWater = new G4MaterialPropertiesTable();

		mptWater->AddProperty("RINDEX", photonEnergy, rindexWater, 2);
		mptWater->AddProperty("ABSLENGTH", photonEnergy, absorptionWater, 2);

		logicWaterCylinder->GetMaterial()->SetMaterialPropertiesTable(mptWater);

		// =====================================================================
		// EFECTO ÓPTICO TYVEK: Superficie de frontera reflectante difusa al 98%
		// =====================================================================
		G4OpticalSurface* tyvekSurface = new G4OpticalSurface("TyvekSurface", unified, groundfrontpainted, dielectric_metal);
		
		G4double tyvekReflectivity[2] = { 0.98, 0.98 }; 
		G4MaterialPropertiesTable* mptTyvek = new G4MaterialPropertiesTable();
		mptTyvek->AddProperty("REFLECTIVITY", photonEnergy, tyvekReflectivity, 2);
		tyvekSurface->SetMaterialPropertiesTable(mptTyvek);

		new G4LogicalSkinSurface("WaterCylinderTyvekSkin", logicWaterCylinder, tyvekSurface);
	}

	// 2. MATRIZ DE DETECTORES (REJILLA)
	logicDetector = nullptr;
	if (useDetectorGrid) {
		G4double totalGridSizeX = 1. * m;
		G4double totalGridSizeY = 1. * m;

		G4double sizeX = totalGridSizeX / nRows;
		G4double sizeY = totalGridSizeY / nCols;
		G4double thicknessZ = 0.005 * m;

		solidDetector = new G4Box("solidDetector", 0.5 * sizeX, 0.5 * sizeY, 0.5 * thicknessZ);
		logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

		for (G4int i = 0; i < nRows; i++) {
			for (G4int j = 0; j < nCols; j++) 
			{ 
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
	}

	// 3. VIDRIO CILÍNDRICO QUE CUBRE AL FOTOCÁTODO (Radio = 0.05 m, Half-Z = 0.0005 m)
	logicGlass = nullptr;
	if (useGlass) {
		solidGlass = new G4Tubs("solidGlass", 0. * cm, 0.05 * m, 0.0005 * m, 0. * deg, 360. * deg);
		logicGlass = new G4LogicalVolume(solidGlass, Pyrex, "logicGlass");
		physGlass = new G4PVPlacement(
			0, G4ThreeVector(0.05 * m, 0.0 * m, -0.1985 * m),
			logicGlass, "physGlass",
			logicWaterCylinder, false, 0, checkOverlaps
		);

		G4double rindexGlass[2] = {1.47, 1.47};
		G4double absorptionGlass[2] = {4.0 * m, 4.0 * m};

		G4MaterialPropertiesTable *mptGlass = new G4MaterialPropertiesTable();
		mptGlass->AddProperty("RINDEX", photonEnergy, rindexGlass, 2);
		mptGlass->AddProperty("ABSLENGTH", photonEnergy, absorptionGlass, 2);
		
		logicGlass->GetMaterial()->SetMaterialPropertiesTable(mptGlass);
	}

	// 4. RESINA ÓPTICA CILÍNDRICA (Radio = 0.05 m, Half-Z = 0.0003 m)
	logicResina = nullptr;
	if (useResina) {
		solidResina = new G4Tubs("solidResina", 0. * cm, 0.05 * m, 0.0003 * m, 0. * deg, 360. * deg);
		logicResina = new G4LogicalVolume(solidResina, Epoxica, "logicResina");
		physResina = new G4PVPlacement(
			0, G4ThreeVector(0.05 * m, 0.0 * m, -0.1993 * m),
			logicResina, "physResina",
			logicWaterCylinder, false, 0, checkOverlaps
		);

		G4double rindexResina[2] = {1.55, 1.55};
		G4double absorptionResina[2] = {4.0 * m, 4.0 * m};

		G4MaterialPropertiesTable *mptResina = new G4MaterialPropertiesTable();
		mptResina->AddProperty("RINDEX", photonEnergy, rindexResina, 2);
		mptResina->AddProperty("ABSLENGTH", photonEnergy, absorptionResina, 2);
		
		logicResina->GetMaterial()->SetMaterialPropertiesTable(mptResina);
	}

	// 5. FOTOCÁTODO CILÍNDRICO (Radio = 0.05 m, Half-Z = 0.0002 m)
	logicPhotocathode = nullptr;
	if (usePhotocathode) {
		solidPhotocathode = new G4Tubs("solidPhotocathode", 0. * cm, 0.05 * m, 0.0002 * m, 0. * deg, 360. * deg);
		logicPhotocathode = new G4LogicalVolume(solidPhotocathode, Bialcalino, "logicPhotocathode");
		
		physPhotocathode = new G4PVPlacement(
			0, 
			G4ThreeVector(0.05 * m, 0.0 * m, -0.1998 * m),		
			logicPhotocathode, "physPhotocathode",
			logicWaterCylinder, false, 0, checkOverlaps
		);

		G4double PhotocathodeEnergy[2] = { 1.91*eV, 4.43*eV }; 
		G4double PhotocathodeEfficiency[2] = { 0.25, 0.25 }; 
		G4double PhotocathodeReflectivity[2] = { 0.0, 0.0 }; 
		G4double PhotocathodeRindex[2] = {2.4, 2.4};

		G4MaterialPropertiesTable* mptPhotocathode = new G4MaterialPropertiesTable();
		mptPhotocathode->AddProperty("RINDEX", PhotocathodeEnergy, PhotocathodeRindex, 2);
		mptPhotocathode->AddProperty("EFFICIENCY", PhotocathodeEnergy, PhotocathodeEfficiency, 2);
		mptPhotocathode->AddProperty("REFLECTIVITY", PhotocathodeEnergy, PhotocathodeReflectivity, 2);

		logicPhotocathode->GetMaterial()->SetMaterialPropertiesTable(mptPhotocathode);
	}
	
	return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
	if (logicDetector != nullptr) {
		MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
		logicDetector->SetSensitiveDetector(sensDet);
	}

	if (logicPhotocathode != nullptr) {
		PMPhotoDetector *sensDet = new PMPhotoDetector("PhotoDetector");
		logicPhotocathode->SetSensitiveDetector(sensDet);
	}
}