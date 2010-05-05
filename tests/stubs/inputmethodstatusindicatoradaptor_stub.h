#ifndef INPUTMETHODSTATUSINDICATORADAPTOR_STUB
#define INPUTMETHODSTATUSINDICATORADAPTOR_STUB

#include "inputmethodstatusindicatoradaptor.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class InputMethodStatusIndicatorAdaptorStub : public StubBase {
  public:
  virtual void InputMethodStatusIndicatorAdaptorConstructor(InputMethodStatusIndicator *parent);
  virtual void InputMethodStatusIndicatorAdaptorDestructor();
  virtual void setIconID(const QString &iconID);
}; 

// 2. IMPLEMENT STUB
void InputMethodStatusIndicatorAdaptorStub::InputMethodStatusIndicatorAdaptorConstructor(InputMethodStatusIndicator *parent) {
  Q_UNUSED(parent);

}
void InputMethodStatusIndicatorAdaptorStub::InputMethodStatusIndicatorAdaptorDestructor() {

}
void InputMethodStatusIndicatorAdaptorStub::setIconID(const QString &iconID) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(iconID));
  stubMethodEntered("setIconID",params);
}



// 3. CREATE A STUB INSTANCE
InputMethodStatusIndicatorAdaptorStub gDefaultInputMethodStatusIndicatorAdaptorStub;
InputMethodStatusIndicatorAdaptorStub* gInputMethodStatusIndicatorAdaptorStub = &gDefaultInputMethodStatusIndicatorAdaptorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
InputMethodStatusIndicatorAdaptor::InputMethodStatusIndicatorAdaptor(InputMethodStatusIndicator *parent) : QDBusAbstractAdaptor(parent) {
  gInputMethodStatusIndicatorAdaptorStub->InputMethodStatusIndicatorAdaptorConstructor(parent);
}

InputMethodStatusIndicatorAdaptor::~InputMethodStatusIndicatorAdaptor() {
  gInputMethodStatusIndicatorAdaptorStub->InputMethodStatusIndicatorAdaptorDestructor();
}

void InputMethodStatusIndicatorAdaptor::setIconID(const QString &iconID) {
  gInputMethodStatusIndicatorAdaptorStub->setIconID(iconID);
}


#endif
