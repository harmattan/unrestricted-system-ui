#ifndef SHUTDOWNBUSINESSLOGIC_STUB
#define SHUTDOWNBUSINESSLOGIC_STUB

#include "shutdownbusinesslogic.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ShutdownBusinessLogicStub : public StubBase {
  public:
  virtual void ShutdownBusinessLogicConstructor(QObject *parent);
  virtual void ShutdownBusinessLogicDestructor();
  virtual void showUI(QString text1, QString text2, int timeout);
  virtual void systemStateChanged(MeeGo::QmSystemState::StateIndication what);
  virtual void thermalShutdown();
  virtual void batteryShutdown();
  virtual void shutdownDeniedUSB();
   ShutdownUI *m_Ui ;
   MeeGo::QmSystemState *m_State ;
}; 

// 2. IMPLEMENT STUB
void ShutdownBusinessLogicStub::ShutdownBusinessLogicConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void ShutdownBusinessLogicStub::ShutdownBusinessLogicDestructor() {

}
void ShutdownBusinessLogicStub::showUI(QString text1, QString text2, int timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(text1));
  params.append( new Parameter<QString >(text2));
  params.append( new Parameter<int >(timeout));
  stubMethodEntered("showUI",params);
}

void ShutdownBusinessLogicStub::systemStateChanged(MeeGo::QmSystemState::StateIndication what) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmSystemState::StateIndication >(what));
  stubMethodEntered("systemStateChanged",params);
}

void ShutdownBusinessLogicStub::thermalShutdown() {
  stubMethodEntered("thermalShutdown");
}

void ShutdownBusinessLogicStub::batteryShutdown() {
  stubMethodEntered("batteryShutdown");
}

void ShutdownBusinessLogicStub::shutdownDeniedUSB() {
  stubMethodEntered("shutdownDeniedUSB");
}



// 3. CREATE A STUB INSTANCE
ShutdownBusinessLogicStub gDefaultShutdownBusinessLogicStub;
ShutdownBusinessLogicStub* gShutdownBusinessLogicStub = &gDefaultShutdownBusinessLogicStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ShutdownBusinessLogic::ShutdownBusinessLogic(QObject *parent) {
  gShutdownBusinessLogicStub->ShutdownBusinessLogicConstructor(parent);
}

ShutdownBusinessLogic::~ShutdownBusinessLogic() {
  gShutdownBusinessLogicStub->ShutdownBusinessLogicDestructor();
}

void ShutdownBusinessLogic::showUI(QString text1, QString text2, int timeout) {
  gShutdownBusinessLogicStub->showUI(text1, text2, timeout);
}

void ShutdownBusinessLogic::systemStateChanged(MeeGo::QmSystemState::StateIndication what) {
  gShutdownBusinessLogicStub->systemStateChanged(what);
}

void ShutdownBusinessLogic::thermalShutdown() {
  gShutdownBusinessLogicStub->thermalShutdown();
}

void ShutdownBusinessLogic::batteryShutdown() {
  gShutdownBusinessLogicStub->batteryShutdown();
}

void ShutdownBusinessLogic::shutdownDeniedUSB() {
  gShutdownBusinessLogicStub->shutdownDeniedUSB();
}


#endif
