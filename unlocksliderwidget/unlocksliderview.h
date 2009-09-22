#ifndef UNLOCKSLIDERVIEW_H
#define UNLOCKSLIDERVIEW_H

#include <QTimer>

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

    virtual void styleChanged();

    virtual void drawBackground (QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawContents (QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void timeStep();

private:
    bool handleHit(const QPointF& pos);
    void setHandleModelPos(const QPointF& center);
    void setHandleScreenPos(const qreal& percent);

    bool grabHandle(const QPointF& pos);
    void moveHandle(const QPointF& pos);
    void releaseHandle();

    QRectF handleRect() const;
    void recalcRects();

    qreal clamp(qreal val, qreal min, qreal max) const;

private:
    QRectF sliderRect;
    QRectF iconRect;
    QPointF handlePos;
    QTimer timer;
    qreal resetVelocity;
    int blinkCount;
    int blinkTime;
    int backgroundTileCount;
};

#endif // UNLOCKSLIDERVIEW_H
