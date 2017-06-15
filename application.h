#ifndef APPLICATION_H
#define APPLICATION_H

#include "GIMMMLIB/gimmmconnection.h"

#include <QObject>
#include <QHostAddress>

class Application: public QObject
{
    Q_OBJECT
        GimmmConnection             __gimmmConn;
        QHostAddress                __hostAddress;
        quint16                     __hostPortNo;
        std::string                 __sessionId;

    public:
        Application();
    public slots:
        // FCM handle slots
        void handleNewUpstreamMessage(const QJsonDocument& json);
        void handleNewDownstreamRejectMessage(
                        const QJsonDocument& json,
                        const QString& reject_reason);
        void handleConnectionStarted();
        void handleConnectionEstablished();
        void handleConnectionShutdownStarted();
        void handleConnectionShutdownCompleted();
        void handleConnectionError(const QString& error);
        void handleConnectionLost();
        void handleConnectionHandshakeStarted();
        void handleSessionEstablished();
signals:
        void sendMessage(const QJsonDocument& jdoc);
   private:
        // setup functions
        void start();
        void readConfigFile();
        void setupPhantomStuff();
};

#endif // APPLICATION_H
