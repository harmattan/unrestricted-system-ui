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
    sliderRect(-1, -1, -1, -1),
    iconRect(-1, -1, -1, -1),
    handlePos(-1, -1),
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
        else if (member == UnlockSliderModel::Orientation) {
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

void UnlockSliderView::styleChanged()
{
    DuiWidgetView::styleChanged();

    recalcRects();
}

void UnlockSliderView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    const qreal tilestep(style()->backgroundTileSize().width());
    const qreal angle(model()->orientation() == Qt::Horizontal ? 0.0f : 90.0f);
    QRectF tile(sliderRect.topLeft(), style()->backgroundTileSize());
    if (model()->orientation() == Qt::Vertical) {
        qreal tmp = tile.width();
        tile.setWidth(tile.height());
        tile.setHeight(tmp);
    }

    drawImage(NULL, sliderRect.toRect(), painter);

    const DuiScalableImage* image = style()->backgroundTileImage();

    for (int i = 0; i < backgroundTileCount; i++) {
        qDebug() << i;
        drawImage(image, tile.toRect(), painter, angle);

        if (model()->orientation() == Qt::Horizontal) {
            tile.adjust(tilestep, 0, tilestep, 0);
        }
        else { // vertical
            tile.adjust(0, tilestep, 0, tilestep);
        }
    }

    if (model()->iconVisible()) {
        drawImage(style()->iconImage(), iconRect.toRect(), painter);
    }
}

void UnlockSliderView::drawContents(QPainter* painter, const QStyleOptionGraphicsItem* option) const
{
    Q_UNUSED(option);

    const qreal angle = model()->orientation() == Qt::Horizontal ? 0.0f : 90.0f;

    drawImage(style()->handleImage(), handleRect().toRect(), painter, angle);

    if (model()->blinking() && !(blinkCount & 1)) {
        drawImage(style()->handleHilightedImage(), handleRect().toRect(), painter, angle);
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
    if (model()->orientation() == Qt::Horizontal &&
        (pos.y() < handlePos.y() || pos.y() >= (handlePos.y() + style()->handleSize().height()))) {
        return false;
    }
    else if (model()->orientation() == Qt::Vertical &&
            (pos.x() < handlePos.x() || pos.x() >= (handlePos.x() + style()->handleSize().height()))) {
        // handle height used because it's rotated in vertical mode                      --^
        return false;
    }

    // at this point, the slider area his hit, so..
    // ..if the handle is pressed, we are moving the slider so true is returned
    if (model()->handlePressed()) {
        // slight approximation =)
        return true;
    }

    if (model()->orientation() == Qt::Horizontal) {
        return pos.x() >= handlePos.x() && pos.x() < (handlePos.x() + style()->handleSize().width());
    }
    else { // vertical (handle width used because of rotation)
        return pos.y() >= handlePos.y() && pos.y() < (handlePos.y() + style()->handleSize().width());
    }
}

// Sets the handle position to model from widget coordinates
void UnlockSliderView::setHandleModelPos(const QPointF& center)
{
    const bool h = model()->orientation() == Qt::Horizontal;

    // center pos
    const qreal c = (h == true ? center.x() : center.y());

    // hhs = half of the handle size
    const qreal hhs = style()->handleSize().width() * 0.5f;
    const qreal min = (h == true ? sliderRect.left() : sliderRect.top()) + hhs;
    const qreal max = (h == true ? sliderRect.right() : sliderRect.bottom()) - hhs;

    // Prevent possible(?) division by zero
    if (max-min != 0) {
        // Scale c to 0..1 range and set the value to model which causes modelModified to be called
        model()->setPosition((clamp(c, min, max) - min) / (max - min));
        resetVelocity = 0;
    }
    else {
        model()->setPosition(0);
    }
}

// Sets the handle position in (0..1) range
void UnlockSliderView::setHandleScreenPos(const qreal& percent)
{
    if (model()->orientation() == Qt::Horizontal) {
        const qreal min = sliderRect.left();
        const qreal max = sliderRect.right() - style()->handleSize().width();

        handlePos.setX(clamp(percent * (max - min) + min, min, max));
        handlePos.setY(sliderRect.top());
    }
    else { // vertical
        const qreal min = sliderRect.top();
        // handle is rotated, use width here
        const qreal max = sliderRect.bottom() - style()->handleSize().width();

        handlePos.setX(sliderRect.left());
        handlePos.setY(clamp(percent * (max - min) + min, min, max));
    }
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

void UnlockSliderView::drawImage(const DuiScalableImage* image, const QRect& rect, QPainter* painter, const qreal angle) const
{
    if (image != NULL) {
        if (angle < -0.1f || angle > 0.1f) {
            painter->save();
            painter->translate(rect.center());
            painter->rotate(angle);
            QRect r(QPoint(-rect.width() / 2, -rect.height() / 2), rect.size());
            image->draw(r, painter);
            painter->restore();
        }
        else {
            image->draw(rect, painter);
        }
    }
    else {
        painter->fillRect(rect, QBrush(Qt::red));
    }
}

// Generate a rect for current handle position
QRectF UnlockSliderView::handleRect() const
{
    QRectF rect;

    if (model()->orientation() == Qt::Horizontal) {
        rect = QRectF(handlePos, style()->handleSize());
    }
    else { // vertical
        rect.setTopLeft(handlePos);
        rect.setWidth(style()->handleSize().height());
        rect.setHeight(style()->handleSize().width());
    }

    return rect;
}

// Recalculate the slider and icon area
void UnlockSliderView::recalcRects()
{
    if (model()->orientation() == Qt::Horizontal) {
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
    }
    else { // vertical
        // Note: only icon is kept in same orientaton towards the user,
        // handle and background images are rotated 90 degrees

        // slider + icon fills the whole widget height (icon not rotated, so using its height)
        qreal sliderheight = size().height() - (model()->iconVisible() ? style()->iconSize().height() : 0);
        // centered horizontally
        qreal sliderleft = (size().width() / 2) - (style()->handleSize().height() / 2);

        // background is rotated, so use width
        qreal leftover = fmod(sliderheight, style()->backgroundTileSize().width());

        sliderRect.setWidth(style()->handleSize().height()); // handle is rotated, so use height here
        sliderRect.setHeight(sliderheight - leftover);
        sliderRect.setTopLeft(QPoint(sliderleft, leftover));

        qDebug() << sliderRect;

        // background rotated -> use width
        backgroundTileCount = sliderRect.height() / style()->backgroundTileSize().width();

        qDebug() << backgroundTileCount << style()->backgroundTileSize().width();

        iconRect.setTopLeft(QPoint((size().width() / 2) - (style()->iconSize().width() / 2), sliderheight + leftover));
        iconRect.setSize(style()->iconSize());
    }

    setHandleScreenPos(model()->position());

    releaseHandle();
}

inline qreal UnlockSliderView::clamp(qreal val, qreal min, qreal max) const
{
    return val < min ? min : (val > max ? max : val);
}

