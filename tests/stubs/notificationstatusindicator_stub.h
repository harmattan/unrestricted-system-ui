#ifndef NOTIFICATIONSTATUSINDICATOR_STUB
#define NOTIFICATIONSTATUSINDICATOR_STUB

#include "notificationstatusindicator.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationStatusIndicatorStub : public StubBase {
  public:
  virtual void NotificationStatusIndicatorConstructor(NotifierNotificationSink *notifierSink, QGraphicsItem *parent);
  virtual void NotificationStatusIndicatorDestructor();
  virtual void setActive(bool active);
}; 

// 2. IMPLEMENT STUB
void NotificationStatusIndicatorStub::NotificationStatusIndicatorConstructor(NotifierNotificationSink *, QGraphicsItem *) {
}
void NotificationStatusIndicatorStub::NotificationStatusIndicatorDestructor() {

}

void NotificationStatusIndicatorStub::setActive(bool active) {
    QList<ParameterBase*> params;
    params.append( new Parameter<bool >(active));
    stubMethodEntered("setActive",params);
}


// 3. CREATE A STUB INSTANCE
NotificationStatusIndicatorStub gDefaultNotificationStatusIndicatorStub;
NotificationStatusIndicatorStub* gNotificationStatusIndicatorStub = &gDefaultNotificationStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationStatusIndicator::NotificationStatusIndicator(NotifierNotificationSink *notifierSink, QGraphicsItem *parent)
{
  gNotificationStatusIndicatorStub->NotificationStatusIndicatorConstructor(notifierSink, parent);
}

NotificationStatusIndicator::~NotificationStatusIndicator() {
  gNotificationStatusIndicatorStub->NotificationStatusIndicatorDestructor();
}

void NotificationStatusIndicator::setActive(bool active) {
  gNotificationStatusIndicatorStub->setActive(active);
}


#endif
