#ifndef STATUSAREARENDERER_STUB
#define STATUSAREARENDERER_STUB

#include "statusarearenderer.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusAreaRendererStub : public StubBase {
  public:
  virtual void StatusAreaRendererConstructor(QWidget *parent);
  virtual void StatusAreaRendererDestructor();
  virtual void sceneChanged(const QList<QRectF> &region);
  virtual void setSceneRender(Maemo::QmDisplayState::DisplayState state);
   QGraphicsScene *scene ;
   StatusArea *statusArea_ ;
   QPixmap *statusAreaPixmap ;
  virtual bool createSharedPixmapHandle();
  virtual void setSizeFromStyle();
   uint statusAreaHeight ;
   uint statusAreaWidth ;
   Maemo::QmDisplayState *displayState ;
   bool renderScene ;
}; 

// 2. IMPLEMENT STUB
void StatusAreaRendererStub::StatusAreaRendererConstructor(QWidget *parent) {
  Q_UNUSED(parent);

}
void StatusAreaRendererStub::StatusAreaRendererDestructor() {

}
void StatusAreaRendererStub::sceneChanged(const QList<QRectF> &region) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<QRectF> & >(region));
  stubMethodEntered("sceneChanged",params);
}

void StatusAreaRendererStub::setSceneRender(Maemo::QmDisplayState::DisplayState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<Maemo::QmDisplayState::DisplayState >(state));
  stubMethodEntered("setSceneRender",params);
}

bool StatusAreaRendererStub::createSharedPixmapHandle() {
  stubMethodEntered("createSharedPixmapHandle");
  return stubReturnValue<bool>("createSharedPixmapHandle");
}

void StatusAreaRendererStub::setSizeFromStyle() {
  stubMethodEntered("setSizeFromStyle");
}



// 3. CREATE A STUB INSTANCE
StatusAreaRendererStub gDefaultStatusAreaRendererStub;
StatusAreaRendererStub* gStatusAreaRendererStub = &gDefaultStatusAreaRendererStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusAreaRenderer::StatusAreaRenderer(QWidget *parent) {
  gStatusAreaRendererStub->StatusAreaRendererConstructor(parent);
}

StatusAreaRenderer::~StatusAreaRenderer() {
  gStatusAreaRendererStub->StatusAreaRendererDestructor();
}

void StatusAreaRenderer::sceneChanged(const QList<QRectF> &region) {
  gStatusAreaRendererStub->sceneChanged(region);
}

void StatusAreaRenderer::setSceneRender(Maemo::QmDisplayState::DisplayState state) {
  gStatusAreaRendererStub->setSceneRender(state);
}

bool StatusAreaRenderer::createSharedPixmapHandle() {
  return gStatusAreaRendererStub->createSharedPixmapHandle();
}

void StatusAreaRenderer::setSizeFromStyle() {
  gStatusAreaRendererStub->setSizeFromStyle();
}


#endif
