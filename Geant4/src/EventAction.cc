#include "EventAction.hh"
#include "G4Event.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SDManager.hh"
#include "G4StepPoint.hh"
#include "G4VHitsCollection.hh"
#include "G4THitsCollection.hh"

#include "Analysis.hh"

EventAction::EventAction()
{
	outfile.open("output.txt", ios::app);
}

EventAction::~EventAction()
{
	outfile.close();
}

void EventAction::BeginOfEventAction(const G4Event*)
{
	// G4cout << "Event " << event->GetEventID() << " start." << G4endl;
	this->setNOTP(0);
	this->setNOSP(0);
}

void EventAction::EndOfEventAction(const G4Event* event)
{
	if (NrOfTriggerParticles)
	{
		// G4cout << "Event " << event->GetEventID() << " done." << G4endl;
		outfile << "Event N. " << event->GetEventID() << G4endl;
		outfile << "N. of Particles that reached trigger:\t"
				<< NrOfTriggerParticles << G4endl;
		outfile << "N. of Particles that reached sensor while triggered:\t"
				<< NrOfSensorParticles << G4endl;
		outfile << "\tProbability P(S|T)=" 
				<< NrOfSensorParticles/NrOfTriggerParticles;
		outfile << G4endl << G4endl;


		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		G4RunManager* runmanager = G4RunManager::GetRunManager();
		RunAction* runaction = (RunAction*) runmanager->GetUserRunAction();
		

		runaction->AddTriggerEvent();

		if (NrOfSensorParticles)  runaction->AddSensorEvent();
		

		analysisManager->FillH1(1,NrOfTriggerParticles);
		analysisManager->FillH1(2,NrOfSensorParticles);
	}
	
}

void EventAction::setNOTP(int n)
{
	NrOfTriggerParticles = n;
}

void EventAction::setNOSP(int n)
{
	NrOfSensorParticles = n;
}

void EventAction::AddNOTP()
{
	NrOfTriggerParticles++;
}

void EventAction::AddNOSP()
{
	NrOfSensorParticles++;
}
