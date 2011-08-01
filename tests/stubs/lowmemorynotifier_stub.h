#ifndef LOWMEMORYNOTIFIER_STUB
#define LOWMEMORYNOTIFIER_STUB

#include <memnotify/memory_notification.hpp>
#include "lowmemorynotifier.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LowMemoryNotifierStub : public StubBase {
  public:
  virtual void LowMemoryNotifierConstructor(QObject *parent);
  virtual void LowMemoryNotifierDestructor();
  virtual void handleLowMemoryNotification(const QString &name, const bool state);
}; 

// 2. IMPLEMENT STUB
void LowMemoryNotifierStub::LowMemoryNotifierConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void LowMemoryNotifierStub::LowMemoryNotifierDestructor() {

}
void LowMemoryNotifierStub::handleLowMemoryNotification(const QString &name, const bool state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(name));
  params.append( new Parameter<const bool >(state));
  stubMethodEntered("handleLowMemoryNotification",params);
}



// 3. CREATE A STUB INSTANCE
LowMemoryNotifierStub gDefaultLowMemoryNotifierStub;
LowMemoryNotifierStub* gLowMemoryNotifierStub = &gDefaultLowMemoryNotifierStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LowMemoryNotifier::LowMemoryNotifier(QObject *parent) : memoryNotification(MEMNOTIFY::MemoryNotification::defaultObject()) {
  gLowMemoryNotifierStub->LowMemoryNotifierConstructor(parent);
}

LowMemoryNotifier::~LowMemoryNotifier() {
  gLowMemoryNotifierStub->LowMemoryNotifierDestructor();
}

void LowMemoryNotifier::handleLowMemoryNotification(const QString &name, const bool state) {
  gLowMemoryNotifierStub->handleLowMemoryNotification(name, state);
}


#endif
