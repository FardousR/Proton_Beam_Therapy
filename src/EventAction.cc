
#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
//#include "Analysis.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>


EventAction::EventAction():G4UserEventAction()
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

	using std::to_string;
	using std::ofstream;
	using std::fstream;

	fstream dataFile;

	G4int EvNum=Event->GetEventID();
	G4double Ek=Event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
	//std::stringstream StreamName;
    //StreamName << "Events/";
	//StreamName <<"250MeV.csv";

    G4String FileName = "/home/fardous/Desktop/Proton_Beam_Therapy/Data/ProtonBeamTherapyDataFile.csv";//StreamName.str();
	dataFile.open(FileName, std::ios::out| std::ios::app);

	for (G4int i=0;i<ParticleID.size();++i)
	{
		G4ThreeVector preCoordinate = GetprePosition(i);
		G4double pre_x = (preCoordinate.getX())/1000;
		G4double pre_y = (preCoordinate.getY())/1000;
		G4double pre_z = preCoordinate.getZ();

    	G4ThreeVector postCoordinate = GetpostPosition(i);
		G4double post_x = postCoordinate.getX()/1000;
		G4double post_y = postCoordinate.getY()/1000;
		G4double post_z = postCoordinate.getZ();

		G4ThreeVector preMomentumDirection = GetpreMomentumDir(i);
		G4double pre_x_angle = atan(preMomentumDirection.getX()/preMomentumDirection.getZ());
		G4double pre_y_angle = atan(preMomentumDirection.getY()/preMomentumDirection.getZ());
		G4double pre_z_direction = preMomentumDirection.getZ();

		G4ThreeVector postMomentumDirection = GetpostMomentumDir(i);
		G4double post_x_angle = atan(postMomentumDirection.getX()/postMomentumDirection.getZ());
		G4double post_y_angle = atan(postMomentumDirection.getY()/postMomentumDirection.getZ());
        G4double post_z_direction = postMomentumDirection.getZ();

		dataFile<<EventID[i]<<", ";
		dataFile<<TrackID[i]<<", ";
        dataFile<<ParticleID[i]<<", ";
		dataFile<<ParentID[i]<<", ";

		dataFile<<pre_x<<", ";
		dataFile<<pre_y<<", ";
		dataFile<<pre_z<<", ";
		dataFile<<post_x<<", ";
		dataFile<<post_y<<", ";
		dataFile<<post_z<<", ";

		dataFile<<pre_x_angle<<", ";
		dataFile<<pre_y_angle<<", ";
		dataFile<<pre_z_direction<<",";

		dataFile<<post_x_angle<<", ";
		dataFile<<post_y_angle<<", ";
		dataFile<<post_z_direction<<", ";

		dataFile<<EnergyDeposit[i]<<", ";
		dataFile<<SecondaryEnergy[i]<<", ";
		dataFile<<PreKE[i]<<", ";
		dataFile<<PostKE[i];
        dataFile<<G4endl;


		//G4cout<<"*********Writing*********";
		/*
		//G4ThreeVector OutPosition=GetPosition(i);
		//G4double x=OutPosition.x()/um;
		//G4double y=OutPosition.y()/um;
		//G4double z=OutPosition.z()/um;

		OS.write(reinterpret_cast<char*>(&ParticleID[i]),sizeof(G4int));
		OS.write(reinterpret_cast<char*>(&ProcessID[i]),sizeof(G4int));
		//OS.write(reinterpret_cast<char*>(&x),sizeof(G4double));
		//OS.write(reinterpret_cast<char*>(&y),sizeof(G4double));
		//OS.write(reinterpret_cast<char*>(&z),sizeof(G4double));
		//OS.write(reinterpret_cast<char*>(&PreEnergy[i]),sizeof(G4double));
		OS.write(reinterpret_cast<char*>(&PostEnergy[i]),sizeof(G4double));
		OS.write(reinterpret_cast<char*>(&SecEnergy[i]),sizeof(G4double));
		OS.write(reinterpret_cast<char*>(&EnergyDeposit[i]),sizeof(G4double));
		OS.write(reinterpret_cast<char*>(&TrackID[i]),sizeof(G4int));
		OS.write(reinterpret_cast<char*>(&EventID[i]),sizeof(G4int));

		//OS.write(GetPosition(i).x(),sizeof(G4double));
		//OS.write(GetPosition(i).y(),sizeof(G4double));
		*/

	}
dataFile.close();
ClearTrackData();
}
