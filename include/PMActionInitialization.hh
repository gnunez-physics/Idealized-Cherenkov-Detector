#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

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

#endif 