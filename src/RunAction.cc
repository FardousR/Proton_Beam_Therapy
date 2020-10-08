


#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RunAction::RunAction() : G4UserRunAction()
{

    if(remove("/home/fardous/Desktop/Proton_Beam_Therapy/Data/ProtonBeamTherapyDataFile.csv") != 0 )
        perror( "Error deleting file: ProtonBeamTherapyDataFile.csv");
    else
        puts("File:ProtonBeamTherapyDataFile.csv is successfully deleted");

    if(remove("/home/fardous/Desktop/Proton_Beam_Therapy/Data/ProtonPhaseSpaceData.csv") != 0 )
        perror( "Error deleting file: ProtonPhaseSpaceData.csv");
    else
        puts("File:ProtonPhaseSpaceData.csv is successfully deleted");

    if(remove("/home/fardous/Desktop/Proton_Beam_Therapy/Data/InitialBeam.csv") != 0 )
        perror( "Error deleting file: Boundery_data.csv");
    else
        puts("File:InitialBeam.csv is successfully deleted");

    if(remove("/home/fardous/Desktop/Proton_Beam_Therapy/Data/TargetBoundaryData.csv") != 0 )
        perror( "Error deleting file: TargetBoundaryData.csv");
    else
        puts("File:TargetBoundaryData.csv is successfully deleted");


}

RunAction::~RunAction()
{

}

void RunAction::BeginOfRunAction(const G4Run* run)
{

}

void RunAction::EndOfRunAction(const G4Run* run)
{

}
