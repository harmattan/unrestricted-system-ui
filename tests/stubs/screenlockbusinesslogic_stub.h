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
  virtual int tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker);
  virtual int tklock_close(bool silent);
  virtual void toggleScreenLockUI(bool toggle);
  virtual void toggleEventEater(bool toggle);
  virtual void registerExtension(MApplicationExtensionInterface *extension);
  virtual void unregisterExtension(MApplicationExtensionInterface *extension);
  virtual void reset();
  virtual void unlockScreen();
  virtual void showScreenLock();
  virtual void showLowPowerMode();
  virtual void setDisplayOffMode();
  virtual void hideScreenLockAndEventEater();
  virtual void showEventEater();
  virtual void hideEventEater();
  virtual bool isScreenLocked();
#ifdef HAVE_QMSYSTEM
  virtual void displayStateChanged(MeeGo::QmDisplayState::DisplayState state);
  virtual void systemStateChanged(MeeGo::QmSystemState::StateIndication what);
#endif
}; 

// 2. IMPLEMENT STUB
void ScreenLockBusinessLogicStub::ScreenLockBusinessLogicConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void ScreenLockBusinessLogicStub::ScreenLockBusinessLogicDestructor() {

}
int ScreenLockBusinessLogicStub::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(service));
  params.append( new Parameter<const QString & >(path));
  params.append( new Parameter<const QString & >(interface));
  params.append( new Parameter<const QString & >(method));
  params.append( new Parameter<uint >(mode));
  params.append( new Parameter<bool >(silent));
  params.append( new Parameter<bool >(flicker));
  stubMethodEntered("tklock_open",params);
  return stubReturnValue<int>("tklock_open");
}

int ScreenLockBusinessLogicStub::tklock_close(bool silent) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(silent));
  stubMethodEntered("tklock_close",params);
  return stubReturnValue<int>("tklock_close");
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

void ScreenLockBusinessLogicStub::registerExtension(MApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MApplicationExtensionInterface * >(extension));
  stubMethodEntered("registerExtension",params);
}

void ScreenLockBusinessLogicStub::unregisterExtension(MApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MApplicationExtensionInterface * >(extension));
  stubMethodEntered("unregisterExtension",params);
}

void ScreenLockBusinessLogicStub::reset() {
  stubMethodEntered("reset");
}

void ScreenLockBusinessLogicStub::unlockScreen() {
  stubMethodEntered("unlockScreen");
}

void ScreenLockBusinessLogicStub::showScreenLock() {
  stubMethodEntered("showScreenLock");
}

void ScreenLockBusinessLogicStub::showLowPowerMode() {
  stubMethodEntered("showLowPowerMode");
}

void ScreenLockBusinessLogicStub::setDisplayOffMode() {
  stubMethodEntered("setDisplayOffMode");
}

void ScreenLockBusinessLogicStub::hideScreenLockAndEventEater() {
  stubMethodEntered("hideScreenLockAndEventEater");
}

void ScreenLockBusinessLogicStub::showEventEater() {
  stubMethodEntered("showEventEater");
}

void ScreenLockBusinessLogicStub::hideEventEater() {
  stubMethodEntered("hideEventEater");
}

bool ScreenLockBusinessLogicStub::isScreenLocked() {
  stubMethodEntered("isScreenLocked");
  return stubReturnValue<bool>("isScreenLocked");
}

#ifdef HAVE_QMSYSTEM
void ScreenLockBusinessLogicStub::displayStateChanged(MeeGo::QmDisplayState::DisplayState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmDisplayState::DisplayState >(state));
  stubMethodEntered("displayStateChanged",params);
}

void ScreenLockBusinessLogicStub::systemStateChanged(MeeGo::QmSystemState::StateIndication what) {
    QList<ParameterBase*> params;
    params.append( new Parameter<MeeGo::QmSystemState::StateIndication >(what));
    stubMethodEntered("systemStateChanged",params);
}
#endif



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

int ScreenLockBusinessLogic::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
  return gScreenLockBusinessLogicStub->tklock_open(service, path, interface, method, mode, silent, flicker);
}

int ScreenLockBusinessLogic::tklock_close(bool silent) {
  return gScreenLockBusinessLogicStub->tklock_close(silent);
}

void ScreenLockBusinessLogic::toggleScreenLockUI(bool toggle) {
  gScreenLockBusinessLogicStub->toggleScreenLockUI(toggle);
}

void ScreenLockBusinessLogic::toggleEventEater(bool toggle) {
  gScreenLockBusinessLogicStub->toggleEventEater(toggle);
}

void ScreenLockBusinessLogic::registerExtension(MApplicationExtensionInterface *extension) {
  gScreenLockBusinessLogicStub->registerExtension(extension);
}

void ScreenLockBusinessLogic::unregisterExtension(MApplicationExtensionInterface *extension) {
  gScreenLockBusinessLogicStub->unregisterExtension(extension);
}

void ScreenLockBusinessLogic::reset() {
  gScreenLockBusinessLogicStub->reset();
}

void ScreenLockBusinessLogic::unlockScreen() {
  gScreenLockBusinessLogicStub->unlockScreen();
}

void ScreenLockBusinessLogic::showScreenLock() {
  gScreenLockBusinessLogicStub->showScreenLock();
}

void ScreenLockBusinessLogic::showLowPowerMode() {
  gScreenLockBusinessLogicStub->showLowPowerMode();
}

void ScreenLockBusinessLogic::setDisplayOffMode() {
  gScreenLockBusinessLogicStub->setDisplayOffMode();
}

void ScreenLockBusinessLogic::hideScreenLockAndEventEater() {
  gScreenLockBusinessLogicStub->hideScreenLockAndEventEater();
}

void ScreenLockBusinessLogic::showEventEater() {
  gScreenLockBusinessLogicStub->showEventEater();
}

void ScreenLockBusinessLogic::hideEventEater() {
  gScreenLockBusinessLogicStub->hideEventEater();
}

bool ScreenLockBusinessLogic::isScreenLocked() const {
  return gScreenLockBusinessLogicStub->isScreenLocked();
}

#ifdef HAVE_QMSYSTEM
void ScreenLockBusinessLogic::displayStateChanged(MeeGo::QmDisplayState::DisplayState state) {
  gScreenLockBusinessLogicStub->displayStateChanged(state);
}

void ScreenLockBusinessLogic::systemStateChanged(MeeGo::QmSystemState::StateIndication what) {
  gScreenLockBusinessLogicStub->systemStateChanged(what);
}
#endif


#endif
