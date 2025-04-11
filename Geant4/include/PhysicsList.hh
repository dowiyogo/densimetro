#ifndef _PHYSICSLIST_H_
#define _PHYSICSLIST_H_

#include "G4VUserPhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList();
   ~PhysicsList();

  protected:

    void ConstructParticle();
    void ConstructProcess();
    void ConstructEM();
    void ConstructOp();
    void ConstructScintillation();
    void SetCuts();
};

#endif
