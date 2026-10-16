/**
 * @file PMDetector.cc
 * @brief Implementación del detector sensible para el registro de impactos.
 * 
 * Este archivo gestiona qué sucede cuando una partícula (como un fotón Cherenkov)
 * choca contra los elementos de la matriz de detectores (PMTs). Se encarga de:
 * 1. Simular la absorción del fotón al impactar el sensor.
 * 2. Extraer la posición geométrica y el número de identificación (Copy Number) de la celda.
 * 3. Registrar y almacenar los datos en un archivo ROOT para su posterior análisis.
 */

#include "PMDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name) {}

MySensitiveDetector::~MySensitiveDetector() {}

/**
 * @brief Se ejecuta automáticamente cada vez que una partícula ingresa al volumen sensible.
 */
G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // =========================================================================
    // INTERRUPTORES DE CONFIGURACIÓN (TRUE = ENCENDIDO / FALSE = APAGADO)
    // =========================================================================
    G4bool killPhoton = true;  // true = Absorbe el fotón al tocar el detector (comportamiento de PMT)
    G4bool fillNtuple = true;  // true = Guarda los datos de posición en el archivo ROOT
    G4bool verbose    = false; // true = Imprime coordenadas detalladas en la terminal (lento)

    // 1. Obtener la información del trayecto (Track) de la partícula
    G4Track *track = aStep->GetTrack();

    // Si está activo, destruye la partícula al impactar para simular la detección real
    if (killPhoton) {
        track->SetTrackStatus(fStopAndKill);
    }

    // 2. Extraer la información geométrica del impacto y de la celda (Copy Number)
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();
    
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    G4ThreeVector posPhoton = aStep->GetPreStepPoint()->GetPosition();

    // 3. Impresiones opcionales en terminal (solo si verbose está en true)
    if (verbose) {
        G4cout << "Photon position: " << posPhoton << G4endl;
        G4cout << "Copy number: " << copyNo << G4endl;
        G4cout << "Detector position: " << posDetector << G4endl;
    }

    // 4. Almacenamiento de datos en el archivo ROOT
    if (fillNtuple) {
        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->FillNtupleIColumn(0, evt);
        man->FillNtupleDColumn(1, posDetector[0]);
        man->FillNtupleDColumn(2, posDetector[1]);
        man->FillNtupleDColumn(3, posDetector[2]);
        man->AddNtupleRow(0);
    }

    return true;
}