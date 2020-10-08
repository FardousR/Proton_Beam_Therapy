
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4VProcess.hh"

class DetectorConstruction;
class RunAction;
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
SteppingAction(EventAction*);
//SteppingAction(DetectorConstruction*, RunAction*, EventAction*);
virtual ~SteppingAction();
virtual void UserSteppingAction(const G4Step*);

private:
//DetectorConstruction* fDetector;
//RunAction* fRunAction;
EventAction* fEventAction;
};
#endif
