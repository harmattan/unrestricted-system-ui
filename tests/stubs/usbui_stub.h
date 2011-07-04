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
#ifdef HAVE_QMSYSTEM
  virtual void applyCurrentUSBMode();
  virtual void applyUSBMode(MeeGo::QmUSBMode::Mode mode);
  virtual void setRequestedUSBMode();
#endif
  virtual void setOviSuiteMode();
  virtual void setMassStorageMode();
  virtual void setSDKMode();
  virtual void showDialog();
  virtual void showError(const QString &error);
#ifdef HAVE_QMSYSTEM
  virtual void showNotification(MeeGo::QmUSBMode::Mode mode);
#endif
  virtual void hideDialog(bool accept);
  virtual void retranslateUi();
  virtual void updateSDKItemVisibility();
}; 

// 2. IMPLEMENT STUB
void UsbUiStub::UsbUiConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void UsbUiStub::UsbUiDestructor() {

}
#ifdef HAVE_QMSYSTEM
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
#endif

void UsbUiStub::setOviSuiteMode() {
  stubMethodEntered("setOviSuiteMode");
}

void UsbUiStub::setMassStorageMode() {
  stubMethodEntered("setMassStorageMode");
}

void UsbUiStub::setSDKMode() {
  stubMethodEntered("setSDKMode");
}

void UsbUiStub::showDialog() {
  stubMethodEntered("showDialog");
}

void UsbUiStub::showError(const QString &error) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(error));
  stubMethodEntered("showError",params);
}

#ifdef HAVE_QMSYSTEM
void UsbUiStub::showNotification(MeeGo::QmUSBMode::Mode mode) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmUSBMode::Mode >(mode));
  stubMethodEntered("showNotification",params);
}
#endif

void UsbUiStub::hideDialog(bool accept) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(accept));
  stubMethodEntered("hideDialog",params);
}

void UsbUiStub::retranslateUi() {
  stubMethodEntered("retranslateUi");
}

void UsbUiStub::updateSDKItemVisibility() {
  stubMethodEntered("updateSDKItemVisibility");
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

#ifdef HAVE_QMSYSTEM
void UsbUi::applyCurrentUSBMode() {
  gUsbUiStub->applyCurrentUSBMode();
}

void UsbUi::applyUSBMode(MeeGo::QmUSBMode::Mode mode) {
  gUsbUiStub->applyUSBMode(mode);
}

void UsbUi::setRequestedUSBMode() {
  gUsbUiStub->setRequestedUSBMode();
}
#endif

void UsbUi::setOviSuiteMode() {
  gUsbUiStub->setOviSuiteMode();
}

void UsbUi::setMassStorageMode() {
  gUsbUiStub->setMassStorageMode();
}

void UsbUi::setSDKMode() {
  gUsbUiStub->setSDKMode();
}

void UsbUi::showDialog() {
  gUsbUiStub->showDialog();
}

void UsbUi::showError(const QString &error) {
  gUsbUiStub->showError(error);
}

#ifdef HAVE_QMSYSTEM
void UsbUi::showNotification(MeeGo::QmUSBMode::Mode mode) {
  gUsbUiStub->showNotification(mode);
}
#endif

void UsbUi::hideDialog(bool accept) {
  gUsbUiStub->hideDialog(accept);
}

void UsbUi::retranslateUi() {
  gUsbUiStub->retranslateUi();
}

void UsbUi::updateSDKItemVisibility() {
  gUsbUiStub->updateSDKItemVisibility();
}

#endif
