
#include "callhandler.h"

#include <QDebug>

using namespace CallUi;

CallHandler::CallHandler(QObject *parent) :
    QObject(parent),
    callUi(NULL),
    calling(false)
{
}

CallHandler::~CallHandler()
{

}

bool CallHandler::callOngoing()
{
    return calling;
}

bool CallHandler::startCall()
{
    qDebug() << Q_FUNC_INFO;

    bool res = false;

    if(!callUi){
        callUi = new CallUiServiceApi(this);
    }

    if(callUi){
        qDebug() << Q_FUNC_INFO << "call";

        CallUi::PendingCallRequest *req = NULL;
        QString callNum(getenv( envVar() ));
        if(!callNum.isEmpty()) {
            req = callUi->RequestCellularCall(callNum);
        } else {
            req = callUi->RequestEmergencyCall();
        }
        res = connect(req, SIGNAL(finished(CallUi::PendingCallRequest *)),
                      this, SLOT(callStarted(CallUi::PendingCallRequest *)));
    }
    calling = res;
    return res;
}

void CallHandler::callStarted(CallUi::PendingCallRequest *req)
{
    if(req){
        qDebug() << Q_FUNC_INFO << "callId" << req->callId() << "isError:" << req->isError()
                << ";" << req->errorName() << ":" << req->errorMessage();

        if(req->callId().isEmpty() || req->isError()){
            qDebug() << Q_FUNC_INFO << "failed: open ui";
            calling = false;
            callDone(req->callId(), CallUi::Error, 0, req->errorMessage());
        } else if (callUi) {
            calling = true;
            uid = req->callId();
            connect(callUi, SIGNAL(CallEnded(QString, int, int, QString)),
                this, SLOT(callDone(QString, int, int, QString)));
            emit callStarted();
        }
    } else {
        qDebug() << Q_FUNC_INFO << "call start failure?";
        calling = false;
        callDone(0, CallUi::Error, 0, QString());
    }
}

void CallHandler::callDone(QString uid, int reason, int duration, QString message)
{
    qDebug() << Q_FUNC_INFO << "uid" << uid << "reason" << reason
                << "duration" << duration << ":" << message;

    if(CallUi::Error == reason || this->uid == uid){
        calling = false;
        this->uid.clear();
        emit callDone();

        if(callUi){
            disconnect(callUi, SIGNAL(CallEnded(QString, int, int, QString)),
                    this, SLOT(callDone(QString, int, int, QString)));
        }
    }
}
