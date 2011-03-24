#ifndef USBUI_STUB
#define USBUI_STUB

#include "usbui.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class UsbUiStub : public StubBase {
  public:
  virtual void UsbUiConstructor(QObject *parent);
  virtual void UsbUiDestructor();
  virtual void applyCurrentUSBMode();
  virtual void applyUSBMode(MeeGo::QmUSBMode::Mode mode);
  virtual void setRequestedUSBMode();
  virtual void setOviSuiteMode();
  virtual void setMassStorageMode();
  virtual void showDialog();
  virtual void showError(const QString &error);
  virtual void showNotification(int id);
  virtual void hideNotification();
  virtual void hideDialog(bool accept);
}; 

// 2. IMPLEMENT STUB
void UsbUiStub::UsbUiConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void UsbUiStub::UsbUiDestructor() {

}
void UsbUiStub::applyCurrentUSBMode() {
  stubMethodEntered("applyCurrentUSBMode");
}

void UsbUiStub::applyUSBMode(MeeGo::QmUSBMode::Mode mode) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmUSBMode::Mode >(mode));
  stubMethodEntered("applyUSBMode",params);
}

void UsbUiStub::setRequestedUSBMode() {
  stubMethodEntered("setRequestedUSBMode");
}

void UsbUiStub::setOviSuiteMode() {
  stubMethodEntered("setOviSuiteMode");
}

void UsbUiStub::setMassStorageMode() {
  stubMethodEntered("setMassStorageMode");
}

void UsbUiStub::showDialog() {
  stubMethodEntered("showDialog");
}

void UsbUiStub::showError(const QString &error) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(error));
  stubMethodEntered("showError",params);
}

void UsbUiStub::showNotification(int id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(id));
  stubMethodEntered("showNotification",params);
}

void UsbUiStub::hideNotification() {
  stubMethodEntered("hideNotification");
}

void UsbUiStub::hideDialog(bool accept) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(accept));
  stubMethodEntered("hideDialog",params);
}



// 3. CREATE A STUB INSTANCE
UsbUiStub gDefaultUsbUiStub;
UsbUiStub* gUsbUiStub = &gDefaultUsbUiStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
UsbUi::UsbUi(QObject *parent) {
  gUsbUiStub->UsbUiConstructor(parent);
}

UsbUi::~UsbUi() {
  gUsbUiStub->UsbUiDestructor();
}

void UsbUi::applyCurrentUSBMode() {
  gUsbUiStub->applyCurrentUSBMode();
}

void UsbUi::applyUSBMode(MeeGo::QmUSBMode::Mode mode) {
  gUsbUiStub->applyUSBMode(mode);
}

void UsbUi::setRequestedUSBMode() {
  gUsbUiStub->setRequestedUSBMode();
}

void UsbUi::setOviSuiteMode() {
  gUsbUiStub->setOviSuiteMode();
}

void UsbUi::setMassStorageMode() {
  gUsbUiStub->setMassStorageMode();
}

void UsbUi::showDialog() {
  gUsbUiStub->showDialog();
}

void UsbUi::showError(const QString &error) {
  gUsbUiStub->showError(error);
}

void UsbUi::showNotification(int id) {
  gUsbUiStub->showNotification(id);
}

void UsbUi::hideNotification() {
  gUsbUiStub->hideNotification();
}

void UsbUi::hideDialog(bool accept) {
  gUsbUiStub->hideDialog(accept);
}


#endif
