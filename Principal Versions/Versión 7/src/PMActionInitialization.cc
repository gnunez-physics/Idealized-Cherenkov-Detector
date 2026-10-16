/**
 * @file PMActionInitialization.cc
 * @brief Inicializador de acciones de usuario para Geant4.
 * 
 * Este archivo actúa como el "puente de mando" que le indica a Geant4 qué 
 * clases de acciones debe usar en cada hilo de la simulación (por ejemplo, 
 * el cañón de partículas y el registro de ejecuciones/runs).
 */

#include "PMActionInitialization.hh"
#include "PMPrimaryGenerator.hh"
#include "run.hh"

// Constructor y destructor estándar
PMActionInitialization::PMActionInitialization() {}
PMActionInitialization::~PMActionInitialization() {}

// Configuración opcional para simulaciones paralelas (multihilo)
void PMActionInitialization::BuildForMaster() const {}

/**
 * @brief Registra y activa las acciones de la simulación.
 * Incluye interruptores booleanos (true/false) para encender o apagar componentes de forma modular.
 */
void PMActionInitialization::Build() const
{
    // =========================================================================
    // INTERRUPTORES DE CONFIGURACIÓN (TRUE = ENCENDIDO / FALSE = APAGADO)
    // =========================================================================
    bool enableGenerator = true;  // Activa o desactiva el cañón de partículas
    bool enableRunAction  = true;  // Activa o desactiva las acciones de ejecución (Run)

    // 1. Configuración del cañón de partículas
    if (enableGenerator) {
        PMPrimaryGenerator *generator = new PMPrimaryGenerator();
        SetUserAction(generator);
    }

    // 2. Configuración de las acciones de ejecución / análisis
    if (enableRunAction) {
        MyRunAction *runAction = new MyRunAction();
        SetUserAction(runAction);
    }
}