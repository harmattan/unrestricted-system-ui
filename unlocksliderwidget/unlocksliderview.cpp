#include "unlocksliderview.h"
#include "unlockslider.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <DuiTheme>

UnlockSliderView::UnlockSliderView(UnlockSlider *controller) :
    DuiWidgetView(controller)
{
    buttonSize = QSize(64, 32);
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

    sliderArea = QRectF(    QPointF(0, vCenter - buttonSize.height() / 2),
                            QPointF(size().width(), vCenter + buttonSize.height() / 2));
    setButtonPos(0);
/*
    qreal sliderLength = model()->orientation() == Qt::Vertical ? d->slideArea.height() : d->slideArea.width();
    d->tileCount = d->tileSize.height() == 0 ? 0 : sliderLength / d->tileSize.height();

    int range = model()->maximum() - model()->minimum();

    // the 'handle' is the the number of tiles currently selected
    if(range > 0 && model()->value() > model()->minimum()) {
        d->handlePos = (model()->value() - model()->minimum()) * d->tileCount / range + 1;
    }
    else {
        // value == minimum --> handlePos = 0
        d->handlePos = 0;
    }

    if (d->handlePos > d->tileCount) { // true, if value == maximum
        d->handlePos = d->tileCount;
    }
*/
    update();
}

void UnlockSliderView::drawContents (QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    painter->drawLine(0, 0, size().width(), size().height());
    painter->drawLine(0, size().height(), size().width(), 0);

    const QPixmap* buttonImage = DuiTheme::pixmap("sadfasdf", buttonSize);
    painter->drawPixmap(buttonPos, *buttonImage);
    //painter->drawRect(buttonPos.x(), buttonPos.y(), buttonSize.width(), buttonSize.height());
}

void UnlockSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isButtonHit(event->pos())) {
        setButtonPos(event->pos().x());
    }
}

void UnlockSliderView::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (isButtonHit(event->pos())) {
        setButtonPos(event->pos().x());
    } else {
        resetButtonPos();
    }
}

void UnlockSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    resetButtonPos();
}

bool UnlockSliderView::isButtonHit(QPointF pos)
{
    if (pos.x() < buttonPos.x() ||
        pos.x() > (buttonPos.x() + buttonSize.width()) ||
        pos.y() < sliderArea.top() ||
        pos.y() > sliderArea.bottom() ) {
        return false;
    }

    return true;
}

void UnlockSliderView::setButtonPos(qreal centerx)
{
    qreal x = centerx - (buttonSize.width() / 2);

    if (x < sliderArea.left()) {
        x = sliderArea.left();
    }
    else if (x >= sliderArea.right() - buttonSize.width()) {
        x = sliderArea.right() - buttonSize.width();
    }

    buttonPos = QPointF(x, sliderArea.top());
    update();
}

void UnlockSliderView::resetButtonPos()
{
//    buttonPos = QPointF(sliderArea.topLeft());
    setButtonPos(0);
    // release model state
}
