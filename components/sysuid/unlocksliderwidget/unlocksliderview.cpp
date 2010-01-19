#include "unlocksliderview.h"
#include "unlockslider.h"

#include <DuiTheme>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <math.h>

// update 25 times per sec
const int UPDATEFREQ(1000 / 25);
// reset speed acceleration
const qreal RESETACCEL(1.0f / 25.0f);

UnlockSliderView::UnlockSliderView(UnlockSlider* controller) :
        DuiWidgetView(controller),
        sliderRect(-1, -1, -1, -1),
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

void UnlockSliderView::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    DuiWidgetView::resizeEvent(event);

    recalcRects();
    update();
}

void UnlockSliderView::updateData(const QList<const char*>& modifications)
{
    DuiWidgetView::updateData(modifications);

    const char* member;
    foreach(member, modifications) {
        if (member == UnlockSliderModel::HandlePressed) {
            // TODO: this is a bit unclear... pressed state should only change according to touch events
            if (!model()->handlePressed()) {
                releaseHandle();
            }
        } else if (member == UnlockSliderModel::Position) {
            setHandleScreenPos(model()->position());
        } else if (member == UnlockSliderModel::IconVisible) {
            recalcRects();
        } else if (member == UnlockSliderModel::Magnetic) {
            // TODO ...is there anything to do?
        } else if (member == UnlockSliderModel::Blinking) {
            if (!model()->blinking()) {
                blinkCount = 0;
                blinkTime = 0;
            }
        } else if (member == UnlockSliderModel::Orientation) {
            recalcRects();
        }
    }

    update();
}

void UnlockSliderView::setupModel()
{
    DuiWidgetView::setupModel();

    recalcRects();
}

void UnlockSliderView::applyStyle()
{
    DuiWidgetView::applyStyle();

    recalcRects();
}

void UnlockSliderView::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option) const
{
    Q_UNUSED(option);

    const bool reverse(qApp->isRightToLeft());
    const qreal thickness(style()->thickness());
    const qreal angle((model()->orientation() == Qt::Horizontal ? 0.0f : 90.0f) + (reverse ? 180.0f : 0.0f));

    QRectF tile(sliderRect.topLeft(), QSizeF(thickness, thickness));
    if (reverse) {
        if (model()->orientation() == Qt::Horizontal) {
            tile.adjust(sliderRect.width() - thickness, 0, sliderRect.width() - thickness, 0);
        } else { // vertical
            tile.adjust(0, sliderRect.height() - thickness, 0, sliderRect.height() - thickness);
        }
    }

    const DuiScalableImage* image = style()->backgroundTileImage();

    for (int i = 0; i < backgroundTileCount; i++) {
        drawImage(image, tile.toRect(), painter, angle);

        qreal a = reverse ? -thickness : thickness;

        if (model()->orientation() == Qt::Horizontal) {
            tile.adjust(a, 0, a, 0);
        } else { // vertical
            tile.adjust(0, a, 0, a);
        }
    }

    if (model()->iconVisible()) {
        drawImage(style()->iconImage(), tile.toRect(), painter);
    }
}

void UnlockSliderView::drawContents(QPainter* painter, const QStyleOptionGraphicsItem* option) const
{
    Q_UNUSED(option);
    const qreal angle((model()->orientation() == Qt::Horizontal ? 0.0f : 90.0f) + (qApp->isRightToLeft() ? 180.0f : 0.0f));

    drawImage(style()->handleImage(), handleRect().toRect(), painter, angle);

    if (model()->blinking() && !(blinkCount & 1)) {
        QRect tmp = handleRect().toRect();
        tmp.adjust(-2, -2, 2, 2);
        drawImage(style()->handleHilightedImage(), tmp, painter);
    }
}

void UnlockSliderView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!grabHandle(event->pos())) {
        model()->setBlinking(true);
    }
}

void UnlockSliderView::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    moveHandle(event->pos());
}

