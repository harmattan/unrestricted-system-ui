#ifndef SHUTDOWNBUSINESSLOGICADAPTOR_STUB
#define SHUTDOWNBUSINESSLOGICADAPTOR_STUB

#include "shutdownbusinesslogic.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ShutdownBusinessLogicAdaptorStub : public StubBase {
  public:
  virtual void ShutdownBusinessLogicAdaptorConstructor(QObject *parent, ShutdownBusinessLogic *logic);
  virtual void showScreen(QString text1, QString text2, int timeout);
   ShutdownBusinessLogic *m_logic ;
}; 

// 2. IMPLEMENT STUB
void ShutdownBusinessLogicAdaptorStub::ShutdownBusinessLogicAdaptorConstructor(QObject *parent, ShutdownBusinessLogic *logic) {
  Q_UNUSED(parent);
  Q_UNUSED(logic);

}
void ShutdownBusinessLogicAdaptorStub::showScreen(QString text1, QString text2, int timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(text1));
  params.append( new Parameter<QString >(text2));
  params.append( new Parameter<int >(timeout));
  stubMethodEntered("showScreen",params);
}



// 3. CREATE A STUB INSTANCE
ShutdownBusinessLogicAdaptorStub gDefaultShutdownBusinessLogicAdaptorStub;
ShutdownBusinessLogicAdaptorStub* gShutdownBusinessLogicAdaptorStub = &gDefaultShutdownBusinessLogicAdaptorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ShutdownBusinessLogicAdaptor::ShutdownBusinessLogicAdaptor(QObject *parent, ShutdownBusinessLogic *logic) : QDBusAbstractAdaptor(parent) {
  gShutdownBusinessLogicAdaptorStub->ShutdownBusinessLogicAdaptorConstructor(parent, logic);
}

void ShutdownBusinessLogicAdaptor::showScreen(QString text1, QString text2, int timeout) {
  gShutdownBusinessLogicAdaptorStub->showScreen(text1, text2, timeout);
}


#endif
