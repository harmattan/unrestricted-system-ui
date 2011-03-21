#ifndef SHUTDOWNUI_STUB
#define SHUTDOWNUI_STUB

#include "shutdownui.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ShutdownUIStub : public StubBase {
  public:
  virtual void ShutdownUIConstructor();
  virtual void ShutdownUIDestructor();
  virtual void showWindow(const QString &text1, const QString &text2, int timeout);
  virtual void showEvent(QShowEvent *event);
  virtual void showLogo();
  virtual void turnOffScreen();
  virtual void realize();
}; 

// 2. IMPLEMENT STUB
void ShutdownUIStub::ShutdownUIConstructor() {

}
void ShutdownUIStub::ShutdownUIDestructor() {

}
void ShutdownUIStub::showWindow(const QString &text1, const QString &text2, int timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(text1));
  params.append( new Parameter<const QString & >(text2));
  params.append( new Parameter<int >(timeout));
  stubMethodEntered("showWindow",params);
}

void ShutdownUIStub::showEvent(QShowEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QShowEvent * >(event));
  stubMethodEntered("showEvent",params);
}

void ShutdownUIStub::showLogo() {
  stubMethodEntered("showLogo");
}

void ShutdownUIStub::turnOffScreen() {
  stubMethodEntered("turnOffScreen");
}

void ShutdownUIStub::realize() {
  stubMethodEntered("realize");
}



// 3. CREATE A STUB INSTANCE
ShutdownUIStub gDefaultShutdownUIStub;
ShutdownUIStub* gShutdownUIStub = &gDefaultShutdownUIStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ShutdownUI::ShutdownUI() {
  gShutdownUIStub->ShutdownUIConstructor();
}

ShutdownUI::~ShutdownUI() {
  gShutdownUIStub->ShutdownUIDestructor();
}

void ShutdownUI::showWindow(const QString &text1, const QString &text2, int timeout) {
  gShutdownUIStub->showWindow(text1, text2, timeout);
}

void ShutdownUI::showEvent(QShowEvent *event) {
  gShutdownUIStub->showEvent(event);
}

void ShutdownUI::showLogo() {
  gShutdownUIStub->showLogo();
}

void ShutdownUI::turnOffScreen() {
  gShutdownUIStub->turnOffScreen();
}

void ShutdownUI::realize() {
  gShutdownUIStub->realize();
}


#endif
