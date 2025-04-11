#include "globals.hh"

#include "G4ProcessManager.hh"
#include "G4IonConstructor.hh"
#include "G4ProcessManager.hh"
#include "G4RadioactiveDecay.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "PhysicsList.hh"
#include "G4PhysicsListHelper.hh"
#include "G4DecayPhysics.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4GammaConversionToMuons.hh"
#include "G4VModularPhysicsList.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4Scintillation.hh"


PhysicsList::PhysicsList()
  :G4VModularPhysicsList()
{
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  RegisterPhysics(new G4EmStandardPhysics());

}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::ConstructParticle()
{

  // gamma
  G4Gamma::GammaDefinition();

  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();


  G4OpticalPhoton::OpticalPhotonDefinition();

  // ions
  //  G4IonConstructor iConstructor;
  //  iConstructor.ConstructParticle();


}

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructOp();
  ConstructScintillation();

  //   G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  //   radioactiveDecay->SetHLThreshold(-1.*s);
  //   radioactiveDecay->SetICM(true);               //Internal Conversion
  //   radioactiveDecay->SetARM(false);              //Atomic Rearangement

  //   G4ProcessManager* pmanager = G4GenericIon::GenericIon()->GetProcessManager();  
  //   pmanager->AddProcess(radioactiveDecay, 0, -1, 1);

}


void PhysicsList::ConstructEM()
{

  G4PhysicsListHelper* helper = G4PhysicsListHelper::GetPhysicsListHelper();
  G4ParticleDefinition* particle;

  //Gamma 
  particle = G4Gamma::Gamma();
  helper->RegisterProcess(new G4GammaConversion(), particle);
  helper->RegisterProcess(new G4ComptonScattering(), particle);
  helper->RegisterProcess(new G4PhotoElectricEffect(), particle);

  //Electron
  particle = G4Electron::Electron();
  helper->RegisterProcess(new G4eIonisation(), particle);
  helper->RegisterProcess(new G4eBremsstrahlung(), particle);
  helper->RegisterProcess(new G4eMultipleScattering(), particle);

  //Positron
  particle = G4Positron::Positron();
  helper->RegisterProcess(new G4eIonisation(), particle);
  helper->RegisterProcess(new G4eBremsstrahlung(), particle);
  helper->RegisterProcess(new G4eMultipleScattering(), particle);
  helper->RegisterProcess(new G4eplusAnnihilation(), particle);

}

void PhysicsList::ConstructOp()
{

  G4PhysicsListHelper* helper = G4PhysicsListHelper::GetPhysicsListHelper();
  G4ParticleDefinition* OpPhoton;

  OpPhoton = G4OpticalPhoton::OpticalPhoton();
  helper->RegisterProcess(new G4OpBoundaryProcess(), OpPhoton);
  helper->RegisterProcess(new G4OpAbsorption(), OpPhoton);
  helper->RegisterProcess(new G4OpRayleigh(), OpPhoton);

}

void PhysicsList::ConstructScintillation()
{

  G4Scintillation* Scint = new G4Scintillation("Scintillation");
  Scint->SetTrackSecondariesFirst(true);

  G4PhysicsListHelper* helper = G4PhysicsListHelper::GetPhysicsListHelper();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if(Scint->IsApplicable(*particle)) helper->RegisterProcess(Scint, particle);
  }

}


void PhysicsList::SetCuts()
{
  G4int temp = GetVerboseLevel();
  SetVerboseLevel(0);                                                           
  SetCutsWithDefault();   
  SetVerboseLevel(temp);  
}
