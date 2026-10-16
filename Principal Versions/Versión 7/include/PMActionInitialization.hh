/**
 * @file PMActionInitialization.hh
 * @brief Encabezado para la inicialización de acciones de usuario en Geant4.
 */

#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "PMPrimaryGenerator.hh"
#include "run.hh"

/**
 * @class PMActionInitialization
 * @brief Gestiona el registro de acciones (como el generador y el run action) para cada hilo.
 */
class PMActionInitialization : public G4VUserActionInitialization
{
public:
    PMActionInitialization();
    ~PMActionInitialization();

    // Configuración para el hilo principal (Master) en modo multihilo
    virtual void BuildForMaster() const;

    // Configuración de las acciones para cada hilo de trabajo
    virtual void Build() const;
};

#endif // PMACTIONINITIALIZATION_HH