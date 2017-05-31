#include "application.h"
#include "macros.h"

#include <iostream>

#include <QSettings>

Application::Application()
{
    start();
}

void Application::start()
{
    readConfigFile();
    setupPhantomStuff();
}

/*!
 * \brief Application::readConfigFile
 */
void Application::readConfigFile()
{
    std::cout << "Reading config file..." << std::endl;
    QSettings ini("./config.ini", QSettings::IniFormat);

    //phantom port no.
    __hostPortNo = ini.value("PHANTOM_SECTION/port_no", 0).toInt();
    if (__hostPortNo == 0)
    {
        std::cout << "ERROR: Missing config parameter 'PHANTOM_SECTION/port_no. Exiting..." << std::endl;
        exit(0);
    }
    // phantom host addr
    QString addr = ini.value("PHANTOM_SECTION/host_address", "NULL").toString();
    if ( addr == "NULL")
    {
        std::cout << "ERROR: Missing config parameter 'PHANTOM_SECTION/host_address. Exiting..." << std::endl;
        exit(0);
    }
    if (!__hostAddress.setAddress(addr))
    {
        std::cout << "ERROR: Unable to parse PHANTOM_SECTION/host_address. Exiting..." << std::endl;
        exit(0);
    }

    // session id aka bundle identifier for ios app.
    __sessionId = ini.value("CLIENT_SECTION/session_id", "NULL").toString().toStdString();
    if ( __sessionId == "NULL")
    {
        std::cout<< "ERROR: No session id found. Exiting..." << std::endl;
        exit(0);
    }
}

void Application::setupPhantomStuff()
{
    // setup signal handlers
    connect(&__gimmmConn, SIGNAL(connectionStarted()),             this, SLOT(handleConnectionStarted()));
    connect(&__gimmmConn, SIGNAL(connectionEstablished()),         this, SLOT(handleConnectionEstablished()));
    connect(&__gimmmConn, SIGNAL(connectionHandshakeStarted()),    this, SLOT(handleConnectionHandshakeStarted()));
    connect(&__gimmmConn, SIGNAL(sessionEstablished()),            this, SLOT(handleSessionEstablished()));
    connect(&__gimmmConn, SIGNAL(connectionShutdownStarted()),     this, SLOT(handleConnectionShutdownStarted()));
    connect(&__gimmmConn, SIGNAL(connectionShutdownCompleted()),   this, SLOT(handleConnectionShutdownCompleted()));
    connect(&__gimmmConn, SIGNAL(connectionLost()),                this, SLOT(handleConnectionLost()));
    connect(&__gimmmConn, SIGNAL(connectionError(const QString&)), this, SLOT(handleConnectionError(const QString&)));
    connect(&__gimmmConn, SIGNAL(newUpstreamMessage(const QJsonDocument&)),this, SLOT(handleNewUpstreamMessage(const QJsonDocument&)));

    connect(&__gimmmConn, SIGNAL(newDownstreamRejectMessage(const QJsonDocument&, const QString&)),
            this, SLOT(handleNewDownstreamRejectMessage(const QJsonDocument&, const QString&)));

    //setup queued connection to fcmconn for sending downstream message to FCM.
    connect(this, SIGNAL(sendMessage(const QJsonDocument&)),&__gimmmConn, SLOT(handleSendMessage(const QJsonDocument&)));

    __gimmmConn.connectToServer(__hostAddress, __hostPortNo, __sessionId);
}

void Application::handleConnectionStarted()
{
    std::cout << "Connecting to Phantom server..." << std::endl;
}

void Application::handleConnectionEstablished()
{
    std::cout << "Connected to Phantom server. Establishing session..." << std::endl;
}

void Application::handleConnectionHandshakeStarted()
{
    std::cout << "Starting PHANTOM handshake..." << std::endl;
}

void Application::handleSessionEstablished()
{
    std::cout << "--------------------------------------------------------"  << std::endl;
    std::cout << "-     SESSION WITH PHANTOM ESTABLISHED SUCCESSFULLY    -"  << std::endl;
    std::cout << "--------------------------------------------------------"  << std::endl;
}


void Application::handleConnectionLost()
{
    std::cout << "Disconnected to Phantom server." << std::endl;
}

void Application::handleConnectionShutdownStarted()
{
    std::cout << "Shuting down connection to Phantom..." << std::endl;
}

void Application::handleConnectionShutdownCompleted()
{
    std::cout << "Connection to Phantom shutdown successfully." << std::endl;
}

void Application::handleConnectionError(const QString& error)
{
    std::cout << "ERROR: Connection error:" << error.toStdString() << std::endl;
}

void Application::handleNewUpstreamMessage(const QJsonDocument& client_msg)
{
  PRINT_JSON_DOC(std::cout, client_msg);
}

void Application::handleNewDownstreamRejectMessage(
    const QJsonDocument& client_msg,
    const QString& reject_readon)
{
  PRINT_JSON_DOC(std::cout, client_msg);
}

/*
void Application::sendTestMessage()
{
    std::cout << "Sending test message..." << std::endl;
    Message msg(MessageType::DOWNSTREAM);
    msg.setMessageId("800002");
    msg.setTo("feQqD9P957k:APA91bH-jE8j6dAohIB5spQrCajDwUsDtNdf3lImSi8Ebpkq0q8O1nI0O9Y5P3l6qd5D3sPfOYqnCNrOmnsMBt-mM1zxcbRknquMi2zvsqm7kkOj5UpksOduqwYVWosJoL--tiOkTRKa");
    msg.setTimeToLive(30);
    //msg.setDeliveryRecieptRequested(true);

    QJsonObject data;
    data["Msg"] = "Hello Rajesh";
    msg.setData(data);
    try
    {
        msg.validate();
        QJsonDocument jdoc = msg.toJson();
        emit sendMessage(jdoc);
    }
    catch (std::exception& ex)
    {
      PRINT_EXCEPTION_STRING(std::cout, ex);
    }
    catch (...)
    {
      std::cout <<"Unhandled exception" << std::endl;
    }
}
*/
