#include "unlockslidermodel.h"

const int& UnlockSliderModel::minimum() const
{
    return _minimum();
}

void UnlockSliderModel::setMinimum(const int& minimum)
{
    //update the value and emit change signal only if the value has changed
    if (minimum != _minimum()) {
        _minimum() = minimum;
        memberModified(Minimum);
    }

    // the maximum will be at least the minimum
    if (minimum > _maximum()) {
       setMaximum(minimum);
    }

    //limit the value to new min and max
    setValue(value());
}

const int& UnlockSliderModel::maximum() const
{
    return _maximum();
}

void UnlockSliderModel::setMaximum(const int& maximum)
{
    //update the value and emit change signal only if the value has changed
    if (maximum != _maximum()) {
        _maximum() = maximum;
        memberModified(Maximum);
    }

    // the minimum will be at most the maximum
    if (maximum < _minimum()) {
        setMinimum(maximum);
    }

    //limit the value to new min and max
    setValue(value());
}

const int& UnlockSliderModel::value() const
{
    return _value();
}

void UnlockSliderModel::setValue(const int& value)
{
    int setVal = value;
    if (setVal < _minimum()) {
        setVal = _minimum();
    }

    if (setVal > _maximum()) {
        setVal = _maximum();
    }

    if (setVal != _value()) {
        _value() = setVal;
        memberModified(Value);
    }
}
