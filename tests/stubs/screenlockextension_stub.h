#ifndef SCREENLOCKEXTENSION_STUB
#define SCREENLOCKEXTENSION_STUB

#include "screenlockextension.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ScreenLockExtensionStub : public StubBase {
  public:
  virtual void ScreenLockExtensionConstructor();
  virtual void ScreenLockExtensionDestructor();
  virtual void reset();
  virtual void setNotificationManagerInterface(NotificationManagerInterface &notificationManager);
  virtual bool initialize(const QString &interface);
  virtual QGraphicsWidget * widget();
  virtual NotificationManagerInterface * notificationManagerInterface();
}; 

// 2. IMPLEMENT STUB
void ScreenLockExtensionStub::ScreenLockExtensionConstructor() {

}
void ScreenLockExtensionStub::ScreenLockExtensionDestructor() {

}
void ScreenLockExtensionStub::reset() {
  stubMethodEntered("reset");
}

void ScreenLockExtensionStub::setNotificationManagerInterface(NotificationManagerInterface &notificationManager) {
  QList<ParameterBase*> params;
  params.append( new Parameter<NotificationManagerInterface & >(notificationManager));
  stubMethodEntered("setNotificationManagerInterface",params);
}

bool ScreenLockExtensionStub::initialize(const QString &interface) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(interface));
  stubMethodEntered("initialize",params);
  return stubReturnValue<bool>("initialize");
}

QGraphicsWidget * ScreenLockExtensionStub::widget() {
  stubMethodEntered("widget");
  return stubReturnValue<QGraphicsWidget *>("widget");
}

NotificationManagerInterface * ScreenLockExtensionStub::notificationManagerInterface() {
  stubMethodEntered("notificationManagerInterface");
  return stubReturnValue<NotificationManagerInterface *>("notificationManagerInterface");
}



// 3. CREATE A STUB INSTANCE
ScreenLockExtensionStub gDefaultScreenLockExtensionStub;
ScreenLockExtensionStub* gScreenLockExtensionStub = &gDefaultScreenLockExtensionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ScreenLockExtension::ScreenLockExtension() {
  gScreenLockExtensionStub->ScreenLockExtensionConstructor();
}

ScreenLockExtension::~ScreenLockExtension() {
  gScreenLockExtensionStub->ScreenLockExtensionDestructor();
}

void ScreenLockExtension::reset() {
  gScreenLockExtensionStub->reset();
}

void ScreenLockExtension::setNotificationManagerInterface(NotificationManagerInterface &notificationManager) {
  gScreenLockExtensionStub->setNotificationManagerInterface(notificationManager);
}

bool ScreenLockExtension::initialize(const QString &interface) {
  return gScreenLockExtensionStub->initialize(interface);
}

QGraphicsWidget * ScreenLockExtension::widget() {
  return gScreenLockExtensionStub->widget();
}

NotificationManagerInterface * ScreenLockExtension::notificationManagerInterface() {
  return gScreenLockExtensionStub->notificationManagerInterface();
}


#endif
