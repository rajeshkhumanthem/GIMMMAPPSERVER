#include "myfcmapplication.h"

#include "GIMMMAPPLIB/gimmmmessage.h"

#include <iostream>

#include <QJsonObject>
#include <QTimer>


#define PRINT_JSON_DOC(out, jsondoc) \
    QString str = jsondoc.toJson(QJsonDocument::Indented); \
    out << str.toStdString() << std::endl;


/*!
 * \brief MyFcmApplication::MyFcmApplication
 */
MyFcmApplication::MyFcmApplication()
{
}


/*!
 * \brief MyFcmApplication::handleConnectionStarted
 */
void MyFcmApplication::handleConnectionStarted()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Connecting to GIMMM server..." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleConnectionEstablished
 */
void MyFcmApplication::handleConnectionEstablished()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Connected to GIMMM server... " << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleConnectionHandshakeStarted
 */
void MyFcmApplication::handleConnectionHandshakeStarted()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Starting GIMMM handshake.Sending LOGON request..." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleSessionEstablished
 */
void MyFcmApplication::handleSessionEstablished()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "-     SESSION WITH GIMMM SERVER ESTABLISHED SUCCESSFULLY!                                         -"  << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleConnectionLost
 */
void MyFcmApplication::handleConnectionLost()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Disconnected from GIMMM server." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}

/*!
 * \brief MyFcmApplication::handleConnectionShutdownStarted
 */
void MyFcmApplication::handleConnectionShutdownStarted()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Shuting down connection to Phantom..." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleConnectionShutdownCompleted
 */
void MyFcmApplication::handleConnectionShutdownCompleted()
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "Connection to GIMMM shutdown successfully." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleConnectionError
 * \param error
 */
void MyFcmApplication::handleConnectionError(const QString& error)
{
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
    std::cout << "ERROR: Connection error:" << error.toStdString() << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleNewDownstreamRejectMessage
 * \param client_msg
 * \param reject_reason
 */
void MyFcmApplication::handleNewDownstreamRejectMessage(
    const QJsonDocument& client_msg,
    const QString& reject_reason)
{
  PRINT_JSON_DOC(std::cout, client_msg);
  std::cout << "Reject Reason:" << reject_reason.toStdString() << std::endl;
}


/*!
 * \brief MyFcmApplication::handleNewUpstreamMessage
 * \param client_msg
 */
void MyFcmApplication::handleNewUpstreamMessage(const QJsonDocument& client_msg)
{
    std::cout << "----------------------------START NEW UPSTREAM MESSAGE---------------------------------------------"  << std::endl;
    std::cout << "Recieved new message..." << std::endl;
    PRINT_JSON_DOC(std::cout, client_msg);

    auto timerCallback = [this, client_msg]{
        std::string to = client_msg.object().value("from").toString().toStdString();
        QJsonObject orig_data = client_msg.object().value("data").toObject();

        DataMessage msg;
        msg.setTo(to.c_str());

        std::string msgid = getNextMessageId();
        msg.setMessageId(msgid);
        msg.setData(orig_data);

        QJsonDocument jdoc = msg.toJson();
        std::cout << "Echoing message back to sender..." << std::endl;
        PRINT_JSON_DOC(std::cout, jdoc);

        sendDownstreamMessage(jdoc);
    };
    // echo the message back to the sender after 5 secs.
    QTimer::singleShot(5000, Qt::TimerType::PreciseTimer, timerCallback);

    std::cout << "----------------------------END UPSTREAM MESSAGE---------------------------------------------------"  << std::endl;
}
