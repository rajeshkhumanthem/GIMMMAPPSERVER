#include "myfcmapplication.h"

#include "GIMMMAPPLIB/gimmmmessage.h"

#include <iostream>

#include <QJsonObject>
#include <QTimer>


#define PRINT_JSON_DOC(out, jsondoc) \
    QString str = jsondoc.toJson(QJsonDocument::Indented); \
    out << str.toStdString() << std::endl;

#define PRINT_JSON_DOC_RAW(out, jsondoc) \
    QString str = jsondoc.toJson(QJsonDocument::Compact); \
    out << str.toStdString() << std::endl;


/*!
 * \brief MyFcmApplication::MyFcmApplication
 */
MyFcmApplication::MyFcmApplication()
{
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
 * \brief MyFcmApplication::handleNewDownstreamAckMessage
 * \param ack_msg
 */
void MyFcmApplication::handleNewDownstreamAckMessage(const QJsonDocument& ack_msg)
{
    std::cout << "----------------------------START handleNewDownstreamAckMessage---------------------------------------------"  << std::endl;
    std::cout << "Recieved new ack message from fcm..." << std::endl;
    PRINT_JSON_DOC_RAW(std::cout, ack_msg);
    std::cout << "----------------------------END handleNewDownstreamAckMessage---------------------------------------------"  << std::endl;
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


void MyFcmApplication::handleNewDownstreamReceiptMessage(const QJsonDocument &json)
{
    std::cout << "----------------------------START handleNewDownstreamReceiptMessage---------------------------------------------"  << std::endl;
    QJsonObject orig_data = json.object().value("data").toObject();
    std::string msgid = orig_data.value("original_message_id").toString().toStdString();
    std::cout << "Recieved new receipt message from fcm for msgid:"  << msgid << std::endl;

    PRINT_JSON_DOC(std::cout, json);
    std::cout << "----------------------------END   handleNewDownstreamReceiptMessage---------------------------------------------"  << std::endl;
}


/*!
 * \brief MyFcmApplication::handleNewUpstreamMessage
 * \param client_msg
 */
void MyFcmApplication::handleNewUpstreamMessage(const QJsonDocument& client_msg)
{
    std::cout << "----------------------------START handleNewUpstreamMessage---------------------------------------------"  << std::endl;
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
        msg.setDeliveryRecieptRequested(true);

        QJsonDocument jdoc = msg.toJson();
        std::cout << "Echoing message back to sender..." << std::endl;
        PRINT_JSON_DOC_RAW(std::cout, jdoc);

        sendDownstreamMessage(jdoc);
    };
    // echo the message back to the sender after 5 secs.
    QTimer::singleShot(5000, Qt::TimerType::PreciseTimer, timerCallback);

    std::cout << "----------------------------END handleNewUpstreamMessage---------------------------------------------------"  << std::endl;
}
