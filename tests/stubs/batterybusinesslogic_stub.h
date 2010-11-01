#ifndef BATTERYBUSINESSLOGIC_STUB
#define BATTERYBUSINESSLOGIC_STUB

#include "batterybusinesslogic.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class BatteryBusinessLogicStub : public StubBase {
  public:
  virtual void BatteryBusinessLogicConstructor(SystemUIGConf *systemUIGConf, QObject *parent);
  virtual void BatteryBusinessLogicDestructor();
  virtual void setPSMThreshold(const QString &threshold);
  virtual void togglePSM(bool toggle);
  virtual void togglePSMAuto(bool toggle);
  virtual int batteryBarValue(int percentage);
  virtual bool PSMValue();
  virtual void batteryStatus();
  virtual QVariant GConfItemValue(SystemUIGConf::GConfKey key);
  virtual QStringList remainingTimeValues();
  virtual QStringList PSMThresholdValues();
  virtual QString PSMThresholdValue();
  virtual void initBattery();
  virtual void lowBatteryAlert();
  virtual void batteryEnergyLevelChanged(int energyLevel);
#ifdef HAVE_QMSYSTEM
  virtual void batteryStateChanged(MeeGo::QmBattery::BatteryState state);
  virtual void chargingStateChanged(MeeGo::QmBattery::ChargingState state);
  virtual void batteryChargerEvent(MeeGo::QmBattery::ChargerType type);
  virtual void devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState PSMState);
#endif
  virtual void utiliseLED(bool activate, const QString &pattern);
  virtual void setPSMState(bool on);
  virtual void initSystemUIGConfKeys();
#ifdef HAVE_QMSYSTEM
  virtual int animationRate(MeeGo::QmBattery::ChargerType type);
#endif
  virtual void sendNotification(BatteryBusinessLogic::NotificationID id);
  virtual void sendNotification(const QString &text, const QString &feedback, const QString &icon);
}; 

// 2. IMPLEMENT STUB
void BatteryBusinessLogicStub::BatteryBusinessLogicConstructor(SystemUIGConf *systemUIGConf, QObject *parent) {
  Q_UNUSED(systemUIGConf);
  Q_UNUSED(parent);

}
void BatteryBusinessLogicStub::BatteryBusinessLogicDestructor() {

}
void BatteryBusinessLogicStub::setPSMThreshold(const QString &threshold) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(threshold));
  stubMethodEntered("setPSMThreshold",params);
}

void BatteryBusinessLogicStub::togglePSM(bool toggle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(toggle));
  stubMethodEntered("togglePSM",params);
}

void BatteryBusinessLogicStub::togglePSMAuto(bool toggle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(toggle));
  stubMethodEntered("togglePSMAuto",params);
}

int BatteryBusinessLogicStub::batteryBarValue(int percentage) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(percentage));
  stubMethodEntered("batteryBarValue",params);
  return stubReturnValue<int>("batteryBarValue");
}

bool BatteryBusinessLogicStub::PSMValue() {
  stubMethodEntered("PSMValue");
  return stubReturnValue<bool>("PSMValue");
}

void BatteryBusinessLogicStub::batteryStatus() {
  stubMethodEntered("batteryStatus");
}

QVariant BatteryBusinessLogicStub::GConfItemValue(SystemUIGConf::GConfKey key) {
  QList<ParameterBase*> params;
  params.append( new Parameter<SystemUIGConf::GConfKey >(key));
  stubMethodEntered("GConfItemValue",params);
  return stubReturnValue<QVariant>("GConfItemValue");
}

QStringList BatteryBusinessLogicStub::remainingTimeValues() {
  stubMethodEntered("remainingTimeValues");
  return stubReturnValue<QStringList>("remainingTimeValues");
}

QStringList BatteryBusinessLogicStub::PSMThresholdValues() {
  stubMethodEntered("PSMThresholdValues");
  return stubReturnValue<QStringList>("PSMThresholdValues");
}

QString BatteryBusinessLogicStub::PSMThresholdValue() {
  stubMethodEntered("PSMThresholdValue");
  return stubReturnValue<QString>("PSMThresholdValue");
}

void BatteryBusinessLogicStub::initBattery() {
  stubMethodEntered("initBattery");
}

void BatteryBusinessLogicStub::lowBatteryAlert() {
  stubMethodEntered("lowBatteryAlert");
}

void BatteryBusinessLogicStub::batteryEnergyLevelChanged(int energyLevel) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(energyLevel));
  stubMethodEntered("batteryEnergyLevelChanged",params);
}

#ifdef HAVE_QMSYSTEM
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
#endif

void BatteryBusinessLogicStub::utiliseLED(bool activate, const QString &pattern) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(activate));
  params.append( new Parameter<QString>(pattern));
  stubMethodEntered("utiliseLED",params);
}

