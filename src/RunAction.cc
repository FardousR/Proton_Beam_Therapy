


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


RunAction::RunAction(G4String output) : G4UserRunAction(), OutputFile(output)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);

    G4double x_num   = 200,   x_min   = -50, x_max   = 50;
    G4double y_num   = 200,   y_min   = -50, y_max   = 50;
    G4double z_num   = 800,   z_min   = 0.0, z_max   = 200;
    G4double r_num   = 400,   r_min   = 0.0, r_max   = 200;
    G4double KE_num  = 160,   KE_min  = 0.0, KE_max  = 160000000;
    G4double LET_num = 200,   LET_min = 0.0, LET_max = 10.0;

    //AllEnergyHisoID       =  analysisManager->CreateH3("AllEnergyHisto", "Energy Deposition by Particles",    400, -200.0, 200.0, 400, -200.0, 200.0, 200, 0., 200.0);
    ProtonEnergyHisoID      =  analysisManager->CreateH3("ProtonEnergyHisto", "Energy Deposition(Proton)",
                               x_num, x_min, x_max, y_num, y_min, y_max, z_num, z_min, z_max);
    //
    // ProtonKEHisoID          =  analysisManager->CreateH2("ProtonKEHisto", "Kinetic Energy(Proton)",
    //                            z_num, z_min, z_max, KE_num, KE_min, KE_max);
    // //NeutronEnergyHisoID   =  analysisManager->CreateH3("NeutronEnergyHisto",  "Energy Deposition by Neutrons",     400, -200.0, 200.0, 400, -200.0, 200.0, 200, 0., 200.0);
    // //ElectronEnergyHisoID  =  analysisManager->CreateH3("ElectronEnergyHisto", "Energy Deposition by Electron",     400, -200.0, 200.0, 400, -200.0, 200.0, 200, 0., 200.0);
    // ProtonLETdataHisoID     =  analysisManager->CreateH3("ProtonLETdataHisto", "LET data(Protons)",
    //                            x_num, x_min, x_max, y_num, y_min, y_max, z_num, z_min, z_max);
    //
    // ParticlecountZ0HistID   =  analysisManager->CreateH1("ParticelCounter","Number of Particles at Water Surface", 5, 0, 5);
    //
    // ProtonFluenceHistoID    =  analysisManager->CreateH1("ProtonFluence" , "Number of Proton with depth" , z_num, z_min, z_max);

    // NeutronFluenceHistoID   =  analysisManager->CreateH1("NeutronFluence", "Number of Neutron with depth", z_num, z_min, z_max);
    //
    // LETDisHistoID           =  analysisManager->CreateH2("LETHisto", "Proton LET Histo",  z_num, z_min, z_max, 150, 0.0, 15.0 );

}

RunAction::~RunAction()
{

}

void RunAction::BeginOfRunAction(const G4Run* run)
{

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    analysisManager->OpenFile(OutputFile);

}

void RunAction::EndOfRunAction(const G4Run* run)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
