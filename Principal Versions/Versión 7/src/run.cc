#include "run.hh"
#include "G4AnalysisManager.hh"
#include <sstream>
#include <filesystem> // <-- ¡Importante para que se cree la carpeta solo!

MyRunAction::MyRunAction()
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  man->CreateNtuple("Hits", "Hits");
  man->CreateNtupleIColumn("fEvent");
  man->CreateNtupleDColumn("fX");
  man->CreateNtupleDColumn("fY");
  man->CreateNtupleDColumn("fZ");
  man->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{
}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
  // 1. Crea la carpeta automáticamente si no existe (como en tu versión antigua)
  std::filesystem::create_directories("output");

  G4AnalysisManager* man = G4AnalysisManager::Instance();

  G4int runNumber = run->GetRunID();

  std::stringstream strRunID;
  strRunID << runNumber; 

  man->OpenFile("output/output" + strRunID.str() + ".root");
}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  man->Write();
  man->CloseFile();
}