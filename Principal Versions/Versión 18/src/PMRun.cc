#include "PMRun.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

#include <sstream>
#include <filesystem> 

// Constructor: Configuración y reserva de espacio para Ntuples
MyRunAction::MyRunAction() : G4UserRunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    // CRÍTICO PARA MULTIHILO: Une los datos de todos los hilos en un solo archivo ROOT
    man->SetNtupleMerging(true); 

    // Ntuple 0: Hits de posición (Detector Grid)
    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(0);

    // =====================================================================
    // Ntuple 1: Energía y Tiempo (Fotocátodo) - Estructura Única Solicitada
    // =====================================================================
    man->CreateNtuple("EnergyHits", "EnergyHits");
    man->CreateNtupleDColumn("fEdep_MeV"); // Columna 0
    man->CreateNtupleDColumn("fTime_ns");  // Columna 1
    man->FinishNtuple(1);
    
    // SE ELIMINARON LOS HISTOGRAMAS PARA EVITAR OBJETOS EXTRAS EN LA RAÍZ
}

// Destructor: Implementación explícita para eliminar el error "vtable" de Clang/macOS
MyRunAction::~MyRunAction()
{
}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    std::filesystem::create_directories("output");
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    G4int runNumber = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runNumber; 

    man->OpenFile("output/output" + strRunID.str() + ".root");
}

void MyRunAction::EndOfRunAction(const G4Run* /*run*/)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}
