#ifndef VOLUMEBARWINDOW_STUB
#define VOLUMEBARWINDOW_STUB

#include "volumebarwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class VolumeBarWindowStub : public StubBase {
  public:
  virtual void VolumeBarWindowConstructor(VolumeBarLogic *logic, QWidget *parent);
  virtual void VolumeBarWindowDestructor();
  virtual void updateVolume();
  virtual void setVolume(qreal percentage);
}; 

// 2. IMPLEMENT STUB
void VolumeBarWindowStub::VolumeBarWindowConstructor(VolumeBarLogic *logic, QWidget *parent) {
  Q_UNUSED(logic);
  Q_UNUSED(parent);

}
void VolumeBarWindowStub::VolumeBarWindowDestructor() {

}
void VolumeBarWindowStub::updateVolume() {
  stubMethodEntered("updateVolume");
}

void VolumeBarWindowStub::setVolume(qreal percentage) {
  QList<ParameterBase*> params;
  params.append( new Parameter<qreal >(percentage));
  stubMethodEntered("setVolume",params);
}



// 3. CREATE A STUB INSTANCE
VolumeBarWindowStub gDefaultVolumeBarWindowStub;
VolumeBarWindowStub* gVolumeBarWindowStub = &gDefaultVolumeBarWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
VolumeBarWindow::VolumeBarWindow(VolumeBarLogic *logic, QWidget *parent) {
  gVolumeBarWindowStub->VolumeBarWindowConstructor(logic, parent);
}

VolumeBarWindow::~VolumeBarWindow() {
  gVolumeBarWindowStub->VolumeBarWindowDestructor();
}

void VolumeBarWindow::updateVolume() {
  gVolumeBarWindowStub->updateVolume();
}

void VolumeBarWindow::setVolume(qreal percentage) {
  gVolumeBarWindowStub->setVolume(percentage);
}


#endif
