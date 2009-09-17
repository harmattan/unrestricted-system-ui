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

protected:
    virtual void modelModified(const QList<const char*>& modifications);
    virtual void modelChanged();

    virtual void setGeometry(const QRectF &rect);
//    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const;
//    virtual void drawBackground (QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawContents (QPainter *painter, const QStyleOptionGraphicsItem *option) const;
//    virtual void drawForeground (QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool isButtonHit(QPointF pos);
    void setButtonPos(qreal centerx);
    void resetButtonPos();

private:
    QRectF sliderArea;
    QSize buttonSize;
    QPointF buttonPos;

/*    int sliderPos;
    int sliderMax;
    int sliderMin;*/
};

#endif // UNLOCKSLIDERVIEW_H
