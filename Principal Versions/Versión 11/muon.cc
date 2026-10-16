#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "PMPhysicsList.hh"
#include "PMDetectorConstruction.hh"
#include "PMActionInitialization.hh"

int main(int argc, char** argv)
{

    // 1. Configuración del administrador de ejecuciones (Multihilo o Secuencial)
    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager;
	runManager->SetNumberOfThreads(8);
    #else
        G4RunManager *runManager = new G4RunManager;
    #endif

    // Registro de los componentes principales de la simulación
    runManager->SetUserInitialization(new PMPhysicsList());
    runManager->SetUserInitialization(new PMDetectorConstruction());
    runManager->SetUserInitialization(new PMActionInitialization());

    // 2. Inicialización del sistema de visualización gráfica
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();
	

    // 3. Obtención del gestor de comandos globales
    G4UImanager *UImanager = G4UImanager::GetUIpointer();


    /// 4. Detección inteligente de modo (Batch vs. Interactivo)
    G4UIExecutive *ui = nullptr;
    if (argc == 1) {
        // Si no hay argumentos adicionales, se inicializa la interfaz gráfica
        ui = new G4UIExecutive(argc, argv);
    }

    // 5. Evaluación de ejecución según el modo detectado
    if (!ui) {
        // MODO BATCH (Sin interfaz gráfica): Se ejecuta el macro pasado por consola (ej. ./muon run.mac)
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } 
    else {
        // MODO INTERACTIVO (Con interfaz gráfica): Carga vis.mac por defecto y abre la consola visual
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Limpieza y salida limpia del programa
    delete visManager;
    delete runManager;

    return 0;
}