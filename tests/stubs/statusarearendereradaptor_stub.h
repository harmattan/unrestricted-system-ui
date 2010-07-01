#ifndef STATUSAREARENDERERADAPTOR_STUB
#define STATUSAREARENDERERADAPTOR_STUB

#include "statusarearendereradaptor.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusAreaRendererAdaptorStub : public StubBase {
  public:
  virtual void StatusAreaRendererAdaptorConstructor(StatusAreaRenderer *parent);
  virtual void StatusAreaRendererAdaptorDestructor();
  virtual uint sharedPixmapHandle();
};

// 2. IMPLEMENT STUB
void StatusAreaRendererAdaptorStub::StatusAreaRendererAdaptorConstructor(StatusAreaRenderer *parent) {
  Q_UNUSED(parent);

}
void StatusAreaRendererAdaptorStub::StatusAreaRendererAdaptorDestructor() {

}
uint StatusAreaRendererAdaptorStub::sharedPixmapHandle() {
  stubMethodEntered("sharedPixmapHandle");
  return stubReturnValue<uint>("sharedPixmapHandle");
}



// 3. CREATE A STUB INSTANCE
StatusAreaRendererAdaptorStub gDefaultStatusAreaRendererAdaptorStub;
StatusAreaRendererAdaptorStub* gStatusAreaRendererAdaptorStub = &gDefaultStatusAreaRendererAdaptorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusAreaRendererAdaptor::StatusAreaRendererAdaptor(StatusAreaRenderer *parent) : QDBusAbstractAdaptor(parent) {
  gStatusAreaRendererAdaptorStub->StatusAreaRendererAdaptorConstructor(parent);
}

StatusAreaRendererAdaptor::~StatusAreaRendererAdaptor() {
  gStatusAreaRendererAdaptorStub->StatusAreaRendererAdaptorDestructor();
}

uint StatusAreaRendererAdaptor::sharedPixmapHandle() {
  return gStatusAreaRendererAdaptorStub->sharedPixmapHandle();
}


#endif
