#include "DetectorConstruction.hh"

#include "globals.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4MaterialPropertyVector.hh"

#include "G4VPhysicalVolume.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//#include "G4OpticalSurface.hh"
//#include "G4LogicalBorderSurface.hh"

using namespace CLHEP;

DetectorConstruction::DetectorConstruction(){}

DetectorConstruction::~DetectorConstruction(){}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  /*** Material Definitions ***/
  G4NistManager* nist = G4NistManager::Instance(); // Manager that use G4's elements database

  G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");
	
  G4Element* Br = nist->FindOrBuildElement("Br");
  G4Element* La = nist->FindOrBuildElement("La");
  //G4Element* Ce = nist->FindOrBuildElement("Ce");
	
  G4double brillanceDensity = 5.08*g/cm3;
  G4Material* Brillance = new G4Material("Brillance", brillanceDensity, 2);
  Brillance->AddElement(Br, 3);
  Brillance->AddElement(La, 1);

 /*** Material Definitions ***/

  /*** Trigger Construction ***/
  G4LogicalVolume* volume[6]; //Colimator, trigger colimator, source, material, sensor.
  G4Tubs* solidVol[6];
  G4ThreeVector positions[6];

  G4double innerRadius = (3/2)*mm;
  G4double outerRadius = (25.7/2)*mm;
  G4double halflenght = (25/2)*mm;
  G4double phi_i = 0.*deg;
  G4double phi_f = 360.*deg;

  //G4double worldEdge = 10*cm;

  G4Tubs* SWorld = new G4Tubs("solidWorld",0.*cm,3.*cm,9.*cm,phi_i, phi_f);
  G4LogicalVolume* LWorld = new G4LogicalVolume(SWorld, Air, "LWorld");
  G4VPhysicalVolume* PWorld =
    new G4PVPlacement(NULL,G4ThreeVector(0.,0.,0.),LWorld,"World",NULL,false,0,false);

  solidVol[0] = new G4Tubs("Vol_0",0.*cm,outerRadius,halflenght,phi_i,phi_f); //trigger
  solidVol[1] = new G4Tubs("Vol_1",innerRadius,outerRadius,7.*mm,phi_i,phi_f); //first collimator
  solidVol[2] = new G4Tubs("Vol_2",0.*cm,outerRadius,1.5*mm,phi_i,phi_f); //radiation source
  //solidVol[3] = new G4Tubs("Vol_3",innerRadius,outerRadius,halflenght,phi_i,phi_f);
  //second collimator
  // solidVol[4] = new G4Tubs("Vol_4",0.*cm,outerRadius,5.*mm,phi_i,phi_f); //material
  solidVol[5] = new G4Tubs("Vol_5",0.*cm,outerRadius,halflenght,phi_i,phi_f); //sensor

  volume[0] = new G4LogicalVolume(solidVol[0],Brillance,"trigger");
  volume[1] = new G4LogicalVolume(solidVol[1],Pb,"trigger collimator");
  volume[2] = new G4LogicalVolume(solidVol[2],Air,"radiation source");
  //volume[3] = new G4LogicalVolume(solidVol[3],Pb,"radiation collimator");
  // volume[4] = new G4LogicalVolume(solidVol[4],Air,"matter");
  volume[5] = new G4LogicalVolume(solidVol[5],Brillance,"sensor");

  positions[0] = G4ThreeVector(0.*cm,0.*cm,-1*(3.*mm+14.*mm+halflenght));
  positions[1] = G4ThreeVector(0.*cm,0.*cm,-1*(3.*mm+7.*mm));
  positions[2] = G4ThreeVector(0.*cm,0.*cm,-1.5*mm);
  //positions[3] = G4ThreeVector(0.*cm,0.*cm,1*halflenght);
  // positions[4] = G4ThreeVector(0.*cm,0.*cm,5.*mm);
  /* Modificar este valor para distintas distancias  */
  this->XDist = 80.*mm;
  positions[5] = G4ThreeVector(0.*cm,0.*cm,XDist+halflenght);

  this->Trigger =
    new G4PVPlacement(NULL,positions[0],volume[0],"trigger",	LWorld,false,0,false);
  new G4PVPlacement(NULL,positions[1],volume[1],"tcollimator",LWorld,false,0,false);
  this->Source =
    new G4PVPlacement(NULL,positions[2],volume[2],"rsource",LWorld,false,0,false);
  //new G4PVPlacement(NULL,positions[3],volume[3],"rcollimator",LWorld,false,0,false);
  // new G4PVPlacement(NULL,positions[4],volume[4],"matter",LWorld,false,0,false);
  this->Sensor = 
    new G4PVPlacement(NULL,positions[5],volume[5],"sensor",LWorld,false,0,false);
  /*** Trigger Construction ***/

  //Tagger Materials properties 
  
  // const G4int Air_NUMENTRIES = 20;
  // G4double rAir[Air_NUMENTRIES]  =
  //   {1.00, 1.00, 1.00, 1.00,
  //    1.00, 1.00, 1.00, 1.00,
  //    1.00, 1.00, 1.00, 1.00,
  //    1.00, 1.00, 1.00, 1.00,
  //    1.00, 1.00, 1.00, 1.00};
  
  // G4double a_ene[Air_NUMENTRIES]   =
  //   {1.79*eV, 1.85*eV, 1.91*eV, 1.97*eV,
  //    2.04*eV, 2.11*eV, 2.19*eV, 2.27*eV,
  //    2.36*eV, 2.45*eV, 2.56*eV, 2.67*eV,
  //    2.80*eV, 2.94*eV, 3.09*eV, 3.25*eV,
  //    3.44*eV, 3.65*eV, 3.89*eV, 4.16*eV};
  
  // G4MaterialPropertiesTable* Air_MPT = new G4MaterialPropertiesTable();
  // Air_MPT->AddProperty("RINDEX", a_ene, rAir, Air_NUMENTRIES);
  // Air->SetMaterialPropertiesTable(Air_MPT);
  
  // const G4int Brillance_NUMENTRIES = 4;
  // G4double Brillance_Energy[Brillance_NUMENTRIES] = {2.9176*eV, 3.100*eV,3.3066*eV, 3.5428*eV};
  // G4double Brillance_FAST[Brillance_NUMENTRIES] = {1.5, 15.0, 29.0, 17.5};
  // G4double Brillance_RIND[Brillance_NUMENTRIES] = {1.9,1.9,1.9,1.9};
  // G4double Brillance_ABSL[Brillance_NUMENTRIES] = {35.0*cm, 35.0*cm,35.0*cm,35.0*cm};
  // G4MaterialPropertiesTable* Brillance_MPT = new G4MaterialPropertiesTable();

  // Brillance_MPT->AddProperty("FASTCOMPONENT", Brillance_Energy, Brillance_FAST,
  // 			     Brillance_NUMENTRIES);
  // Brillance_MPT->AddProperty("RINDEX", Brillance_Energy, Brillance_RIND,
  // 			     Brillance_NUMENTRIES);
  // Brillance_MPT->AddProperty("ABSLENGTH", Brillance_Energy, Brillance_ABSL,
  // 			     Brillance_NUMENTRIES);
  // Brillance_MPT->AddConstProperty("SCINTILLATIONYIELD", 63./keV);
  // Brillance_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
  // Brillance_MPT->AddConstProperty("FASTTIMECONSTANT",16*ns);
  // Brillance->SetMaterialPropertiesTable(Brillance_MPT);
  
  /*** Visual Atributtes ***/
  LWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

  volume[0]->SetVisAttributes(new G4VisAttributes(G4Colour::Red()));
  volume[1]->SetVisAttributes(new G4VisAttributes(G4Colour::Cyan()));
  volume[2]->SetVisAttributes(new G4VisAttributes(G4Colour::Green()));
  //volume[3]->SetVisAttributes(new G4VisAttributes(G4Colour::Cyan()));
  // volume[4]->SetVisAttributes(new G4VisAttributes(G4Colour::Yellow()));
  volume[5]->SetVisAttributes(new G4VisAttributes(G4Colour::Red()));
  /*** Visual Atributtes ***/

  return PWorld;
}
