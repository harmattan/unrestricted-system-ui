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
  virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
};

// 2. IMPLEMENT STUB
void StatusIndicatorMenuStub::StatusIndicatorMenuConstructor() {

}
void StatusIndicatorMenuStub::StatusIndicatorMenuDestructor() {

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

void StatusIndicatorMenuStub::mousePressEvent(QGraphicsSceneMouseEvent*) {
  stubMethodEntered("mousePressEvent");
}

void StatusIndicatorMenuStub::mouseReleaseEvent(QGraphicsSceneMouseEvent*) {
  stubMethodEntered("mouseReleaseEvent");
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

void StatusIndicatorMenu::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gStatusIndicatorMenuStub->mousePressEvent(event);
}

void StatusIndicatorMenu::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  gStatusIndicatorMenuStub->mouseReleaseEvent(event);
}

#endif
