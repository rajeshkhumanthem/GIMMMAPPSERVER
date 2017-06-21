#ifndef APPLICATION_H
#define APPLICATION_H

#include "GIMMMAPPLIB/gimmmapplication.h"

class MyFcmApplication: public GimmmApplication
{
    public:
        MyFcmApplication();
    public slots:
        virtual void handleConnectionStarted();
        virtual void handleConnectionEstablished();
        virtual void handleConnectionShutdownStarted();
        virtual void handleConnectionShutdownCompleted();
        virtual void handleConnectionError(const QString& error);
        virtual void handleConnectionLost();
        virtual void handleConnectionHandshakeStarted();
        virtual void handleSessionEstablished();
        // application hooks
        virtual void handleNewUpstreamMessage(const QJsonDocument& json);
        virtual void handleNewDownstreamRejectMessage(
                        const QJsonDocument& json,
                        const QString& reject_reason);
};

#endif // APPLICATION_H
