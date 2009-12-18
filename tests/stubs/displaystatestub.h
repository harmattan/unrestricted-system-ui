#ifndef QMDISPLAYSTATE_H
#define QMDISPLAYSTATE_H

#include <QObject>

// STUB CLASS

namespace Maemo
{

class QmDisplayState : public QObject
{
    Q_OBJECT

public:
    /** Possible states for device display. */
    enum DisplayState {
        Off = -1,   /**< Display is off */
        Dimmed = 0, /**< Display is dimmed */
        On = 1      /**< Display is on */
    };

    QmDisplayState(QObject *parent = 0);
    virtual ~QmDisplayState();

    /**
     * Get the current display state.
     * @return Current display state.
     */
    QmDisplayState::DisplayState get() const;

    //NOT YET IMPLEMENTED IN THE REAL QMDISPLAYSTATE
    int getMaxDisplayBrightnessValue();
    int getDisplayBrightnessValue();
    int getDisplayBlankTimeout();
    int getDisplayDimTimeout();
    bool getBlankingWhenCharging();
    void setDisplayBrightnessValue(int value);
    void setDisplayBlankTimeout(int value);
    void setDisplayDimTimeout(int value);
    void setBlankingWhenCharging(bool value);

    /**
     * Set the current display state.
     * @param state Display state new set.
     * @return True on success, false otherwise.
     */
    bool set(DisplayState state);

    /**
     * Request not to blank the display. Must be repeated every 60 seconds to renew request.
     * Also prevents suspending.
     * @return True on success, false otherwise.
     */
    bool setBlankingPause(void);

signals:
    /**
     * Sent when display state has changed.
     * @param state Current display state.
     */
    void displayStateChanged(Maemo::QmDisplayState::DisplayState state);

private:
    QmDisplayState::DisplayState state;
    int brightness;
    int blankTimeout;
    int dimTimeout;
    bool blankingWhenCharging;

};
}

#endif // QMDISPLAYSTATE_H
