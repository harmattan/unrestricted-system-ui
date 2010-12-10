#ifndef VOLUMEBARLOGIC_STUB
#define VOLUMEBARLOGIC_STUB

#include "volumebarlogic.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class VolumeBarLogicStub : public StubBase {
  public:
  virtual void VolumeBarLogicConstructor(QObject *parent);
  virtual void VolumeBarLogicDestructor();
  virtual quint32 volume();
  virtual quint32 maxVolume();
  virtual void setVolume(quint32 value);
  virtual void stepsUpdated(quint32 value, quint32 maxvalue);
  virtual void initValues();
  virtual void hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state);
  virtual void hwKeyResourceAcquired();
  virtual void hwKeyResourceLost();
  virtual void addSignalMatch();
  virtual void openConnection(bool init);
  virtual void ping();
  virtual void stepsUpdatedSignal(DBusConnection *conn, DBusMessage *message, VolumeBarLogic *logic);
}; 

// 2. IMPLEMENT STUB
void VolumeBarLogicStub::VolumeBarLogicConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void VolumeBarLogicStub::VolumeBarLogicDestructor() {

}
quint32 VolumeBarLogicStub::volume() {
  stubMethodEntered("volume");
  return stubReturnValue<quint32>("volume");
}

quint32 VolumeBarLogicStub::maxVolume() {
  stubMethodEntered("maxVolume");
  return stubReturnValue<quint32>("maxVolume");
}

void VolumeBarLogicStub::setVolume(quint32 value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<quint32 >(value));
  stubMethodEntered("setVolume",params);
}

void VolumeBarLogicStub::stepsUpdated(quint32 value, quint32 maxvalue) {
  QList<ParameterBase*> params;
  params.append( new Parameter<quint32 >(value));
  params.append( new Parameter<quint32 >(maxvalue));
  stubMethodEntered("stepsUpdated",params);
}

void VolumeBarLogicStub::initValues() {
  stubMethodEntered("initValues");
}

void VolumeBarLogicStub::hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmKeys::Key >(key));
  params.append( new Parameter<MeeGo::QmKeys::State >(state));
  stubMethodEntered("hwKeyEvent",params);
}

void VolumeBarLogicStub::hwKeyResourceAcquired() {
  stubMethodEntered("hwKeyResourceAcquired");
}

void VolumeBarLogicStub::hwKeyResourceLost() {
  stubMethodEntered("hwKeyResourceLost");
}

void VolumeBarLogicStub::addSignalMatch() {
  stubMethodEntered("addSignalMatch");
}

void VolumeBarLogicStub::openConnection(bool init) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(init));
  stubMethodEntered("openConnection",params);
}

void VolumeBarLogicStub::ping() {
  stubMethodEntered("ping");
}

void VolumeBarLogicStub::stepsUpdatedSignal(DBusConnection *conn, DBusMessage *message, VolumeBarLogic *logic) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DBusConnection * >(conn));
  params.append( new Parameter<DBusMessage * >(message));
  params.append( new Parameter<VolumeBarLogic * >(logic));
  stubMethodEntered("stepsUpdatedSignal",params);
}



// 3. CREATE A STUB INSTANCE
VolumeBarLogicStub gDefaultVolumeBarLogicStub;
VolumeBarLogicStub* gVolumeBarLogicStub = &gDefaultVolumeBarLogicStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
VolumeBarLogic::VolumeBarLogic(QObject *parent) {
  gVolumeBarLogicStub->VolumeBarLogicConstructor(parent);
}

VolumeBarLogic::~VolumeBarLogic() {
  gVolumeBarLogicStub->VolumeBarLogicDestructor();
}

quint32 VolumeBarLogic::volume() {
  return gVolumeBarLogicStub->volume();
}

quint32 VolumeBarLogic::maxVolume() {
  return gVolumeBarLogicStub->maxVolume();
}

void VolumeBarLogic::setVolume(quint32 value) {
  gVolumeBarLogicStub->setVolume(value);
}

void VolumeBarLogic::stepsUpdated(quint32 value, quint32 maxvalue) {
  gVolumeBarLogicStub->stepsUpdated(value, maxvalue);
}

void VolumeBarLogic::initValues() {
  gVolumeBarLogicStub->initValues();
}

void VolumeBarLogic::hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state) {
  gVolumeBarLogicStub->hwKeyEvent(key, state);
}

void VolumeBarLogic::hwKeyResourceAcquired() {
  gVolumeBarLogicStub->hwKeyResourceAcquired();
}

void VolumeBarLogic::hwKeyResourceLost() {
  gVolumeBarLogicStub->hwKeyResourceLost();
}

void VolumeBarLogic::addSignalMatch() {
  gVolumeBarLogicStub->addSignalMatch();
}

void VolumeBarLogic::openConnection(bool init) {
  gVolumeBarLogicStub->openConnection(init);
}

void VolumeBarLogic::ping() {
  gVolumeBarLogicStub->ping();
}

void VolumeBarLogic::stepsUpdatedSignal(DBusConnection *conn, DBusMessage *message, VolumeBarLogic *logic) {
  gVolumeBarLogicStub->stepsUpdatedSignal(conn, message, logic);
}


#endif
