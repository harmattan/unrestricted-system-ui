#ifndef BATTERYBUSINESSLOGIC_STUB
#define BATTERYBUSINESSLOGIC_STUB

#include "batterybusinesslogic.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class BatteryBusinessLogicStub : public StubBase {
  public:
  virtual void BatteryBusinessLogicConstructor(QObject *parent);
  virtual void BatteryBusinessLogicDestructor();
  virtual void initBattery();
  virtual void lowBatteryAlert();
  virtual void batteryStateChanged(MeeGo::QmBattery::BatteryState state);
  virtual void chargingStateChanged(MeeGo::QmBattery::ChargingState state);
  virtual void batteryChargerEvent(MeeGo::QmBattery::ChargerType type);
  virtual void devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState PSMState);
  virtual void utiliseLED(bool activate, const QString &pattern);
  virtual void sendNotification(BatteryBusinessLogic::NotificationID id);
  virtual void sendNotification(const QString &eventType, const QString &text, const QString &icon);
  virtual QString chargingImageId();
  virtual void setTouchScreenLockActive(bool active);
}; 

// 2. IMPLEMENT STUB
void BatteryBusinessLogicStub::BatteryBusinessLogicConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void BatteryBusinessLogicStub::BatteryBusinessLogicDestructor() {

}
void BatteryBusinessLogicStub::initBattery() {
  stubMethodEntered("initBattery");
}

void BatteryBusinessLogicStub::lowBatteryAlert() {
  stubMethodEntered("lowBatteryAlert");
}

void BatteryBusinessLogicStub::batteryStateChanged(MeeGo::QmBattery::BatteryState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmBattery::BatteryState >(state));
  stubMethodEntered("batteryStateChanged",params);
}

void BatteryBusinessLogicStub::chargingStateChanged(MeeGo::QmBattery::ChargingState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmBattery::ChargingState >(state));
  stubMethodEntered("chargingStateChanged",params);
}

void BatteryBusinessLogicStub::batteryChargerEvent(MeeGo::QmBattery::ChargerType type) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmBattery::ChargerType >(type));
  stubMethodEntered("batteryChargerEvent",params);
}

void BatteryBusinessLogicStub::devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState PSMState) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmDeviceMode::PSMState >(PSMState));
  stubMethodEntered("devicePSMStateChanged",params);
}

void BatteryBusinessLogicStub::utiliseLED(bool activate, const QString &pattern) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(activate));
  params.append( new Parameter<const QString & >(pattern));
  stubMethodEntered("utiliseLED",params);
}

void BatteryBusinessLogicStub::sendNotification(BatteryBusinessLogic::NotificationID id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<BatteryBusinessLogic::NotificationID >(id));
  stubMethodEntered("sendNotification",params);
}

void BatteryBusinessLogicStub::sendNotification(const QString &eventType, const QString &text, const QString &icon) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(eventType));
  params.append( new Parameter<const QString & >(text));
  params.append( new Parameter<const QString & >(icon));
  stubMethodEntered("sendNotification",params);
}

QString BatteryBusinessLogicStub::chargingImageId() {
  stubMethodEntered("chargingImageId");
  return stubReturnValue<QString>("chargingImageId");
}

void BatteryBusinessLogicStub::setTouchScreenLockActive(bool active) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(active));
  stubMethodEntered("setTouchScreenLockActive",params);
}



// 3. CREATE A STUB INSTANCE
BatteryBusinessLogicStub gDefaultBatteryBusinessLogicStub;
BatteryBusinessLogicStub* gBatteryBusinessLogicStub = &gDefaultBatteryBusinessLogicStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
BatteryBusinessLogic::BatteryBusinessLogic(QObject *parent) {
  gBatteryBusinessLogicStub->BatteryBusinessLogicConstructor(parent);
}

BatteryBusinessLogic::~BatteryBusinessLogic() {
  gBatteryBusinessLogicStub->BatteryBusinessLogicDestructor();
}

void BatteryBusinessLogic::initBattery() {
  gBatteryBusinessLogicStub->initBattery();
}

void BatteryBusinessLogic::lowBatteryAlert() {
  gBatteryBusinessLogicStub->lowBatteryAlert();
}

void BatteryBusinessLogic::batteryStateChanged(MeeGo::QmBattery::BatteryState state) {
  gBatteryBusinessLogicStub->batteryStateChanged(state);
}

void BatteryBusinessLogic::chargingStateChanged(MeeGo::QmBattery::ChargingState state) {
  gBatteryBusinessLogicStub->chargingStateChanged(state);
}

void BatteryBusinessLogic::batteryChargerEvent(MeeGo::QmBattery::ChargerType type) {
  gBatteryBusinessLogicStub->batteryChargerEvent(type);
}

void BatteryBusinessLogic::devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState PSMState) {
  gBatteryBusinessLogicStub->devicePSMStateChanged(PSMState);
}

void BatteryBusinessLogic::utiliseLED(bool activate, const QString &pattern) {
  gBatteryBusinessLogicStub->utiliseLED(activate, pattern);
}

void BatteryBusinessLogic::sendNotification(BatteryBusinessLogic::NotificationID id) {
  gBatteryBusinessLogicStub->sendNotification(id);
}

void BatteryBusinessLogic::sendNotification(const QString &eventType, const QString &text, const QString &icon) {
  gBatteryBusinessLogicStub->sendNotification(eventType, text, icon);
}

QString BatteryBusinessLogic::chargingImageId() {
  return gBatteryBusinessLogicStub->chargingImageId();
}

void BatteryBusinessLogic::setTouchScreenLockActive(bool active) {
  gBatteryBusinessLogicStub->setTouchScreenLockActive(active);
}

#endif
