
#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include <iostream>
#include <ostream>

class G4Event;

class EventAction: public G4UserEventAction
     {
      private:   //Trackdata vectors
  	     std::vector<G4int> EventID;
  	     std::vector<G4int> TrackID;
         std::vector<G4int> ParticleID;
         std::vector<G4int> ParentID;
  	     std::vector<G4ThreeVector> PrePosition;
         std::vector<G4ThreeVector> PostPosition;
         std::vector<G4ThreeVector> PreMomentumDir;
         std::vector<G4ThreeVector> PostMomentumDir;
         std::vector<G4double> EnergyDeposit;
         std::vector<G4double> SecondaryEnergy;
         std::vector<G4double> PreKE;
         std::vector<G4double> PostKE;

      public:
           EventAction();
           ~EventAction();
           virtual void BeginOfEventAction(const G4Event*);
           virtual void EndOfEventAction(const G4Event*);

	  void ClearTrackData()
      {
          EventID.clear();
          TrackID.clear();
          ParticleID.clear();
          ParentID.clear();
          PrePosition.clear();
          PostPosition.clear();
          PreMomentumDir.clear();
          PostMomentumDir.clear();
          SecondaryEnergy.clear();
          EnergyDeposit.clear();
          PreKE.clear();
          PostKE.clear();
      }

	  //Add methods for TrackData vectors
      void AddEventID(G4int i) {EventID.push_back(i);}
      void AddTrackID(G4int i) {TrackID.push_back(i);}
      void AddParticleID(G4int i) {ParticleID.push_back(i);}
      void AddParentID(G4int i) {ParentID.push_back(i);}
      void AddprePosition(G4ThreeVector PrePos) {PrePosition.push_back(PrePos);}
      void AddpostPosition(G4ThreeVector PostPos) {PostPosition.push_back(PostPos);}
      void AddpreMomentumDir(G4ThreeVector PreMom) {PreMomentumDir.push_back(PreMom);}
      void AddpostMomentumDir(G4ThreeVector PostMom) {PostMomentumDir.push_back(PostMom);}
      void AddEnergyDeposit(G4double E) {EnergyDeposit.push_back(E);}
      void AddSecondaryEnergy(G4double SE) {SecondaryEnergy.push_back(SE);}
      void AddpreKE(G4double preKE){PreKE.push_back(preKE);}
      void AddpostKE(G4double postKE){PostKE.push_back(postKE);}


	  //Get method for Post step position
	  G4int GetEventID(G4int i) { return EventID[i];}
      G4int GetTrackID(G4int i) {return TrackID[i];}
      G4int GetParticleID(G4int i) {return ParticleID[i];}
      G4int GetParentID(G4int i) {return ParentID[i];}
      G4ThreeVector GetprePosition(G4int i) {return PrePosition[i];}
      G4ThreeVector GetpostPosition(G4int i) {return PostPosition[i];}
      G4ThreeVector GetpreMomentumDir(G4int i) {return PreMomentumDir[i];}
      G4ThreeVector GetpostMomentumDir(G4int i) {return PostMomentumDir[i];};
      G4double GetEnergyDeposit(G4int i) {return EnergyDeposit[i];}
      G4double GetSecEnergy(G4int i) {return SecondaryEnergy[i];}
      G4double GetpreKE(G4int i)  {return PreKE[i];}
      G4double GetpostKE(G4int i)  {return PostKE[i];}
     };

#endif
