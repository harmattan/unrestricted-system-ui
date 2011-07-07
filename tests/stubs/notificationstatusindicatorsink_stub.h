#ifndef NOTIFICATIONSTATUSINDICATORSINK_STUB
#define NOTIFICATIONSTATUSINDICATORSINK_STUB

#include "notificationstatusindicatorsink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationStatusIndicatorSinkStub : public StubBase {
  public:
  virtual void NotificationStatusIndicatorSinkConstructor(QObject *parent);
  virtual void NotificationStatusIndicatorSinkDestructor();
  virtual void addNotification(const Notification &notification);
  virtual void removeNotification(uint notificationId);
  virtual void addGroup(uint groupId, const NotificationParameters &parameters);
  virtual void removeGroup(uint groupId);
  virtual void addStandaloneNotification(int notificationId, const QString &iconId, int priority);
  virtual void removeStandaloneNotification(int notificationId);
  virtual void addNotificationToGroup(int notificationId, int groupId);
  virtual void removeNotificationFromGroup(int notificationId, int groupId);
  virtual void updateNotificationData(int notificationId, const QString &iconId, int priority);
  virtual void updateGroupData(int groupId, const QString &iconId, int priority);
  virtual void checkMostImportantNotification();
}; 

// 2. IMPLEMENT STUB
void NotificationStatusIndicatorSinkStub::NotificationStatusIndicatorSinkConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void NotificationStatusIndicatorSinkStub::NotificationStatusIndicatorSinkDestructor() {

}

void NotificationStatusIndicatorSinkStub::addNotification(const Notification &notification) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const Notification & >(notification));
  stubMethodEntered("addNotification",params);
}

void NotificationStatusIndicatorSinkStub::removeNotification(uint notificationId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationId));
  stubMethodEntered("removeNotification",params);
}

void NotificationStatusIndicatorSinkStub::addGroup(uint groupId, const NotificationParameters &parameters) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(groupId));
  params.append( new Parameter<const NotificationParameters & >(parameters));
  stubMethodEntered("addGroup",params);
}

void NotificationStatusIndicatorSinkStub::removeGroup(uint groupId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(groupId));
  stubMethodEntered("removeGroup",params);
}

void NotificationStatusIndicatorSinkStub::addStandaloneNotification(int notificationId, const QString &iconId, int priority) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(notificationId));
  params.append( new Parameter<const QString & >(iconId));
  params.append( new Parameter<int >(priority));
  stubMethodEntered("addStandaloneNotification",params);
}

void NotificationStatusIndicatorSinkStub::removeStandaloneNotification(int notificationId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(notificationId));
  stubMethodEntered("removeStandaloneNotification",params);
}

void NotificationStatusIndicatorSinkStub::addNotificationToGroup(int notificationId, int groupId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(notificationId));
  params.append( new Parameter<int >(groupId));
  stubMethodEntered("addNotificationToGroup",params);
}

void NotificationStatusIndicatorSinkStub::removeNotificationFromGroup(int notificationId, int groupId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(notificationId));
  params.append( new Parameter<int >(groupId));
  stubMethodEntered("removeNotificationFromGroup",params);
}

void NotificationStatusIndicatorSinkStub::updateNotificationData(int notificationId, const QString &iconId, int priority) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(notificationId));
  params.append( new Parameter<const QString & >(iconId));
  params.append( new Parameter<int >(priority));
  stubMethodEntered("updateNotificationData",params);
}

void NotificationStatusIndicatorSinkStub::updateGroupData(int groupId, const QString &iconId, int priority) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(groupId));
  params.append( new Parameter<const QString & >(iconId));
  params.append( new Parameter<int >(priority));
  stubMethodEntered("updateGroupData",params);
}

void NotificationStatusIndicatorSinkStub::checkMostImportantNotification() {
  stubMethodEntered("checkMostImportantNotification");
}



// 3. CREATE A STUB INSTANCE
NotificationStatusIndicatorSinkStub gDefaultNotificationStatusIndicatorSinkStub;
NotificationStatusIndicatorSinkStub* gNotificationStatusIndicatorSinkStub = &gDefaultNotificationStatusIndicatorSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationStatusIndicatorSink::NotificationStatusIndicatorSink(QObject *parent) {
  gNotificationStatusIndicatorSinkStub->NotificationStatusIndicatorSinkConstructor(parent);
}

NotificationStatusIndicatorSink::~NotificationStatusIndicatorSink() {
  gNotificationStatusIndicatorSinkStub->NotificationStatusIndicatorSinkDestructor();
}

void NotificationStatusIndicatorSink::addNotification(const Notification &notification) {
  gNotificationStatusIndicatorSinkStub->addNotification(notification);
}

void NotificationStatusIndicatorSink::removeNotification(uint notificationId) {
  gNotificationStatusIndicatorSinkStub->removeNotification(notificationId);
}

void NotificationStatusIndicatorSink::addGroup(uint groupId, const NotificationParameters &parameters) {
  gNotificationStatusIndicatorSinkStub->addGroup(groupId, parameters);
}

void NotificationStatusIndicatorSink::removeGroup(uint groupId) {
  gNotificationStatusIndicatorSinkStub->removeGroup(groupId);
}

void NotificationStatusIndicatorSink::addStandaloneNotification(int notificationId, const QString &iconId, int priority) {
  gNotificationStatusIndicatorSinkStub->addStandaloneNotification(notificationId, iconId, priority);
}

void NotificationStatusIndicatorSink::removeStandaloneNotification(int notificationId) {
  gNotificationStatusIndicatorSinkStub->removeStandaloneNotification(notificationId);
}

void NotificationStatusIndicatorSink::addNotificationToGroup(int notificationId, int groupId) {
  gNotificationStatusIndicatorSinkStub->addNotificationToGroup(notificationId, groupId);
}

void NotificationStatusIndicatorSink::removeNotificationFromGroup(int notificationId, int groupId) {
  gNotificationStatusIndicatorSinkStub->removeNotificationFromGroup(notificationId, groupId);
}

void NotificationStatusIndicatorSink::updateNotificationData(int notificationId, const QString &iconId, int priority) {
  gNotificationStatusIndicatorSinkStub->updateNotificationData(notificationId, iconId, priority);
}

void NotificationStatusIndicatorSink::updateGroupData(int groupId, const QString &iconId, int priority) {
  gNotificationStatusIndicatorSinkStub->updateGroupData(groupId, iconId, priority);
}

void NotificationStatusIndicatorSink::checkMostImportantNotification() {
  gNotificationStatusIndicatorSinkStub->checkMostImportantNotification();
}


#endif
