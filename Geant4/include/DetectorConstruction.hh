#ifndef _DETECTORCONSTRUCTION_H_
#define _DETECTORCONSTRUCTION_H_

#include "G4VUserDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
private:

  G4VPhysicalVolume* PWorld;
  G4VPhysicalVolume* Trigger;
  G4VPhysicalVolume* Sensor;
  G4VPhysicalVolume* Source;
  G4double XDist;
  
public:
 
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume *Construct();
  const G4VPhysicalVolume* GetWorld()		{return PWorld;};           
  const G4VPhysicalVolume* GetTrigger()	{return Trigger;};
  const G4VPhysicalVolume* GetSensor()	{return Sensor;};
  G4VPhysicalVolume* GetSource()	{return Source;};
  G4double GetXDist() {return XDist;};
};
#endif
