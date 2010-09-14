#ifndef NGFADAPTER_STUB
#define NGFADAPTER_STUB

#include "ngfadapter.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NGFAdapterStub : public StubBase {
  public:
  virtual void NGFAdapterConstructor();
  virtual void NGFAdapterDestructor();
  virtual uint play(const QString &id);
  virtual void stop(uint eventId);
  virtual bool isValid();
};

// 2. IMPLEMENT STUB
void NGFAdapterStub::NGFAdapterConstructor() {

}
void NGFAdapterStub::NGFAdapterDestructor() {

}
uint NGFAdapterStub::play(const QString &id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(id));
  stubMethodEntered("play",params);
  return stubReturnValue<uint>("play");
}

void NGFAdapterStub::stop(uint eventId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(eventId));
  stubMethodEntered("stop",params);
}

bool NGFAdapterStub::isValid() {
  stubMethodEntered("isValid");
  return stubReturnValue<bool>("isValid");
}



// 3. CREATE A STUB INSTANCE
NGFAdapterStub gDefaultNGFAdapterStub;
NGFAdapterStub* gNGFAdapterStub = &gDefaultNGFAdapterStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NGFAdapter::NGFAdapter() {
  gNGFAdapterStub->NGFAdapterConstructor();
}

NGFAdapter::~NGFAdapter() {
  gNGFAdapterStub->NGFAdapterDestructor();
}

uint NGFAdapter::play(const QString &id) {
  return gNGFAdapterStub->play(id);
}

void NGFAdapter::stop(uint eventId) {
  gNGFAdapterStub->stop(eventId);
}

bool NGFAdapter::isValid() {
  return gNGFAdapterStub->isValid();
}


#endif
