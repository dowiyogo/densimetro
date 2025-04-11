/* Código copiado del ejemplo novice/N03->SteppingAction */

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4ParticleTypes.hh"
#include "G4String.hh"

#include "G4VProcess.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"

#include <iostream>
using namespace std;

SteppingAction::SteppingAction()                                         
{
  detector = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  eventAction = (EventAction*) G4RunManager::GetRunManager()->GetUserEventAction();               
}

SteppingAction::~SteppingAction()
{ }

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the current step
  G4VPhysicalVolume* volume 
    = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();

  
  G4Track* track = aStep->GetTrack();
  // G4int trackid = track->GetTrackID();
  // G4int parentid = track->GetParentID();
  G4String particlename = track->GetDefinition()->GetParticleType();
  G4String volumename = volume->GetName();
  const G4VProcess* process = track->GetCreatorProcess();
  G4String creatorprocname;
  if (process) creatorprocname = process->GetProcessName();
  
  if (aStep->GetTrack()->GetDefinition()->GetParticleType() == "gamma" && edep)
    {
      if (volume == detector->GetTrigger()) eventAction->AddNOTP();
      if (volume == detector->GetSensor())  eventAction->AddNOSP();
    }
  else if (creatorprocname == "compt")
    {
      if (volume == detector->GetTrigger()) eventAction->AddNOTP();
      if (volume == detector->GetSensor())  eventAction->AddNOSP();
    }
}
