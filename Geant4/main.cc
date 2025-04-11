#include <vector>

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


int main(int argc,char **argv)
{

   
   if (argc>3)
   {
      G4cout << "Error! Too many arguments!" << G4endl;
      G4cout << G4endl;
      return -1;
   }

   ofstream file;
   file.open("output.txt");
   file.close();
   
   G4RunManager* runManager = new G4RunManager;

   runManager->SetUserInitialization(new DetectorConstruction());
   runManager->SetUserInitialization(new PhysicsList());
   runManager->SetUserAction(new PrimaryGeneratorAction());
   runManager->SetUserAction(new EventAction());
   runManager->SetUserAction(new SteppingAction());
   runManager->SetUserAction(new RunAction());
   

   runManager->Initialize();
   G4UImanager* UImanager = G4UImanager::GetUIpointer();

   if (argc==2)   // batch mode  
   {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UImanager->ApplyCommand(command+fileName);
   }
   else           // interactive mode
   {
#ifdef G4UI_USE
     G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
     G4VisManager* visManager = new G4VisExecutive;
     visManager->Initialize();
     UImanager->ApplyCommand("/control/execute vis.mac");
#endif
     ui->SessionStart();
#ifdef G4VIS_USE
     delete visManager;
#endif
     delete ui;
#endif
   }

   delete runManager;

   return 0;

}


