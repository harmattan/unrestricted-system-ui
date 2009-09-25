#include "unlocksliderview.h"
#include "unlockslider.h"

#include <DuiTheme>

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <math.h>

// update 25 times per sec
const int UPDATEFREQ(1000/25);
// reset speed acceleration
const qreal RESETACCEL(1.0f/25.0f);

UnlockSliderView::UnlockSliderView(UnlockSlider *controller) :
    DuiWidgetView(controller),
    sliderRect(1, 1, 1, 1),
    iconRect(1, 1, 1, 1),
    handlePos(0, 0),
    resetVelocity(0),
    blinkCount(0),
    blinkTime(0),
    backgroundTileCount(0)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeStep()));
    timer.setInterval(UPDATEFREQ);
}

UnlockSliderView::~UnlockSliderView()
{
    timer.stop();
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
    foreach (member, modifications) {
        if (member == UnlockSliderModel::HandlePressed ) {
            // this is a bit unclear... pressed state should only change according to touch events
            if (!model()->handlePressed()) {
                releaseHandle();
            }
        }
        else if (member == UnlockSliderModel::Position ) {
            setHandleScreenPos(model()->position());
        }
        else if (member == UnlockSliderModel::IconVisible) {
            recalcRects();
        }
        else if (member == UnlockSliderModel::Magnetic) {
            // TODO
        }
        else if (member == UnlockSliderModel::Blinking) {
            if (!model()->blinking()) {
                blinkCount = 0;
                blinkTime = 0;
            }
        }
    }

    update();
}

void UnlockSliderView::modelChanged()
{
    DuiWidgetView::modelChanged();

    recalcRects();
}

void UnlockSliderView::styleChanged()
{
    DuiWidgetView::styleChanged();

    recalcRects();
}

void UnlockSliderView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    const qreal tilewidth = style()->backgroundTileSize().width();
    QRectF tile(sliderRect.topLeft(), style()->backgroundTileSize());

    for (int i = 0; i < backgroundTileCount; i++) {
        style()->backgroundTileImage()->draw(tile.toRect(), painter);
        tile.adjust(tilewidth, 0, tilewidth, 0);
    }

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

        if (model()->blinking() && !(blinkCount & 1)) {
            //style()->handleHilightedImage()->draw(handlePos.toPoint(), style()->handleSize(), painter);
            painter->fillRect(QRect(handlePos.toPoint(), style()->handleSize()), QBrush(Qt::white));
        }
    }
    else {
        painter->fillRect(QRect(handlePos.toPoint(), style()->handleSize()), QBrush(Qt::red));
    }
}

void UnlockSliderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!grabHandle(event->pos())) {
        model()->setBlinking(true);
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

void UnlockSliderView::timeStep()
{
    bool stoptimer = true;

    if (model()->blinking()) {
        blinkTime += UPDATEFREQ;

        int tmp = style()->blinkTime() / style()->blinkCount();
        if (blinkTime > tmp) {
            blinkTime -= tmp;
            blinkCount++;
            update();
        }

        if (blinkCount < 2 * style()->blinkCount()) {
            stoptimer = false;
        }
        else {
            model()->setBlinking(false);
        }
    }

    if (!model()->handlePressed() && model()->magnetic() && model()->position() > 0) {
        qreal tmp = model()->position() - resetVelocity;
        resetVelocity += RESETACCEL;
        model()->setPosition(tmp);
        stoptimer = false;
    }

    if (stoptimer) {
        timer.stop();
    }
}

// Checks if pos is inside the handle area
bool UnlockSliderView::handleHit(const QPointF& pos)
{
    // vertical check
    if (pos.y() < handlePos.y() ||
        pos.y() >= (handlePos.y() + style()->handleSize().height())) {
        return false;
    }

    // check horizontally if hit is within handle.
    if (model()->handlePressed())
        // slight approximation =)
        return true;

    return handlePos.x() < pos.x()
            && pos.x() < (handlePos.x() + style()->handleSize().width());
}

// Sets the handle position to model from widget coordinates
void UnlockSliderView::setHandleModelPos(const QPointF& center)
{
    const qreal halfwidth = style()->handleSize().width() * 0.5f;
    const qreal xmin = sliderRect.left() + halfwidth;
    const qreal xmax = sliderRect.right() - halfwidth;

    // Prevent possible division by zero
    if (xmax-xmin != 0) {
        // Scale x to 0..1 range and set the value to model which causes modelModified to be called
        model()->setPosition((clamp(center.x(), xmin, xmax) - xmin) / (xmax - xmin));
        resetVelocity = 0;
    }
    else {
        model()->setPosition(0);
    }
}

// Sets the handle position in (0..1) range
void UnlockSliderView::setHandleScreenPos(const qreal& percent)
{
    const qreal xmin = sliderRect.left();
    const qreal xmax = sliderRect.right() - style()->handleSize().width();

    qreal x = clamp(percent * (xmax - xmin) + xmin, xmin, xmax);

    handlePos.setX(x);
    handlePos.setY(sliderRect.top());
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
        setHandleModelPos(pos);
        resetVelocity = 0;
    }
    else {
        releaseHandle();
    }
}

void UnlockSliderView::releaseHandle()
{
    model()->setHandlePressed(false);
    resetVelocity = 0;
    if (!timer.isActive()) {
        timer.start();
    }
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
    qreal sliderwidth = size().width() - (model()->iconVisible() ? style()->iconSize().width() : 0);
    // centered vertically
    qreal slidertop = (size().height() / 2) - (style()->handleSize().height() / 2);

    qreal leftover = fmod(sliderwidth, style()->backgroundTileSize().width());

    sliderRect.setWidth(sliderwidth - leftover);
    sliderRect.setHeight(style()->handleSize().height());
    sliderRect.setTopLeft(QPoint(leftover, slidertop));

    backgroundTileCount = sliderRect.width() / style()->backgroundTileSize().width();

    iconRect.setTopLeft(QPoint(sliderRect.right(), (size().height() / 2) - (style()->iconSize().height() / 2)));
    iconRect.setSize(style()->iconSize());

    setHandleScreenPos(model()->position());

    releaseHandle();
}

inline qreal UnlockSliderView::clamp(qreal val, qreal min, qreal max) const
{
    return val < min ? min : (val > max ? max : val);
}

