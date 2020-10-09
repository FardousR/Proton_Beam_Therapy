
#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include "Analysis.hh"
#include <math.h>

EventAction::EventAction(RunAction* runAction):G4UserEventAction(), fRunAction(runAction)
{
}

EventAction::~EventAction()
{

}

void EventAction::BeginOfEventAction(const G4Event*)
{
	ClearTrackData(); //Clear data from TrackData vectors
}

void EventAction::EndOfEventAction(const G4Event* Event)
{
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	for (G4int i=0;i<ParticleID.size();++i)
	{
		G4double DoseLET =0, CalLET = 0;
		G4double pre_x = PrePosition[i].getX()*mm;
		G4double pre_y = PrePosition[i].getY()*mm;
		G4double pre_z = PrePosition[i].getZ()*mm;

		G4double post_x = PostPosition[i].getX()*mm;
		G4double post_y = PostPosition[i].getY()*mm;
		G4double post_z = PostPosition[i].getZ()*mm;


		G4double pre_x_angle = atan(PreMomentumDir[i].getX()/PreMomentumDir[i].getZ());
		G4double pre_y_angle = atan(PreMomentumDir[i].getY()/PreMomentumDir[i].getZ());
		G4double pre_z_direction = PreMomentumDir[i].getZ();

		G4double post_x_angle = atan(PostMomentumDir[i].getX()/PostMomentumDir[i].getZ());
		G4double post_y_angle = atan(PostMomentumDir[i].getY()/PostMomentumDir[i].getZ());
        G4double post_z_direction = PostMomentumDir[i].getZ();

        G4double energy_dipo_x = pre_x + (post_x - pre_x) * ( (G4double)rand() / (G4double)RAND_MAX );
        G4double energy_dipo_y = pre_y + (post_y - pre_y) * ( (G4double)rand() / (G4double)RAND_MAX );
		G4double energy_dipo_z = pre_z + (post_z - pre_z) * ( (G4double)rand() / (G4double)RAND_MAX );
		G4double radial_distance = sqrt(pow(energy_dipo_x,2) + pow(energy_dipo_y,2));

		// if(pre_z == 0)
		// {
		// 	analysisManager->FillH1(fRunAction->GetParticlecountZ0HistID(), (ParticleID[i]-0.5), 1);
		// }

		if(ParticleID[i] == 1)
		{
    //   if((int)pre_z!=(int)post_z)
	// 		{
	// 		   analysisManager->FillH1(fRunAction->GetProtonFluenceHistoID(), post_z-.5, 1);
	// 	  }
        analysisManager->FillH3(fRunAction->GetProtonEnergyHisoID(), energy_dipo_x, energy_dipo_y, energy_dipo_z, EnergyDeposit[i]);
	//
	//
	// 		if(StepLength[i]==0)
	// 			{
	// 		   DoseLET = 0;
	// 			 CalLET  = 0;
	// 			}
	// 		else
	// 		  {
	// 		   DoseLET = (EnergyDeposit[i]*EnergyDeposit[i])/StepLength[i];
	// 			 CalLET  = EnergyDeposit[i]/StepLength[i];
	// 		  }
	// 		analysisManager->FillH2(fRunAction->GetLETDisHistoID(),   energy_dipo_z,    CalLET/1000000,    1);
	// 		analysisManager->FillH2(fRunAction->GetProtonKEHisoID(),  pre_z,            PreKE[i],  1);
	// 		analysisManager->FillH3(fRunAction->GetProtonLETdataHisoID(), energy_dipo_x, energy_dipo_y, energy_dipo_z, DoseLET);
    // }
	// 	else if(ParticleID[i] == 5)
	// 	{
    //   if((int)pre_z!=(int)post_z)
	// 		{
	// 		   analysisManager->FillH1(fRunAction->GetNeutronFluenceHistoID(), post_z-.5, 1);
	// 	  }
		}
	}
ClearTrackData();
}
