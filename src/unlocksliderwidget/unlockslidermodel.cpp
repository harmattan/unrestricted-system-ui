#include "unlockslidermodel.h"

const qreal& UnlockSliderModel::position() const
{
    return _position();
}

void UnlockSliderModel::setPosition(const qreal& position)
{
    // limit position to 0..1 range
    qreal pos = position < 0.0f ? 0.0f : (position > 1.0f ? 1.0f : position);

    if (pos != _position()) {
        _position() = pos;
        memberModified(Position);
    }
}
