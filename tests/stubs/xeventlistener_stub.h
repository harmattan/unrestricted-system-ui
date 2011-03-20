#ifndef XEVENTLISTENER_STUB
#define XEVENTLISTENER_STUB

#include "xeventlistener.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class XEventListenerStub : public StubBase {
  public:
  virtual void XEventListenerConstructor();
  virtual void XEventListenerDestructor();
  virtual void registerEventFilter(XEventListenerFilterInterface *filter, long eventMask);
  virtual void unregisterEventFilter(XEventListenerFilterInterface *filter);
  virtual bool xEventFilter(void *message);
  virtual void setupListener();
  virtual void setupEventMasks();
}; 

// 2. IMPLEMENT STUB
void XEventListenerStub::XEventListenerConstructor() {

}
void XEventListenerStub::XEventListenerDestructor() {

}

void XEventListenerStub::registerEventFilter(XEventListenerFilterInterface *filter, long eventMask) {
  QList<ParameterBase*> params;
  params.append( new Parameter<XEventListenerFilterInterface * >(filter));
  params.append( new Parameter<long>(eventMask));
  stubMethodEntered("registerEventFilter",params);
}

void XEventListenerStub::unregisterEventFilter(XEventListenerFilterInterface *filter) {
  QList<ParameterBase*> params;
  params.append( new Parameter<XEventListenerFilterInterface * >(filter));
  stubMethodEntered("unregisterEventFilter",params);
}

bool XEventListenerStub::xEventFilter(void *message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<void * >(message));
  stubMethodEntered("xEventFilter",params);
  return stubReturnValue<bool>("xEventFilter");
}

void XEventListenerStub::setupListener() {
  stubMethodEntered("setupListener");
}

void XEventListenerStub::setupEventMasks() {
  stubMethodEntered("setupEventMasks");
}


// 3. CREATE A STUB INSTANCE
XEventListenerStub gDefaultXEventListenerStub;
XEventListenerStub* gXEventListenerStub = &gDefaultXEventListenerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
XEventListener::XEventListener() {
  gXEventListenerStub->XEventListenerConstructor();
}

XEventListener::~XEventListener() {
  gXEventListenerStub->XEventListenerDestructor();
}

void XEventListener::registerEventFilter(XEventListenerFilterInterface *filter, long eventMask) {
  gXEventListenerStub->registerEventFilter(filter, eventMask);
}

void XEventListener::unregisterEventFilter(XEventListenerFilterInterface *filter) {
  gXEventListenerStub->unregisterEventFilter(filter);
}

bool XEventListener::xEventFilter(void *message) {
  return gXEventListenerStub->xEventFilter(message);
}

void XEventListener::setupListener() {
  gXEventListenerStub->setupListener();
}

void XEventListener::setupEventMasks() {
  gXEventListenerStub->setupEventMasks();
}

#endif
