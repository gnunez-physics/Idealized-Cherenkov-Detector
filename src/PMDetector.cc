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

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	// A continuación se presentan los interruptores de mi PMDetector.cc
	G4bool fillNtuple = true;  // true = Guarda los datos de posición en el archivo ROOT

	// 1. Obtener la información del trayecto (Track) de la partícula y 
	G4Track *track = aStep->GetTrack();

	track->SetTrackStatus(fStopAndKill);
	

	// 2. Se generan variables temporales que se usarán luego, se prefiere este método para luego no hacer lineas kilométricas, a continuación se toman datos sobre el impacto de la/las partículas
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4int copyNo = touchable->GetCopyNumber();
    
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	G4ThreeVector posPhoton = aStep->GetPreStepPoint()->GetPosition();

	// G4cout << "Photon position: " << posPhoton << G4endl;
	// G4cout << "Copy number: " << copyNo << G4endl;
	// G4cout << "Detector position: " << posDetector << G4endl;
	

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