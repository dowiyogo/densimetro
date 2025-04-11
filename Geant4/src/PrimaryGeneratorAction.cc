#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"

#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4Tubs.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
	: G4VUserPrimaryGeneratorAction(),
	  particleSource(NULL),
	  particleGun(NULL)
{
	G4int particles = 1;
	particleGun = new G4ParticleGun(particles);
	particleGun->SetParticleDefinition(G4Gamma::Gamma());
	particleGun->SetParticleEnergy(511*keV);


//	particleSource = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete particleGun;
//	delete particleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* Event)
{
  G4double cosAlpha = 1. - G4UniformRand()*(1.- std::cos(15*deg));
  G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
  G4double psi      = twopi*G4UniformRand();  //psi uniform in [0, 2*pi]  
  G4ThreeVector direction(sinAlpha*std::cos(psi),sinAlpha*std::sin(psi),cosAlpha);
  /*============================================================================*/
  /* Las siguientes línead de código buscan obtener un puntero al
     volumen de 'Source' con su tamaño y ubicación, para así
     generar aleatoriamente los gammas dentro de dicho volumen
     (dos gammas con dirección opuesta).
  */
  // G4RunManager* runmanager = G4RunManager::GetRunManager();
  // DetectorConstruction* detector = (DetectorConstruction*) runmanager->GetUserDetectorConstruction();

  // G4VPhysicalVolume* Source = detector->GetSource();

  G4ThreeVector srcPosition(0.*cm,0.*cm,-1.5*mm);
  
  // G4Tubs* srcTubs = (G4Tubs*) Source->GetLogicalVolume()->GetSolid();

  G4double x,y,z;
  G4double r = 1.5*mm;
  G4double srcHLength = 1.5*mm;
  /*============================================================================*/
  
  do{
    x = r*(2*G4UniformRand()-1) + srcPosition.x();
    y = r*(2*G4UniformRand()-1) + srcPosition.y();	
  } while (x*x+y*y>r*r);

  do{
    //z = -1.5*mm + 1.5*(2.0*G4UniformRand()-1.0)*mm;
    z = srcHLength*(2*G4UniformRand()-1) + srcPosition.z();
  } while (!(
	   (z <= (srcPosition.z()+srcHLength)) &&
	   (z >= (srcPosition.z()-srcHLength))
	     ));
  
  G4ThreeVector position(x,y,z);

  particleGun->SetParticlePosition(position);
  particleGun->SetParticleMomentumDirection(direction);
  particleGun->GeneratePrimaryVertex(Event);

  particleGun->SetParticlePosition(position);
  direction *= -1.;
  particleGun->SetParticleMomentumDirection(direction);
  particleGun->GeneratePrimaryVertex(Event);

}
