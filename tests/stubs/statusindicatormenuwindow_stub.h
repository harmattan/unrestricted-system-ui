#ifndef STATUSINDICATORMENUWINDOW_STUB
#define STATUSINDICATORMENUWINDOW_STUB

#include "statusindicatormenuwindow.h"
#include <stubbase.h>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorMenuWindowStub : public StubBase {
  public:
  virtual void StatusIndicatorMenuWindowConstructor(QWidget *parent);
  virtual void StatusIndicatorMenuWindowDestructor();
  virtual void makeVisible();
  virtual void displayActive();
  virtual void displayInActive();
  virtual bool event(QEvent *);
  virtual void resetMenuWidget();
  virtual void mousePressEvent(QMouseEvent*);
  virtual void mouseReleaseEvent(QMouseEvent*);
#ifdef HAVE_QMSYSTEM
  virtual void setWindowStateAccordingToDeviceLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
#endif
};

// 2. IMPLEMENT STUB
void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowConstructor(QWidget *parent) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QObject * >(parent));
  stubMethodEntered("StatusIndicatorMenuWindow", params);
}
void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowDestructor() {
  stubMethodEntered("~StatusIndicatorMenuWindow");
}
void StatusIndicatorMenuWindowStub::makeVisible() {
  stubMethodEntered("makeVisible");
}

void StatusIndicatorMenuWindowStub::displayActive() {
  stubMethodEntered("displayActive");
}

void StatusIndicatorMenuWindowStub::displayInActive() {
  stubMethodEntered("displayInActive");
}

bool StatusIndicatorMenuWindowStub::event(QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent *>(event));
  stubMethodEntered("event", params);
  return stubReturnValue<bool>("event");
}

void StatusIndicatorMenuWindowStub::resetMenuWidget() {
  stubMethodEntered("resetMenuWidget");
}

void StatusIndicatorMenuWindowStub::mousePressEvent(QMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QMouseEvent>(*event));
  stubMethodEntered("mousePressEvent", params);
}

void StatusIndicatorMenuWindowStub::mouseReleaseEvent(QMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QMouseEvent>(*event));
  stubMethodEntered("mouseReleaseEvent", params);
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindowStub::setWindowStateAccordingToDeviceLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmLocks::Lock>(what));
  params.append( new Parameter<MeeGo::QmLocks::State>(how));
  stubMethodEntered("setWindowStateAccordingToDeviceLockState",params);
}
#endif

// 3. CREATE A STUB INSTANCE
StatusIndicatorMenuWindowStub gDefaultStatusIndicatorMenuWindowStub;
StatusIndicatorMenuWindowStub* gStatusIndicatorMenuWindowStub = &gDefaultStatusIndicatorMenuWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) {
  gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowConstructor(parent);
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow() {
  gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowDestructor();
}

void StatusIndicatorMenuWindow::makeVisible() {
  gStatusIndicatorMenuWindowStub->makeVisible();
}

void StatusIndicatorMenuWindow::displayActive() {
  gStatusIndicatorMenuWindowStub->displayActive();
}

void StatusIndicatorMenuWindow::displayInActive() {
  gStatusIndicatorMenuWindowStub->displayInActive();
}

bool StatusIndicatorMenuWindow::event(QEvent *event) {
  return gStatusIndicatorMenuWindowStub->event(event);
}

void StatusIndicatorMenuWindow::resetMenuWidget() {
  gStatusIndicatorMenuWindowStub->resetMenuWidget();
}

void StatusIndicatorMenuWindow::mousePressEvent(QMouseEvent *event) {
  gStatusIndicatorMenuWindowStub->mousePressEvent(event);
}

void StatusIndicatorMenuWindow::mouseReleaseEvent(QMouseEvent *event) {
  gStatusIndicatorMenuWindowStub->mouseReleaseEvent(event);
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindow::setWindowStateAccordingToDeviceLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how) {
  gStatusIndicatorMenuWindowStub->setWindowStateAccordingToDeviceLockState(what, how);
}
#endif

#endif
