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
  virtual void updateVolume(int val, int max);
  virtual void updateContents();
  virtual void finishAnimations();
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void applyStyle();
  virtual void calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event);
  virtual void constructUi();
}; 

// 2. IMPLEMENT STUB
void VolumeBarStub::VolumeBarConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void VolumeBarStub::VolumeBarDestructor() {

}
void VolumeBarStub::updateVolume(int val, int max) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(val));
  params.append( new Parameter<int >(max));
  stubMethodEntered("updateVolume",params);
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

void VolumeBarStub::calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("calculateNewVolumeForEvent",params);
}

void VolumeBarStub::constructUi() {
  stubMethodEntered("constructUi");
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

void VolumeBar::updateVolume(int val, int max) {
  gVolumeBarStub->updateVolume(val, max);
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

void VolumeBar::calculateNewVolumeForEvent(QGraphicsSceneMouseEvent *event) {
  gVolumeBarStub->calculateNewVolumeForEvent(event);
}

void VolumeBar::constructUi() {
  gVolumeBarStub->constructUi();
}


#endif
