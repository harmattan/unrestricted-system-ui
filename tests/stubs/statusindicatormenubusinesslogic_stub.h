#ifndef STATUSINDICATORMENUBUSINESSLOGIC_STUB
#define STATUSINDICATORMENUBUSINESSLOGIC_STUB

#include "statusindicatormenubusinesslogic.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorMenuBusinessLogicStub : public StubBase {
  public:
  virtual void StatusIndicatorMenuBusinessLogicConstructor(QObject *parent);
  virtual void StatusIndicatorMenuBusinessLogicDestructor();
  virtual void showStatusIndicatorMenu();
  virtual bool isStatusIndicatorMenuVisible() const;
}; 

// 2. IMPLEMENT STUB
void StatusIndicatorMenuBusinessLogicStub::StatusIndicatorMenuBusinessLogicConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void StatusIndicatorMenuBusinessLogicStub::StatusIndicatorMenuBusinessLogicDestructor() {

}
void StatusIndicatorMenuBusinessLogicStub::showStatusIndicatorMenu() {
  stubMethodEntered("showStatusIndicatorMenu");
}

bool StatusIndicatorMenuBusinessLogicStub::isStatusIndicatorMenuVisible() const {
  stubMethodEntered("isStatusIndicatorMenuVisible");
  return stubReturnValue<bool>("isStatusIndicatorMenuVisible");
}



// 3. CREATE A STUB INSTANCE
StatusIndicatorMenuBusinessLogicStub gDefaultStatusIndicatorMenuBusinessLogicStub;
StatusIndicatorMenuBusinessLogicStub* gStatusIndicatorMenuBusinessLogicStub = &gDefaultStatusIndicatorMenuBusinessLogicStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorMenuBusinessLogic::StatusIndicatorMenuBusinessLogic(QObject *parent) {
  gStatusIndicatorMenuBusinessLogicStub->StatusIndicatorMenuBusinessLogicConstructor(parent);
}

StatusIndicatorMenuBusinessLogic::~StatusIndicatorMenuBusinessLogic() {
  gStatusIndicatorMenuBusinessLogicStub->StatusIndicatorMenuBusinessLogicDestructor();
}

void StatusIndicatorMenuBusinessLogic::showStatusIndicatorMenu() {
  gStatusIndicatorMenuBusinessLogicStub->showStatusIndicatorMenu();
}

bool StatusIndicatorMenuBusinessLogic::isStatusIndicatorMenuVisible() const {
  return gStatusIndicatorMenuBusinessLogicStub->isStatusIndicatorMenuVisible();
}


#endif
