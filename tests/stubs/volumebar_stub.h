#ifndef VOLUMEBAR_STUB
#define VOLUMEBAR_STUB

#include "volumebar.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class VolumeBarStub : public StubBase {
  public:
  virtual void VolumeBarConstructor(QGraphicsItem *parent);
  virtual void VolumeBarDestructor();
  virtual void setTargetPercentage(qreal percentage);
  virtual void updateContents();
  virtual void finishAnimations();
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void applyStyle();
  virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option);
  virtual void calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event);
  virtual qreal currentPercentage();
  virtual void setCurrentPercentage(qreal percentage);
}; 

// 2. IMPLEMENT STUB
void VolumeBarStub::VolumeBarConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void VolumeBarStub::VolumeBarDestructor() {

}
void VolumeBarStub::setTargetPercentage(qreal percentage) {
  QList<ParameterBase*> params;
  params.append( new Parameter<qreal >(percentage));
  stubMethodEntered("setTargetPercentage",params);
}

void VolumeBarStub::updateContents() {
  stubMethodEntered("updateContents");
}

void VolumeBarStub::finishAnimations() {
  stubMethodEntered("finishAnimations");
}

void VolumeBarStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void VolumeBarStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseMoveEvent",params);
}

void VolumeBarStub::applyStyle() {
  stubMethodEntered("applyStyle");
}

void VolumeBarStub::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<QPainter * >(painter));
    params.append( new Parameter<const QStyleOptionGraphicsItem * >(option));
    stubMethodEntered("drawBackground",params);
}

void VolumeBarStub::calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("calculateNewVolumeForEvent",params);
}

qreal VolumeBarStub::currentPercentage() {
  stubMethodEntered("currentPercentage");
  return stubReturnValue<qreal>("currentPercentage");
}

void VolumeBarStub::setCurrentPercentage(qreal percentage) {
  QList<ParameterBase*> params;
  params.append( new Parameter<qreal >(percentage));
  stubMethodEntered("setCurrentPercentage",params);
}



// 3. CREATE A STUB INSTANCE
VolumeBarStub gDefaultVolumeBarStub;
VolumeBarStub* gVolumeBarStub = &gDefaultVolumeBarStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
VolumeBar::VolumeBar(QGraphicsItem *parent) {
  gVolumeBarStub->VolumeBarConstructor(parent);
}

VolumeBar::~VolumeBar() {
  gVolumeBarStub->VolumeBarDestructor();
}

void VolumeBar::setTargetPercentage(qreal percentage) {
  gVolumeBarStub->setTargetPercentage(percentage);
}

void VolumeBar::updateContents() {
  gVolumeBarStub->updateContents();
}

void VolumeBar::finishAnimations() {
  gVolumeBarStub->finishAnimations();
}

void VolumeBar::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gVolumeBarStub->mousePressEvent(event);
}

void VolumeBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  gVolumeBarStub->mouseMoveEvent(event);
}

void VolumeBar::applyStyle() {
  gVolumeBarStub->applyStyle();
}

void VolumeBar::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const {
  gVolumeBarStub->drawBackground(painter, option);
}

void VolumeBar::calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event) {
  gVolumeBarStub->calculateNewVolumeForEvent(event);
}

qreal VolumeBar::currentPercentage() const {
  return gVolumeBarStub->currentPercentage();
}

void VolumeBar::setCurrentPercentage(qreal percentage) {
  gVolumeBarStub->setCurrentPercentage(percentage);
}


#endif
