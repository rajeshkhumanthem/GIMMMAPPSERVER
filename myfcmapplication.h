#ifndef APPLICATION_H
#define APPLICATION_H

#include "GIMMMAPPLIB/gimmmapplication.h"

class MyFcmApplication: public GimmmApplication
{
    public:
        MyFcmApplication();
    public slots:
        virtual void handleConnectionEstablished();
        virtual void handleConnectionShutdownStarted();
        virtual void handleConnectionShutdownCompleted();
        virtual void handleConnectionLost();
        virtual void handleConnectionHandshakeStarted();
        virtual void handleSessionEstablished();
        // application hooks
        virtual void handleNewUpstreamMessage(const QJsonDocument& json);
        virtual void handleNewDownstreamAckMessage(const QJsonDocument& json);
        virtual void handleNewDownstreamRejectMessage(
                        const QJsonDocument& json,
                        const QString& reject_reason);
        virtual void handleNewDownstreamReceiptMessage(
                        const QJsonDocument& json);
};

#endif // APPLICATION_H
