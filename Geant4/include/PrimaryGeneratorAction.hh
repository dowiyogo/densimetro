#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:

	PrimaryGeneratorAction();
	~PrimaryGeneratorAction();
	void GeneratePrimaries(G4Event*);

private:
	G4GeneralParticleSource* particleSource;
	G4ParticleGun* particleGun;

};

#endif
