#include <QDebug>

#include "unlockslider.h"
#include "unlocksliderview.h"

UnlockSlider::UnlockSlider(QGraphicsItem *parent) :
        DuiWidgetController(new UnlockSliderModel(), parent)
{
    this->setView(view = new UnlockSliderView(this));
}

UnlockSlider::~UnlockSlider()
{
    view->deleteLater();
    view = NULL;
}

void UnlockSlider::hwKeyDown(bool down)
{
    qDebug() << "hwKeyDown " << down;
    //aHWKeyDown = down;
    if (!down) { //hw button released
        view->releaseHandle();
        emit released();
    }
}

bool UnlockSlider::isHandlePressed() const
{
    return model()->handlePressed();
}

const qreal& UnlockSlider::position() const
{
    return model()->position();
}

bool UnlockSlider::isIconVisible() const
{
    return model()->iconVisible();
}

void UnlockSlider::setIconVisible(bool iconVisible)
{
    model()->setIconVisible(iconVisible);
}

bool UnlockSlider::isMagnetic() const
{
    return model()->magnetic();
}

void UnlockSlider::setMagnetic(bool magnetic)
{
    model()->setMagnetic(magnetic);
}

void UnlockSlider::setOrientation(Qt::Orientation orientation)
{
    model()->setOrientation(orientation);
}

Qt::Orientation UnlockSlider::orientation() const
{
    return model()->orientation();
}

void UnlockSlider::setPosition(const qreal& position)
{
    model()->setPosition(position);
}

void UnlockSlider::reset()
{
    model()->setPosition(0);
}

void UnlockSlider::blink()
{
    model()->setBlinking(true);
}

void UnlockSlider::updateData(const QList<const char*>& modifications)
{
    DuiWidgetController::updateData(modifications);

    const char* member;
    foreach(member, modifications) {
        if (member == UnlockSliderModel::Position) {
            if (model()->position() > 0.99f) {
                emit unlocked();
            }
        }
        if (member == UnlockSliderModel::HandlePressed) {
            qDebug() << "released " << model()->handlePressed();
            if (!model()->handlePressed())
                emit released();
        }
    }
}
