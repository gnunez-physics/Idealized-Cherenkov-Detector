/**
 * @file PMPrimaryGenerator.cc
 * @brief Configuración del cañón de partículas primarias para Geant4.
 * 
 * Este archivo define la fuente de radiación o partículas que se dispararán
 * en cada evento de la simulación. Aquí se configuran propiedades clave como:
 * - El tipo de partícula (ej. muón, gamma).
 * - Su energía inicial.
 * - Su posición de origen y dirección de movimiento.
 */

#include "PMPrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"



// =========================================================================
    // OPTIMIZACIÓN DE RENDIMIENTO: CONFIGURACIÓN EN EL CONSTRUCTOR
    // =========================================================================
    // Definir las propiedades del cañón aquí (y no en GeneratePrimaries) 
    // asegura que la búsqueda en la base de datos de partículas y la asignación 
    // de parámetros se realicen una sola vez al iniciar el programa. 
    // Esto evita recalcular lo mismo en cada evento, optimizando drásticamente 
    // el tiempo de CPU al correr simulaciones masivas.
    // =========================================================================



/**
 * @brief Constructor: Inicializa el cañón y define las características del disparo.
 */
PMPrimaryGenerator::PMPrimaryGenerator()
{
    // Creamos el cañón configurado para disparar exactamente 1 partícula por evento.
    fParticleGun = new G4ParticleGun(1);

    // =========================================================================
    // INTERRUPTORES DE CONFIGURACIÓN DE LA FUENTE
    // =========================================================================
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
        fParticleGun->SetParticleEnergy(2. * GeV); // Alta energía para atravesar todo
    } else {
        particle = particleTable->FindParticle("gamma");
        fParticleGun->SetParticleEnergy(511. * keV); // Energía típica de aniquilación/gamma
    }

    fParticleGun->SetParticleDefinition(particle);
}

/**
 * @brief Destructor: Libera la memoria RAM ocupada por el cañón al cerrar.
 */
PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

/**
 * @brief Se ejecuta automáticamente en cada evento para "apretar el gatillo".
 */
void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Ordena al cañón crear el vértice primario y lanzar la partícula en el evento actual.
    fParticleGun->GeneratePrimaryVertex(anEvent);
}