void UnlockSliderView::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
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
        } else {
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
bool UnlockSliderView::handleHit(const QPointF& pos) const
{
    const qreal thickness(style()->thickness());

    if (model()->orientation() == Qt::Horizontal &&
            (pos.y() < handlePos.y() || pos.y() >= (handlePos.y() + thickness))) {
        return false;
    } else if (model()->orientation() == Qt::Vertical &&
               (pos.x() < handlePos.x() || pos.x() >= (handlePos.x() + thickness))) {
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
        return pos.x() >= handlePos.x() && pos.x() < (handlePos.x() + thickness);
    } else { // vertical (handle width used because of rotation)
        return pos.y() >= handlePos.y() && pos.y() < (handlePos.y() + thickness);
    }
}

// Sets the handle position to model from widget coordinates
void UnlockSliderView::setHandleModelPos(const QPointF& center)
{
    const bool h(model()->orientation() == Qt::Horizontal);

    // center pos
    const qreal c((h == true ? center.x() : center.y()));

    // hhs = half of the handle size
    const qreal hhs(style()->thickness() * 0.5f);
    const qreal min((h == true ? sliderRect.left() : sliderRect.top()) + hhs);
    const qreal max((h == true ? sliderRect.right() : sliderRect.bottom()) - hhs);

    // Prevent possible(?) division by zero
    if (max - min != 0) {
        // Scale c to 0..1 range and set the value to model which causes updateData to be called
        qreal pos = (clamp(c, min, max) - min) / (max - min);
        if (qApp->isRightToLeft()) {
            pos = 1.0f - pos;
        }
        model()->setPosition(pos);

        //model()->setPosition(pos);
        resetVelocity = 0;
    } else {
        model()->setPosition(0);
    }
}

// Sets the handle position in (0..1) range
void UnlockSliderView::setHandleScreenPos(const qreal& percent)
{
    qreal pct = clamp(percent, 0.0f, 1.0f);
    if (qApp->isRightToLeft()) {
        pct = 1.0f - pct;
    }

    if (model()->orientation() == Qt::Horizontal) {
        const qreal min = sliderRect.left();
        const qreal max = sliderRect.right() - style()->thickness();

        handlePos.setX(pct * (max - min) + min);
        handlePos.setY(sliderRect.top());
    } else { // vertical
        const qreal min = sliderRect.top();
        const qreal max = sliderRect.bottom() - style()->thickness();

        handlePos.setX(sliderRect.left());
        handlePos.setY(pct * (max - min) + min);
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
    } else {
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
        } else {
            image->draw(rect, painter);
        }
    } else {
        painter->fillRect(rect, QBrush(Qt::red));
    }
}

// Generate a rect for current handle position
QRectF UnlockSliderView::handleRect() const
{
    const qreal t(style()->thickness());
    return QRectF(handlePos, QSizeF(t, t));
}

// Recalculate the slider and icon area
void UnlockSliderView::recalcRects()
{
    const qreal thickness(style()->thickness());
    const bool reverse(qApp->isRightToLeft());

    if (model()->orientation() == Qt::Horizontal) {
        // slider + icon fills the whole widget width
        qreal sliderwidth = size().width() - (model()->iconVisible() ? thickness : 0);
        // centered vertically
        qreal slidertop = (size().height() / 2) - (thickness / 2);
        qreal leftover = fmod(sliderwidth, thickness);

        if (!reverse) {
            sliderRect.setTopLeft(QPoint(leftover, slidertop));
        } else {
            sliderRect.setTopLeft(QPoint(model()->iconVisible() ? thickness : 0, slidertop));
        }
        sliderRect.setWidth(sliderwidth - leftover);
        sliderRect.setHeight(thickness);

        backgroundTileCount = sliderRect.width() / thickness;
    } else { // vertical
        // Note: only icon is kept in same orientaton towards the user,
        // handle and background images are rotated 90 degrees

        // slider + icon fills the whole widget height (icon not rotated, so using its height)
        qreal sliderheight = size().height() - (model()->iconVisible() ? thickness : 0);
        // centered horizontally
        qreal sliderleft = (size().width() / 2) - (thickness / 2);
        qreal leftover = fmod(sliderheight, thickness);

        if (!reverse) {
            sliderRect.setTopLeft(QPoint(sliderleft, leftover));
        } else {
            sliderRect.setTopLeft(QPoint(sliderleft, model()->iconVisible() ? thickness : 0));
        }
        sliderRect.setWidth(thickness); // handle is rotated, so use height here
        sliderRect.setHeight(sliderheight - leftover);

        // background rotated -> use width
        backgroundTileCount = sliderRect.height() / thickness;
    }

    setHandleScreenPos(model()->position());

    releaseHandle();
}

inline qreal UnlockSliderView::clamp(qreal val, qreal min, qreal max) const
{
    return val < min ? min : (val > max ? max : val);
}

