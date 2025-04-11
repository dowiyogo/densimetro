#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include <iostream>
#include <fstream>

using namespace std;

class EventAction : public G4UserEventAction {
private:
	int NrOfTriggerParticles;
	int NrOfSensorParticles;
	ofstream outfile;
public:
    EventAction();
    ~EventAction();

    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    void setNOTP(int);
    void setNOSP(int);
    void AddNOTP();
    void AddNOSP();
    int getNOTP() {return NrOfTriggerParticles;}
    int getNOSP() {return NrOfSensorParticles;}

};

#endif
