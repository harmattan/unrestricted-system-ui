#ifndef SCREENLOCKBUSINESSLOGICADAPTOR_STUB
#define SCREENLOCKBUSINESSLOGICADAPTOR_STUB

#include "screenlockbusinesslogicadaptor.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ScreenLockBusinessLogicAdaptorStub : public StubBase {
  public:
  virtual void ScreenLockBusinessLogicAdaptorConstructor(ScreenLockBusinessLogic *parent);
  virtual void ScreenLockBusinessLogicAdaptorDestructor();
  virtual int tklock_close(bool silent);
  virtual int tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker);
}; 

// 2. IMPLEMENT STUB
void ScreenLockBusinessLogicAdaptorStub::ScreenLockBusinessLogicAdaptorConstructor(ScreenLockBusinessLogic *parent) {
  Q_UNUSED(parent);

}
void ScreenLockBusinessLogicAdaptorStub::ScreenLockBusinessLogicAdaptorDestructor() {

}
int ScreenLockBusinessLogicAdaptorStub::tklock_close(bool silent) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(silent));
  stubMethodEntered("tklock_close",params);
  return stubReturnValue<int>("tklock_close");
}

int ScreenLockBusinessLogicAdaptorStub::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
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



// 3. CREATE A STUB INSTANCE
ScreenLockBusinessLogicAdaptorStub gDefaultScreenLockBusinessLogicAdaptorStub;
ScreenLockBusinessLogicAdaptorStub* gScreenLockBusinessLogicAdaptorStub = &gDefaultScreenLockBusinessLogicAdaptorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ScreenLockBusinessLogicAdaptor::ScreenLockBusinessLogicAdaptor(ScreenLockBusinessLogic *parent) : QDBusAbstractAdaptor(parent) {
  gScreenLockBusinessLogicAdaptorStub->ScreenLockBusinessLogicAdaptorConstructor(parent);
}

ScreenLockBusinessLogicAdaptor::~ScreenLockBusinessLogicAdaptor() {
  gScreenLockBusinessLogicAdaptorStub->ScreenLockBusinessLogicAdaptorDestructor();
}

int ScreenLockBusinessLogicAdaptor::tklock_close(bool silent) {
  return gScreenLockBusinessLogicAdaptorStub->tklock_close(silent);
}

int ScreenLockBusinessLogicAdaptor::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
  return gScreenLockBusinessLogicAdaptorStub->tklock_open(service, path, interface, method, mode, silent, flicker);
}


#endif
