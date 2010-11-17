#ifndef STATUSINDICATORMENU_STUB
#define STATUSINDICATORMENU_STUB

#include "statusindicatormenu.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorMenuStub : public StubBase {
  public:
  virtual void StatusIndicatorMenuConstructor();
  virtual void StatusIndicatorMenuDestructor();
  virtual void showStatusIndicatorMenu();
  virtual void hideStatusIndicatorMenu();
  virtual void setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension);
  virtual void launchControlPanelAndHide();
};

// 2. IMPLEMENT STUB
void StatusIndicatorMenuStub::StatusIndicatorMenuConstructor() {
  stubMethodEntered("StatusIndicatorMenuConstructor");
}

void StatusIndicatorMenuStub::StatusIndicatorMenuDestructor() {
  stubMethodEntered("StatusIndicatorMenuDestructor");
}

void StatusIndicatorMenuStub::showStatusIndicatorMenu() {
  stubMethodEntered("showStatusIndicatorMenu");
}

void StatusIndicatorMenuStub::hideStatusIndicatorMenu() {
  stubMethodEntered("hideStatusIndicatorMenu");
}

void StatusIndicatorMenuStub::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MApplicationExtensionInterface * >(extension));
  stubMethodEntered("setStatusIndicatorMenuInterface",params);
}

void StatusIndicatorMenuStub::launchControlPanelAndHide() {
  stubMethodEntered("launchControlPanelAndHide");
}


// 3. CREATE A STUB INSTANCE
StatusIndicatorMenuStub gDefaultStatusIndicatorMenuStub;
StatusIndicatorMenuStub* gStatusIndicatorMenuStub = &gDefaultStatusIndicatorMenuStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorMenu::StatusIndicatorMenu(QGraphicsItem *parent) {
  Q_UNUSED(parent);
  gStatusIndicatorMenuStub->StatusIndicatorMenuConstructor();
}

StatusIndicatorMenu::~StatusIndicatorMenu() {
  gStatusIndicatorMenuStub->StatusIndicatorMenuDestructor();
}

void StatusIndicatorMenu::showStatusIndicatorMenu() {
  gStatusIndicatorMenuStub->showStatusIndicatorMenu();
}

void StatusIndicatorMenu::hideStatusIndicatorMenu() {
  gStatusIndicatorMenuStub->hideStatusIndicatorMenu();
}

void StatusIndicatorMenu::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension) {
  gStatusIndicatorMenuStub->setStatusIndicatorMenuInterface(extension);
}

void StatusIndicatorMenu::launchControlPanelAndHide() {
  gStatusIndicatorMenuStub->launchControlPanelAndHide();
}

#endif
