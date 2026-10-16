/**
 * @file run.hh
 * @brief Encabezado para la gestión de las acciones al inicio y final de las ejecuciones (Runs).
 */

#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

/**
 * @class MyRunAction
 * @brief Inicializa y finaliza el registro de datos analíticos (como archivos ROOT) por cada simulación.
 */
class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction();
    
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);
};

#endif // RUN_HH