void BatteryBusinessLogicStub::setPSMState(bool on) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(on));
  stubMethodEntered("setPSMState",params);
}

void BatteryBusinessLogicStub::initSystemUIGConfKeys() {
  stubMethodEntered("initSystemUIGConfKeys");
}

#ifdef HAVE_QMSYSTEM
int BatteryBusinessLogicStub::animationRate(MeeGo::QmBattery::ChargerType type) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmBattery::ChargerType >(type));
  stubMethodEntered("animationRate",params);
  return stubReturnValue<int>("animationRate");
}
#endif

void BatteryBusinessLogicStub::sendNotification(BatteryBusinessLogic::NotificationID id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<BatteryBusinessLogic::NotificationID >(id));
  stubMethodEntered("sendNotification",params);
}

void BatteryBusinessLogicStub::sendNotification(const QString &text, const QString &feedback, const QString &icon) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(text));
  params.append( new Parameter<QString>(feedback));
  params.append( new Parameter<QString>(icon));
  stubMethodEntered("sendNotification",params);
}



// 3. CREATE A STUB INSTANCE
BatteryBusinessLogicStub gDefaultBatteryBusinessLogicStub;
BatteryBusinessLogicStub* gBatteryBusinessLogicStub = &gDefaultBatteryBusinessLogicStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
BatteryBusinessLogic::BatteryBusinessLogic(SystemUIGConf *systemUIGConf, QObject *parent) {
  gBatteryBusinessLogicStub->BatteryBusinessLogicConstructor(systemUIGConf, parent);
}

BatteryBusinessLogic::~BatteryBusinessLogic() {
  gBatteryBusinessLogicStub->BatteryBusinessLogicDestructor();
}

void BatteryBusinessLogic::setPSMThreshold(const QString &threshold) {
  gBatteryBusinessLogicStub->setPSMThreshold(threshold);
}

void BatteryBusinessLogic::togglePSM(bool toggle) {
  gBatteryBusinessLogicStub->togglePSM(toggle);
}

void BatteryBusinessLogic::togglePSMAuto(bool toggle) {
  gBatteryBusinessLogicStub->togglePSMAuto(toggle);
}

int BatteryBusinessLogic::batteryBarValue(int percentage) {
  return gBatteryBusinessLogicStub->batteryBarValue(percentage);
}

bool BatteryBusinessLogic::PSMValue() {
  return gBatteryBusinessLogicStub->PSMValue();
}

void BatteryBusinessLogic::batteryStatus() {
  gBatteryBusinessLogicStub->batteryStatus();
}

QVariant BatteryBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key) {
  return gBatteryBusinessLogicStub->GConfItemValue(key);
}

QStringList BatteryBusinessLogic::remainingTimeValues() {
  return gBatteryBusinessLogicStub->remainingTimeValues();
}

QStringList BatteryBusinessLogic::PSMThresholdValues() {
  return gBatteryBusinessLogicStub->PSMThresholdValues();
}

QString BatteryBusinessLogic::PSMThresholdValue() {
  return gBatteryBusinessLogicStub->PSMThresholdValue();
}

void BatteryBusinessLogic::initBattery() {
  gBatteryBusinessLogicStub->initBattery();
}

void BatteryBusinessLogic::lowBatteryAlert() {
  gBatteryBusinessLogicStub->lowBatteryAlert();
}

void BatteryBusinessLogic::batteryEnergyLevelChanged(int energyLevel) {
  gBatteryBusinessLogicStub->batteryEnergyLevelChanged(energyLevel);
}

#ifdef HAVE_QMSYSTEM
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
#endif

void BatteryBusinessLogic::utiliseLED(bool activate, const QString &pattern) {
  gBatteryBusinessLogicStub->utiliseLED(activate, pattern);
}

void BatteryBusinessLogic::setPSMState(bool on) {
  gBatteryBusinessLogicStub->setPSMState(on);
}

void BatteryBusinessLogic::initSystemUIGConfKeys() {
  gBatteryBusinessLogicStub->initSystemUIGConfKeys();
}

#ifdef HAVE_QMSYSTEM
int BatteryBusinessLogic::animationRate(MeeGo::QmBattery::ChargerType type) {
  return gBatteryBusinessLogicStub->animationRate(type);
}
#endif

void BatteryBusinessLogic::sendNotification(BatteryBusinessLogic::NotificationID id) {
  gBatteryBusinessLogicStub->sendNotification(id);
}

void BatteryBusinessLogic::sendNotification(const QString &text, const QString &feedback, const QString &icon) {
  gBatteryBusinessLogicStub->sendNotification(text, feedback, icon);
}


#endif
