#include "PMActionInitialization.hh"
#include "PMPrimaryGenerator.hh"
#include "PMRun.hh"

// Constructor y destructor estándar
PMActionInitialization::PMActionInitialization() {}
PMActionInitialization::~PMActionInitialization() {}

// Configuración opcional para simulaciones paralelas (multihilo)
void PMActionInitialization::BuildForMaster() const 
{
	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction);
}

void PMActionInitialization::Build() const
{
	PMPrimaryGenerator *generator = new PMPrimaryGenerator();
	SetUserAction(generator);

	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction);
    
}