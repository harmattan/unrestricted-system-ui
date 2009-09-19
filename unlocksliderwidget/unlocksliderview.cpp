#include "unlocksliderview.h"
#include "unlockslider.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <DuiTheme>

UnlockSliderView::UnlockSliderView(UnlockSlider *controller) :
    DuiWidgetView(controller)
{
    handleSize = QSize(64, 32);
}

UnlockSliderView::~UnlockSliderView()
{
}

void UnlockSliderView::modelModified(const QList<const char*>& modifications)
{
    DuiWidgetView::modelModified(modifications);
}

void UnlockSliderView::modelChanged()
{
}

/*QSizeF UnlockSliderView::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);
}*/

void UnlockSliderView::setGeometry(const QRectF &rect)
{
    DuiWidgetView::setGeometry(rect);

    int vCenter = size().height() / 2;

    sliderArea = QRectF(    QPointF(0, vCenter - handleSize.height() / 2),
                            QPointF(size().width(), vCenter + handleSize.height() / 2));
    setHandlePos(0);

    update();
}

void UnlockSliderView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

//    const DuiScalableImage* handleImage(style()->handleImage());
}

void UnlockSliderView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    const DuiScalableImage* handleImage(style()->handleImage());

    if (handleImage) {
        handleImage->draw(handlePos.toPoint(), handleSize, painter);
    }
    else {
        QRect r(handlePos.x(), handlePos.y(), handleSize.width(), handleSize.height());
        painter->fillRect(r, QBrush(Qt::blue));
    }
}

void UnlockSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isHandleHit(event->pos())) {
        setHandlePos(event->pos().x());
    }
}

void UnlockSliderView::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (isHandleHit(event->pos(), true)) {
        setHandlePos(event->pos().x());
    } else {
        resetHandlePos();
    }
}

void UnlockSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    //resetHandlePos();
}

bool UnlockSliderView::isHandleHit(QPointF pos, bool isMoving)
{
    if (pos.y() < sliderArea.top() ||
        pos.y() > sliderArea.bottom() ) {
        return false;
    }

    int help = isMoving ? (handleSize.width() / 2) : 0;

    if (pos.x() < (handlePos.x() - help) ||
        pos.x() > (handlePos.x() + handleSize.width() + help)) {
        return false;
    }

    return true;
}

void UnlockSliderView::setHandlePos(qreal centerx)
{
    qreal x = centerx - (handleSize.width() / 2);

    if (x < sliderArea.left()) {
        x = sliderArea.left();
    }
    else if (x >= sliderArea.right() - handleSize.width()) {
        x = sliderArea.right() - handleSize.width();
    }

    handlePos = QPointF(x, sliderArea.top());
    update();
}

void UnlockSliderView::resetHandlePos()
{
//    handlePos = QPointF(sliderArea.topLeft());
    setHandlePos(0);
    // release model state
}
