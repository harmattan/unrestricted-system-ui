#ifndef CALLHANDLER_H
#define CALLHANDLER_H

#include <QObject>
#include <call-ui/CallUiServiceApi>

using namespace CallUi;

/*!
 * Class to handle the call action for emergency call
 */
class CallHandler : public QObject
{
    Q_OBJECT

public:
    CallHandler(QObject *parent);
    ~CallHandler();

    bool callOngoing();

signals:
    void callStarted();
    void callDone();

public slots:
    bool startCall();

private slots:
    void callStarted(CallUi::PendingCallRequest *req);
    void callDone(QString uid, int reason, int duration, QString message);

private:
    CallUiServiceApi *callUi;
    QString uid;
    bool calling;
};

#endif // CALLHANDLER_H
