#ifndef SCREENLOCKBUSINESSLOGIC_STUB
#define SCREENLOCKBUSINESSLOGIC_STUB

#include "screenlockbusinesslogic.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ScreenLockBusinessLogicStub : public StubBase {
  public:
  virtual void ScreenLockBusinessLogicConstructor(QObject *parent);
  virtual void ScreenLockBusinessLogicDestructor();
  virtual void toggleScreenLockUI(bool toggle);
  virtual void toggleEventEater(bool toggle);
  virtual void unlockScreen();
  virtual void hideEventEater();
  virtual void displayStateChanged(MeeGo::QmDisplayState::DisplayState state);
  virtual void locksChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
  virtual bool displayIsOn();
  virtual void reset();
  virtual void registerExtension(MApplicationExtensionInterface *interface);
  virtual void unregisterExtension(MApplicationExtensionInterface *interface);
};

// 2. IMPLEMENT STUB
void ScreenLockBusinessLogicStub::ScreenLockBusinessLogicConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void ScreenLockBusinessLogicStub::ScreenLockBusinessLogicDestructor() {

}
void ScreenLockBusinessLogicStub::toggleScreenLockUI(bool toggle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(toggle));
  stubMethodEntered("toggleScreenLockUI",params);
}

void ScreenLockBusinessLogicStub::toggleEventEater(bool toggle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(toggle));
  stubMethodEntered("toggleEventEater",params);
}

void ScreenLockBusinessLogicStub::unlockScreen() {
  stubMethodEntered("unlockScreen");
}

void ScreenLockBusinessLogicStub::hideEventEater() {
  stubMethodEntered("hideEventEater");
}

void ScreenLockBusinessLogicStub::displayStateChanged(MeeGo::QmDisplayState::DisplayState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmDisplayState::DisplayState >(state));
  stubMethodEntered("displayStateChanged",params);
}

void ScreenLockBusinessLogicStub::locksChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmLocks::Lock >(what));
  params.append( new Parameter<MeeGo::QmLocks::State >(how));
  stubMethodEntered("locksChanged",params);
}

bool ScreenLockBusinessLogicStub::displayIsOn() {
  stubMethodEntered("displayIsOn");
  return stubReturnValue<bool>("displayIsOn");
}

void ScreenLockBusinessLogicStub::reset() {
  stubMethodEntered("reset");
}

void ScreenLockBusinessLogicStub::registerExtension(MApplicationExtensionInterface *interface)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<MApplicationExtensionInterface * >(interface));
    stubMethodEntered("registerExtension", params);
}

void ScreenLockBusinessLogicStub::unregisterExtension(MApplicationExtensionInterface *interface)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<MApplicationExtensionInterface * >(interface));
    stubMethodEntered("unregisterExtension", params);
}



// 3. CREATE A STUB INSTANCE
ScreenLockBusinessLogicStub gDefaultScreenLockBusinessLogicStub;
ScreenLockBusinessLogicStub* gScreenLockBusinessLogicStub = &gDefaultScreenLockBusinessLogicStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ScreenLockBusinessLogic::ScreenLockBusinessLogic(QObject *parent) {
  gScreenLockBusinessLogicStub->ScreenLockBusinessLogicConstructor(parent);
}

ScreenLockBusinessLogic::~ScreenLockBusinessLogic() {
  gScreenLockBusinessLogicStub->ScreenLockBusinessLogicDestructor();
}

void ScreenLockBusinessLogic::toggleScreenLockUI(bool toggle) {
  gScreenLockBusinessLogicStub->toggleScreenLockUI(toggle);
}

void ScreenLockBusinessLogic::toggleEventEater(bool toggle) {
  gScreenLockBusinessLogicStub->toggleEventEater(toggle);
}

void ScreenLockBusinessLogic::unlockScreen() {
  gScreenLockBusinessLogicStub->unlockScreen();
}

void ScreenLockBusinessLogic::hideEventEater() {
  gScreenLockBusinessLogicStub->hideEventEater();
}

void ScreenLockBusinessLogic::displayStateChanged(MeeGo::QmDisplayState::DisplayState state) {
  gScreenLockBusinessLogicStub->displayStateChanged(state);
}

void ScreenLockBusinessLogic::locksChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how) {
  gScreenLockBusinessLogicStub->locksChanged(what, how);
}

bool ScreenLockBusinessLogic::displayIsOn() {
  return gScreenLockBusinessLogicStub->displayIsOn();
}

void ScreenLockBusinessLogic::reset() {
  gScreenLockBusinessLogicStub->reset();
}

void ScreenLockBusinessLogic::registerExtension(MApplicationExtensionInterface *interface)
{
    gScreenLockBusinessLogicStub->registerExtension(interface);
}

void ScreenLockBusinessLogic::unregisterExtension(MApplicationExtensionInterface *interface)
{
    gScreenLockBusinessLogicStub->unregisterExtension(interface);
}


#endif
