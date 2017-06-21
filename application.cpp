#include "application.h"

#include <iostream>

#include <QSettings>
#include <QFileInfo>

#define PRINT_JSON_DOC(out, jsondoc) \
    QString str = jsondoc.toJson(QJsonDocument::Indented); \
    out << str.toStdString() << std::endl;

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
    QString filename;

    if (QFileInfo::exists("./config.ini"))
    {
        filename = "./config.ini";
    }else
    {
        std::cout << "ERROR. Configuration file 'config.ini' missing. Please do the following:\n"
                  << "1) Copy 'config.copy.ini' as 'config.ini' into the same dir as the executable.\n"
                  << "2) Update 'config.ini' with the appropriate credential and try again. Exiting..." << std::endl;
        exit(0);
    }
    QSettings ini(filename, QSettings::IniFormat);


    //phantom port no.
    __hostPortNo = ini.value("GIMMMSERVER_SECTION/port_no", 0).toInt();
    if (__hostPortNo == 0)
    {
        std::cout << "ERROR: Missing config parameter 'GIMMMSERVER_SECTION/port_no. Exiting..." << std::endl;
        exit(0);
    }
    // phantom host addr
    QString addr = ini.value("GIMMMSERVER_SECTION/host_address", "NULL").toString();
    if ( addr == "NULL")
    {
        std::cout << "ERROR: Missing config parameter 'GIMMMSERVER_SECTION/host_address. Exiting..." << std::endl;
        exit(0);
    }
    if (!__hostAddress.setAddress(addr))
    {
        std::cout << "ERROR: Unable to parse GIMMMSERVER_SECTION/host_address. Exiting..." << std::endl;
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
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Connecting to Phantom server..." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleConnectionEstablished()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Connected to Phantom server... " << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleConnectionHandshakeStarted()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Starting GIMMM handshake.Sending LOGON request..." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleSessionEstablished()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "-     SESSION WITH GIMMM SERVER ESTABLISHED SUCCESSFULLY!                                         -"  << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleConnectionLost()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Disconnected to Phantom server." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleConnectionShutdownStarted()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Shuting down connection to Phantom..." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleConnectionShutdownCompleted()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Connection to Phantom shutdown successfully." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleConnectionError(const QString& error)
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "ERROR: Connection error:" << error.toStdString() << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


void Application::handleNewUpstreamMessage(const QJsonDocument& client_msg)
{
    std::cout << "----------------------------START NEW UPSTREAM MESSAGE---------------------------------------------"  << std::endl;
    PRINT_JSON_DOC(std::cout, client_msg);
    std::cout << "----------------------------END UPSTREAM MESSAGE---------------------------------------------------"  << std::endl;
}


void Application::handleNewDownstreamRejectMessage(
    const QJsonDocument& client_msg,
    const QString& reject_reason)
{
  PRINT_JSON_DOC(std::cout, client_msg);
  std::cout << "Reject Reason:" << reject_reason.toStdString() << std::endl;
}
