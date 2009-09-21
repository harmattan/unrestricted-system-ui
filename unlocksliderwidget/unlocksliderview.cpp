#include "unlocksliderview.h"
#include "unlockslider.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <DuiTheme>

UnlockSliderView::UnlockSliderView(UnlockSlider *controller) :
    DuiWidgetView(controller),
    sliderRect(0, 0, 0, 0),
    iconRect(0, 0, 0, 0),
    handlePos(0, 0)
{
}

UnlockSliderView::~UnlockSliderView()
{
}

void UnlockSliderView::resizeEvent(QGraphicsSceneResizeEvent * event)
{
    DuiWidgetView::resizeEvent(event);

    recalcRects();
    update();
}

void UnlockSliderView::modelModified(const QList<const char*>& modifications)
{
    DuiWidgetView::modelModified(modifications);

    const char* member;
    foreach(member, modifications) {
        if(member == UnlockSliderModel::HandlePressed ) {
            if (!model()->handlePressed()) {
                releaseHandle();
            }
        }
        else if( member == UnlockSliderModel::Position ) {
            // TODO
        }
        else if(member == UnlockSliderModel::IconVisible) {
            recalcRects();
        }
    }
    update();
}

void UnlockSliderView::modelChanged()
{
    DuiWidgetView::modelChanged();

    recalcRects();
}

void UnlockSliderView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    const qreal tilewidth = style()->backgroundTileSize().width();
    QRectF tile(sliderRect.topLeft(), style()->backgroundTileSize());

    while (tilewidth > 0 && tile.left() + tilewidth <= sliderRect.right()+1) {
        style()->backgroundTileImage()->draw(tile.toRect(), painter);
        tile.adjust(tilewidth, 0, tilewidth, 0);
    }

    // TODO: last tile piece is not drawn

    if (model()->iconVisible()) {
        style()->iconImage()->draw(iconRect.toRect(), painter);
    }
}

void UnlockSliderView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    // TODO: is this check needed? if there's no image, we're pretty much screwed anyways :)
    const DuiScalableImage* handleImage(style()->handleImage());
    if (handleImage) {
        handleImage->draw(handlePos.toPoint(), style()->handleSize(), painter);
    }
    else {
        painter->fillRect(QRect(handlePos.toPoint(), style()->handleSize()), QBrush(Qt::blue));
    }
}

void UnlockSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!grabHandle(event->pos())) {
//        blink();
    }
}

void UnlockSliderView::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    moveHandle(event->pos());
}

void UnlockSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    releaseHandle();
}

bool UnlockSliderView::handleHit(const QPointF& pos)
{
    if (pos.y() < handlePos.y() ||
        pos.y() >= (handlePos.y() + style()->handleSize().height())) {
        return false;
    }

    qreal help = model()->handlePressed() ? style()->handleSize().width() : 0;

    if (pos.x() < (handlePos.x() - help) ||
        pos.x() >= (handlePos.x() + style()->handleSize().width() + help)) {
        return false;
    }

    return true;
}

void UnlockSliderView::setHandlePos(const QPointF& center)
{
    const qreal halfwidth = style()->handleSize().width() / 2;
    const qreal xmin = sliderRect.left() + halfwidth;
    const qreal xmax = sliderRect.right() - halfwidth;

    if (center.x() >= xmax) {
        //emit unlocked();
        qDebug() << "unlocked";
    }

    qreal x = center.x() < xmin ? xmin : (center.x() >= xmax ? xmax : center.x());

    handlePos.setX(x - halfwidth);
    handlePos.setY(sliderRect.top());

    update();
}

bool UnlockSliderView::grabHandle(const QPointF& pos)
{
    if (handleHit(pos)) {
        model()->setHandlePressed(true);
    }
    return model()->handlePressed();
}

void UnlockSliderView::moveHandle(const QPointF& pos)
{
    if (handleHit(pos)) {
        setHandlePos(pos);
//        resetVelocity = 0;
    }
    else {
        releaseHandle();
    }
}

void UnlockSliderView::releaseHandle()
{
    model()->setHandlePressed(false);
/*    if (!timer.isActive()) {
        timer.start();
    }*/
}

// Generate a rect for current handle position
QRectF UnlockSliderView::handleRect() const
{
    return QRectF(handlePos, style()->handleSize());
}

// Recalculate the slider and icon area
void UnlockSliderView::recalcRects()
{
    // slider + icon fills the whole widget width
    qreal sliderwidth = size().width() - ( model()->iconVisible() ? style()->iconSize().width() : 0);
    // centered vertically
    qreal slidertop = (size().height() / 2) - (style()->handleSize().height() / 2);

    sliderRect.setWidth(sliderwidth);
    sliderRect.setHeight(style()->handleSize().height());
    sliderRect.setTopLeft(QPoint(0, slidertop));

    iconRect.setTopLeft(QPoint(sliderwidth, (size().height() / 2) - (style()->iconSize().height() / 2)));
    iconRect.setSize(style()->iconSize());

    //setHandlePos(QPoint(handlePos.x() + (handleSize.width() >> 1), sliderArea.top()));
    releaseHandle();
}
