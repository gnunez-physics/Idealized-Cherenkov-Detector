/**
 * @file PMPhysicsList.cc
 * @brief Configuración de los modelos físicos que actúan en la simulación.
 * 
 * Este archivo define qué interacciones físicas están permitidas, como el paso 
 * de partículas cargadas por la materia (electromagnética) y la generación 
 * de luz (física óptica para Cherenkov y centelleo).
 */

#include "PMPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

/**
 * @brief Constructor: Registra los paquetes de física de Geant4.
 * Incluye interruptores para controlar qué comportamientos simular.
 */
PMPhysicsList::PMPhysicsList()
{
    // =========================================================================
    // INTERRUPTORES DE CONFIGURACIÓN (TRUE = ENCENDIDO / FALSE = APAGADO)
    // =========================================================================
    G4bool enableStandardEM  = true; // Activa la física electromagnética (ionización, etc.)
    G4bool enableOptical     = true; // Activa la física óptica (fotones Cherenkov y centelleo)

    // 1. Registro de física electromagnética estándar
    if (enableStandardEM) {
        RegisterPhysics(new G4EmStandardPhysics());
    }

    // 2. Registro de física óptica (necesaria para ver los "conos verdes")
    if (enableOptical) {
        RegisterPhysics(new G4OpticalPhysics());
    }
}

// Destructor estándar (sin uso de memoria dinámica manual por ahora)
PMPhysicsList::~PMPhysicsList()
{
}