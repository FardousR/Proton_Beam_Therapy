
#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "Analysis.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction(G4String output);
    virtual ~RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    G4int GetAllEnergyHisoID()            {  return AllEnergyHisoID;        }
    G4int GetProtonEnergyHisoID()         {  return ProtonEnergyHisoID;     }
    G4int GetNeutronEnergyHisoID()        {  return NeutronEnergyHisoID;    }
    G4int GetElectronEnergyHisoID()       {  return ElectronEnergyHisoID;   }
    G4int GetProtonLETdataHisoID()        {  return ProtonLETdataHisoID;    }
    G4int GetParticlecountZ0HistID()      {  return ParticlecountZ0HistID;  }
    G4int GetLETDisHistoID()              {  return LETDisHistoID;          }
    G4int GetProtonFluenceHistoID()       {  return ProtonFluenceHistoID;   }
    G4int GetNeutronFluenceHistoID()      {  return NeutronFluenceHistoID;  }
    G4int GetProtonKEHisoID()             {  return ProtonKEHisoID;         }

  private:
    G4int AllEnergyHisoID;
    G4int ProtonEnergyHisoID;
    G4int NeutronEnergyHisoID;
    G4int ElectronEnergyHisoID;
    G4int ProtonLETdataHisoID;
    G4int ParticlecountZ0HistID;
    G4int LETDisHistoID;
    G4int ProtonFluenceHistoID;
    G4int NeutronFluenceHistoID;
    G4int ProtonKEHisoID;
    
    G4String OutputFile;
};

#endif
