#ifndef STATUSINDICATORICONVIEW_STUB
#define STATUSINDICATORICONVIEW_STUB

#include "statusindicatoriconview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorAnimationViewStub : public StubBase {
  public:
  virtual void StatusIndicatorAnimationViewConstructor(StatusIndicator *controller);
  virtual void StatusIndicatorAnimationViewDestructor();
  virtual void updateData(const QList<const char *> &modifications);
  virtual void startAnimation();
  virtual void stopAnimation();
  virtual QSizeF sizeHint (Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const;
  virtual void setAnimationFrame(int frame);
  virtual void setFirstAnimationFrame(int frame);
  virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
  virtual void setupModel();
  virtual void applyStyle();
  virtual void setupAnimationTimeline();
  virtual void clearImageList();
  virtual void setupImageList(const QString &iconIDs);
}; 

// 2. IMPLEMENT STUB
void StatusIndicatorAnimationViewStub::StatusIndicatorAnimationViewConstructor(StatusIndicator *controller) {
  Q_UNUSED(controller);

}
void StatusIndicatorAnimationViewStub::StatusIndicatorAnimationViewDestructor() {

}
void StatusIndicatorAnimationViewStub::updateData(const QList<const char *> &modifications) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<const char *> & >(modifications));
  stubMethodEntered("updateData",params);
}

void StatusIndicatorAnimationViewStub::startAnimation() {
  stubMethodEntered("startAnimation");
}

void StatusIndicatorAnimationViewStub::stopAnimation() {
  stubMethodEntered("stopAnimation");
}

QSizeF StatusIndicatorAnimationViewStub::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::SizeHint>(which));
  params.append( new Parameter<const QSizeF& > (constraint));
  stubMethodEntered("sizeHint");
  return stubReturnValue<QSizeF>("sizeHint");
}

void StatusIndicatorAnimationViewStub::setAnimationFrame(int frame) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(frame));
  stubMethodEntered("setAnimationFrame",params);
}

void StatusIndicatorAnimationViewStub::setFirstAnimationFrame(int frame) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(frame));
  stubMethodEntered("setFirstAnimationFrame",params);
}

void StatusIndicatorAnimationViewStub::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QPainter * >(painter));
  params.append( new Parameter<const QStyleOptionGraphicsItem * >(option));
  stubMethodEntered("drawContents",params);
}

void StatusIndicatorAnimationViewStub::setupModel() {
  stubMethodEntered("setupModel");
}

void StatusIndicatorAnimationViewStub::applyStyle() {
  stubMethodEntered("applyStyle");
}

void StatusIndicatorAnimationViewStub::setupAnimationTimeline() {
  stubMethodEntered("setupAnimationTimeline");
}

void StatusIndicatorAnimationViewStub::clearImageList() {
  stubMethodEntered("clearImageList");
}

void StatusIndicatorAnimationViewStub::setupImageList(const QString &iconIDs) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(iconIDs));
  stubMethodEntered("setupImageList",params);
}



// 3. CREATE A STUB INSTANCE
StatusIndicatorAnimationViewStub gDefaultStatusIndicatorAnimationViewStub;
StatusIndicatorAnimationViewStub* gStatusIndicatorAnimationViewStub = &gDefaultStatusIndicatorAnimationViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorAnimationView::StatusIndicatorAnimationView(StatusIndicator *controller) : MWidgetView(controller) {
  gStatusIndicatorAnimationViewStub->StatusIndicatorAnimationViewConstructor(controller);
}

StatusIndicatorAnimationView::~StatusIndicatorAnimationView() {
  gStatusIndicatorAnimationViewStub->StatusIndicatorAnimationViewDestructor();
}

void StatusIndicatorAnimationView::updateData(const QList<const char *> &modifications) {
  gStatusIndicatorAnimationViewStub->updateData(modifications);
}

void StatusIndicatorAnimationView::startAnimation() {
  gStatusIndicatorAnimationViewStub->startAnimation();
}

void StatusIndicatorAnimationView::stopAnimation() {
  gStatusIndicatorAnimationViewStub->stopAnimation();
}

QSizeF StatusIndicatorAnimationView::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
    return gStatusIndicatorAnimationViewStub->sizeHint(which, constraint);
}

void StatusIndicatorAnimationView::setAnimationFrame(int frame) {
  gStatusIndicatorAnimationViewStub->setAnimationFrame(frame);
}

void StatusIndicatorAnimationView::setFirstAnimationFrame(int frame) {
  gStatusIndicatorAnimationViewStub->setFirstAnimationFrame(frame);
}

void StatusIndicatorAnimationView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const {
  gStatusIndicatorAnimationViewStub->drawContents(painter, option);
}

void StatusIndicatorAnimationView::setupModel() {
  gStatusIndicatorAnimationViewStub->setupModel();
}

void StatusIndicatorAnimationView::applyStyle() {
  gStatusIndicatorAnimationViewStub->applyStyle();
}

void StatusIndicatorAnimationView::setupAnimationTimeline() {
  gStatusIndicatorAnimationViewStub->setupAnimationTimeline();
}

void StatusIndicatorAnimationView::clearImageList() {
  gStatusIndicatorAnimationViewStub->clearImageList();
}

void StatusIndicatorAnimationView::setupImageList(const QString &iconIDs) {
  gStatusIndicatorAnimationViewStub->setupImageList(iconIDs);
}


#endif
