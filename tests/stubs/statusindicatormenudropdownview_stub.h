#ifndef STATUSINDICATORMENUDROPDOWNVIEW_STUB
#define STATUSINDICATORMENUDROPDOWNVIEW_STUB

#include "statusindicatormenudropdownview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorMenuDropDownViewStub : public StubBase {
  public:
  virtual void StatusIndicatorMenuDropDownViewDestructor();
  virtual void applyStyle();
  virtual void setPannabilityAndLayout();
  virtual void resetViewport();
  virtual MApplicationExtensionArea * createVerticalExtensionArea();
  virtual QGraphicsWidget * createTopRow();
  virtual MPannableViewport * createPannableArea();
  virtual QGraphicsWidget * createCloseButtonRow();
  virtual MOverlay * createCloseButtonOverlay();
};

// 2. IMPLEMENT STUB
void StatusIndicatorMenuDropDownViewStub::StatusIndicatorMenuDropDownViewDestructor() {

}
void StatusIndicatorMenuDropDownViewStub::applyStyle() {
  stubMethodEntered("applyStyle");
}

void StatusIndicatorMenuDropDownViewStub::setPannabilityAndLayout() {
  stubMethodEntered("setPannabilityAndLayout");
}

void StatusIndicatorMenuDropDownViewStub::resetViewport() {
  stubMethodEntered("resetViewport");
}

MApplicationExtensionArea * StatusIndicatorMenuDropDownViewStub::createVerticalExtensionArea() {
  stubMethodEntered("createVerticalExtensionArea");
  return stubReturnValue<MApplicationExtensionArea *>("createVerticalExtensionArea");
}

QGraphicsWidget * StatusIndicatorMenuDropDownViewStub::createTopRow() {
  stubMethodEntered("createTopRow");
  return stubReturnValue<QGraphicsWidget *>("createTopRow");
}

MPannableViewport * StatusIndicatorMenuDropDownViewStub::createPannableArea() {
  stubMethodEntered("createPannableArea");
  return stubReturnValue<MPannableViewport *>("createPannableArea");
}

QGraphicsWidget * StatusIndicatorMenuDropDownViewStub::createCloseButtonRow() {
  stubMethodEntered("createCloseButtonRow");
  return stubReturnValue<QGraphicsWidget *>("createCloseButtonRow");
}

MOverlay * StatusIndicatorMenuDropDownViewStub::createCloseButtonOverlay() {
  stubMethodEntered("createCloseButtonOverlay");
  return stubReturnValue<MOverlay *>("createCloseButtonOverlay");
}



// 3. CREATE A STUB INSTANCE
StatusIndicatorMenuDropDownViewStub gDefaultStatusIndicatorMenuDropDownViewStub;
StatusIndicatorMenuDropDownViewStub* gStatusIndicatorMenuDropDownViewStub = &gDefaultStatusIndicatorMenuDropDownViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorMenuDropDownView::~StatusIndicatorMenuDropDownView() {
  gStatusIndicatorMenuDropDownViewStub->StatusIndicatorMenuDropDownViewDestructor();
}

void StatusIndicatorMenuDropDownView::applyStyle() {
  gStatusIndicatorMenuDropDownViewStub->applyStyle();
}

void StatusIndicatorMenuDropDownView::setPannabilityAndLayout() {
  gStatusIndicatorMenuDropDownViewStub->setPannabilityAndLayout();
}

void StatusIndicatorMenuDropDownView::resetViewport() {
  gStatusIndicatorMenuDropDownViewStub->resetViewport();
}

MApplicationExtensionArea * StatusIndicatorMenuDropDownView::createVerticalExtensionArea() {
  return gStatusIndicatorMenuDropDownViewStub->createVerticalExtensionArea();
}

QGraphicsWidget * StatusIndicatorMenuDropDownView::createTopRow() {
  return gStatusIndicatorMenuDropDownViewStub->createTopRow();
}

MPannableViewport * StatusIndicatorMenuDropDownView::createPannableArea() {
  return gStatusIndicatorMenuDropDownViewStub->createPannableArea();
}

QGraphicsWidget * StatusIndicatorMenuDropDownView::createCloseButtonRow() {
  return gStatusIndicatorMenuDropDownViewStub->createCloseButtonRow();
}

MOverlay * StatusIndicatorMenuDropDownView::createCloseButtonOverlay() {
  return gStatusIndicatorMenuDropDownViewStub->createCloseButtonOverlay();
}


#endif
