/**
 * @file PMDetectorConstruction.cc
 * @brief Construcción geométrica del mundo y los detectores para Geant4.
 * 
 * Este archivo define todo el escenario físico de la simulación:
 * 1. El volumen del "Mundo" (aire).
 * 2. El cilindro de agua central (el medio donde interactúan las partículas).
 * 3. Las propiedades ópticas del medio (índice de refracción para Cherenkov).
 * 4. La matriz de detectores (píxeles o PMTs) que registran los fotones.
 */

#include "PMDetectorConstruction.hh"
#include "PMDetector.hh"

PMDetectorConstruction::PMDetectorConstruction() {}
PMDetectorConstruction::~PMDetectorConstruction() {}

/**
 * @brief Construye todo el mundo físico y los volúmenes de la simulación.
 * @return G4VPhysicalVolume* El volumen físico raíz que contiene a todo el universo.
 */
G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;
    G4NistManager *nist = G4NistManager::Instance();

    // =========================================================================
    // INTERRUPTORES DE CONFIGURACIÓN (TRUE = ENCENDIDO / FALSE = APAGADO)
    // =========================================================================
    G4bool includeWaterCylinder    = true; // Activa o desactiva el cilindro central
    G4bool includeCherenkovPhysics = true; // Activa o desactiva el índice de refracción/luz
    G4bool includeDetectorGrid     = true; // Activa o desactiva la matriz de detectores (PMTs)

    // ---------------------------------------------------------
    // 1. UNIVERSO (MUNDO) - Siempre necesario como contenedor base
    // ---------------------------------------------------------
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    
    G4double worldSize = 1 * m;
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    
    G4VPhysicalVolume *physWorld = new G4PVPlacement(
        0, G4ThreeVector(0., 0., 0.),
        logicWorld, "physWorld",
        0, false, 0, checkOverlaps
    );

    // ---------------------------------------------------------
    // 2. CILINDRO DE AGUA (Opcional)
    // ---------------------------------------------------------
    G4LogicalVolume *logicWaterCylinder = nullptr;

    if (includeWaterCylinder) {    
        G4Material *waterMat = nist->FindOrBuildMaterial("G4_WATER");

        G4Tubs *solidWaterCylinder = new G4Tubs(
            "solidWaterCylinder",
            0. * cm,            
            0.02 * m, // Radio del cilindro       
            0.001 * m, // Mitad de la altura (desde el centro Z)      
            0. * deg, 360. * deg
        );

        logicWaterCylinder = new G4LogicalVolume(solidWaterCylinder, waterMat, "logicWaterCylinder");

        new G4PVPlacement(
            0, G4ThreeVector(0., 0., 0.),
            logicWaterCylinder, "physWaterCylinder",
            logicWorld, false, 0, checkOverlaps
        );
    }

    // ---------------------------------------------------------
    // 3. PROPIEDADES ÓPTICAS (Opcional, requiere el cilindro activo)
    // ---------------------------------------------------------
    G4double photonEnergy[2] = { 1.0 * eV, 4.0 * eV };

    // Configuración base para el aire del mundo
    G4double rindexWorld[2] = { 1.0, 1.0 };
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", photonEnergy, rindexWorld, 2);
    logicWorld->GetMaterial()->SetMaterialPropertiesTable(mptWorld);

    // Inyección de propiedades ópticas del agua para la radiación Cherenkov
    if (includeCherenkovPhysics && logicWaterCylinder != nullptr) {
        G4double rindexWater[2] = { 1.33, 1.33 };
        G4double absorptionWater[2] = { 2.0 * m, 2.0 * m };

        G4MaterialPropertiesTable *mptWater = new G4MaterialPropertiesTable();
        mptWater->AddProperty("RINDEX", photonEnergy, rindexWater, 2);
        mptWater->AddProperty("ABSLENGTH", photonEnergy, absorptionWater, 2);

        logicWaterCylinder->GetMaterial()->SetMaterialPropertiesTable(mptWater);
    }

    // ---------------------------------------------------------
    // 4. MATRIZ DE DETECTORES (Opcional - Rejilla 100x100 de píxeles/PMTs)
    // ---------------------------------------------------------
    if (includeDetectorGrid) {
        G4Box *solidDetector = new G4Box("solidDetector", 0.005 * m, 0.005 * m, 0.01 * m);
        logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

        for (G4int i = 0; i < 100; i++) {
            for (G4int j = 0; j < 100; j++) {
                new G4PVPlacement(
                    0, 
                    G4ThreeVector(-0.5 * m + (i + 0.5) * m / 100, -0.5 * m + (j + 0.5) * m / 100, -0.08 * m), 
                    logicDetector, 
                    "physDetector", 
                    logicWorld, 
                    false, 
                    j + i * 100, 
                    false
                );
            }
        }
    }

    return physWorld;
}

/**
 * @brief Vincula los detectores físicos con la lógica de sensibilidad de impactos.
 * Se ejecuta automáticamente después de crear la geometría.
 */
void PMDetectorConstruction::ConstructSDandField()
{
    // Solo configuramos el detector sensible si la malla de detectores está encendida
    if (logicDetector != nullptr) {
        MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
        logicDetector->SetSensitiveDetector(sensDet);
    }
}