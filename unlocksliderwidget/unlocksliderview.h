#ifndef UNLOCKSLIDERVIEW_H
#define UNLOCKSLIDERVIEW_H

#include <DuiWidgetView>

#include "unlockslidermodel.h"
#include "unlocksliderstyle.h"

class UnlockSlider;

class UnlockSliderView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(UnlockSliderModel, UnlockSliderStyle)

public:
    UnlockSliderView(UnlockSlider* controller);
    virtual ~UnlockSliderView();
    virtual void resizeEvent(QGraphicsSceneResizeEvent * event);

protected:
    virtual void modelModified(const QList<const char*>& modifications);
    virtual void modelChanged();

    virtual void drawBackground (QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawContents (QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool handleHit(const QPointF& pos);
    void setHandlePos(const QPointF& center);

    bool grabHandle(const QPointF& pos);
    void moveHandle(const QPointF& pos);
    void releaseHandle();

    inline QRectF handleRect() const;
    void recalcRects();

private:
    QRectF sliderRect;
    QRectF iconRect;
    QPointF handlePos;
};

#endif // UNLOCKSLIDERVIEW_H
