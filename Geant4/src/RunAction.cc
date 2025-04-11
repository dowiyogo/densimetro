#include "globals.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"

#include <iostream>

RunAction::RunAction()
  : G4UserRunAction()
{
  
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
  triggerEvent = sensorEvent = 0;

  G4cout << "Run " << run->GetRunID() << " start." << G4endl;
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

  analysisManager->OpenFile("outHistos.root");
  analysisManager->SetFirstHistoId(1);
  
  analysisManager->CreateH1("trigger","Particles in Trigger per event",100,0.,100.);
  analysisManager->CreateH1("sensor","Particles in Sensor per event",100,0.,100.);
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4cout << "Run " << run->GetRunID() << " done." << G4endl;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4RunManager* runmanager = G4RunManager::GetRunManager();
  
  analysisManager->Write();
  analysisManager->CloseFile();

  DetectorConstruction* detector = (DetectorConstruction*) runmanager->GetUserDetectorConstruction();
  G4double distance = detector->GetXDist();
  
  G4cout << "Distance used for present run:\t" << distance << " [mm]" << G4endl;
  G4cout << "Number of Triggered Events:\t" << triggerEvent << G4endl;
  G4cout << "Number of Sensed Events:\t" << sensorEvent << G4endl;
  if (triggerEvent)
    {
      G4cout << "Probability: \t\t\tP(S|T)=" <<
	((double)sensorEvent)/((double)triggerEvent) << G4endl;
    }
}
