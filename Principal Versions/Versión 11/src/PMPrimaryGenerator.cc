#include "PMPrimaryGenerator.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
	// Creamos el cañón configurado para disparar exactamente 1 partícula por evento.
	fParticleGun = new G4ParticleGun(1);

	// Interruptores
	G4bool shootMuon = true; // true = Dispara Muón (mu+) | false = Dispara Gamma (gamma)

	// 1. Posición inicial del cañón (coordenadas X, Y, Z en el espacio)
	G4double x = 0. * m;
	G4double y = 0. * m;
	G4double z = 0.5 * m;
	fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));

	// 2. Dirección de movimiento (hacia abajo en el eje Z negativo)
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));

	// 3. Selección del tipo de partícula y su energía según el interruptor
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = nullptr;

	if (shootMuon) {
		particle = particleTable->FindParticle("mu+");
		fParticleGun->SetParticleEnergy(3. * GeV);
	} else {
		particle = particleTable->FindParticle("gamma");
		fParticleGun->SetParticleEnergy(511. * keV); 
		}

	fParticleGun->SetParticleDefinition(particle);

	

	// ==========================================
	// 2. SEGUNDO CAÑÓN (Fijo, sin opciones de if/else)
	// ==========================================
	fParticleGun2 = new G4ParticleGun(1);

	// Posición y dirección para el segundo cañón
	//fParticleGun2->SetParticlePosition(G4ThreeVector(0.499 * m, 0.499 * m, 0.5 * m));
	// El anterior lo dejé como comentario ya que está al borde del cubo, quiero ver que ocurre al chocar directamente un muon con el vidrio
	fParticleGun2->SetParticlePosition(G4ThreeVector(0.06 * m, 0.06 * m, 0.5 * m));
	fParticleGun2->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));

	// Definimos una partícula fija (por ejemplo, un neutrón o la que prefieras) y su energía
	G4ParticleDefinition *particle2 = particleTable->FindParticle("neutron");
	fParticleGun2->SetParticleDefinition(particle2);
	fParticleGun2->SetParticleEnergy(5. * GeV);



}

	

PMPrimaryGenerator::~PMPrimaryGenerator()
{
	delete fParticleGun;
	delete fParticleGun2;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	// Ordena al cañón crear el vértice primario y lanzar la partícula en el evento actual.
	fParticleGun->GeneratePrimaryVertex(anEvent);
	
	// Esta línea de código genera una segunda partícula, si la dejo en comentario lo desactiva
	fParticleGun2->GeneratePrimaryVertex(anEvent);

}