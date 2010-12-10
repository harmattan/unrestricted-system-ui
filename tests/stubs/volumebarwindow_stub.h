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
  virtual void volumeBarChanged(int val);
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

void VolumeBarWindowStub::volumeBarChanged(int val) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(val));
  stubMethodEntered("volumeBarChanged",params);
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

void VolumeBarWindow::volumeBarChanged(int val) {
  gVolumeBarWindowStub->volumeBarChanged(val);
}


#endif
