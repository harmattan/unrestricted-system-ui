#include "unlockslider.h"

UnlockSlider::UnlockSlider() :
        DuiWidgetController(NULL, new UnlockSliderModel)
{
}

UnlockSlider::~UnlockSlider()
{
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

void UnlockSlider::setPosition(const qreal& position)
{
    model()->setPosition(position);
}

void UnlockSlider::blink()
{
    model()->setBlinking(true);
}

void UnlockSlider::modelModified(const QList<const char*>& modifications)
{
    DuiWidgetController::modelModified(modifications);

    const char* member;
    foreach (member, modifications) {
        if (member == UnlockSliderModel::Position ) {
            if (model()->position() > 0.99f) {
                emit unlocked();
            }
        }
    }
}
