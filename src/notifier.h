#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QString>

class Notifier : QObject
{
    Q_OBJECT

public:

    enum Notification {
        SIMCardInserted,
        SIMCardNotInserted,
        SIMCardRemoved,
        SIMCardRejected,
        SIMCardPermanentlyBlocked,
        EmergencyCallStarting,
        EmergencyCallEnded,
        TechnicalProblem,
        Empty,        
        PINCodeIncorrect, //generic, not really used
        PINCodeIncorrect2AttemptsLeft,
        PINCodeIncorrect1AttemptLeft,
        PINCodeIncorrectPUKCodeRequired,
        PUKCodeIncorrect,
        PUKCodeIncorrectSIMCardPermanentlyBlocked,
        PINCodeChanged,
        PINCodesDoNotMatch
    };

    Notifier();
    virtual ~Notifier();

public slots:    
    void showNotification(Notification notification);

private slots:
    void notificationTimeout();

private:
    QString mapNotificationText(Notification notification);

};


#endif // NOTIFIER_H
