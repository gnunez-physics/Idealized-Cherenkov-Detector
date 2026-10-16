#include "Photo.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "Randomize.hh"
#include <fstream> 

PMPhotoDetector::PMPhotoDetector(G4String name) : G4VSensitiveDetector(name) 
{
	quEff = new G4PhysicsFreeVector();

	std::ifstream datafile;
	datafile.open("eff.dat");

	while(1)
	{
		G4double wlen, queff;
		datafile >> wlen >> queff;

		if(datafile.eof())
			break;

		G4cout << wlen << " " << queff << G4endl;
		
		quEff->InsertValues(wlen, queff/100.);
	}

	datafile.close();
}

PMPhotoDetector::~PMPhotoDetector()
{}

G4bool PMPhotoDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4Track *track = aStep->GetTrack();

    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        
        // 1. Declaramos y obtenemos la posición y el momento correctamente
        G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
        G4ThreeVector momentum = track->GetMomentumDirection();

        // 2. Filtro geométrico: Solo procesa si impacta la cara superior y va hacia abajo
        if (pos.z() >= -0.19961 * m && momentum.z() < 0)
        {
            G4double photonEnergy = track->GetKineticEnergy();
            G4double photonTime = track->GetGlobalTime();

            // Convertir la energía del fotón a longitud de onda en nm para consultar la eficiencia
            G4double wavelength = 1239.84193 / (photonEnergy / eV);

            if(G4UniformRand() < quEff->Value(wavelength))
            {
                G4AnalysisManager *man = G4AnalysisManager::Instance();
                man->FillNtupleDColumn(1, 0, photonEnergy); 
                man->FillNtupleDColumn(1, 1, photonTime);   
                man->AddNtupleRow(1);
            }
        }
        
        // 3. El fotón siempre se detiene al tocar el fotocátodo
        track->SetTrackStatus(fStopAndKill);
    }

    return true;
}