#include "mymqttclient.h"

#include <QEventLoop>
#include <memory>
#include <QMqttTopicFilter>
#include <QMqttSubscription>

MyMqttClient::MyMqttClient( QObject *parent )
    : QMqttClient( parent )
    , errmsg( "" )
    , connectionmsg( "" )
    , m_port( 0 )
{
    setHostname( tr( "127.0.0.1" ) );
    setPort( 1883 );
    connectToHost();

    /* wait for my mqtt client to connect to broker */
    QEventLoop loop( this );
    connect( this, &QMqttClient::connected, &loop, &QEventLoop::quit );
    loop.exec();

}

QString MyMqttClient::getErrMessage() const
{
    return myClientErrorMessage( error() );
}

QString MyMqttClient::getConnectMessage() const
{
    return myClientConnectionMessage( state() );
}

QString MyMqttClient::myClientErrorMessage(QMqttClient::ClientError err) const
{
    switch ( err ) {

    case QMqttClient::NoError:
        return "Connection to Broker: Successful";

    case QMqttClient::InvalidProtocolVersion:
        return "Connection to Broker: Broker does accept connection using the specifies protocol version";

    case QMqttClient::IdRejected:
        return "Connection to Broker: Client ID malformed";

    case QMqttClient::ServerUnavailable:
        return "Connection to Broker: Server unavailable";

    case QMqttClient::BadUsernameOrPassword:
        return "Connection to Broker: Wrong username or password";

    case QMqttClient::NotAuthorized:
        return "Connection to Broker: Client has no authorization to connect";

    case QMqttClient::TransportInvalid:
        return "Connection to Broker: Unexpected transport error";

    case QMqttClient::ProtocolViolation:
        return "Connection to Broker: Client encountered a protocol violation";

    case QMqttClient::UnknownError:
        return "Connection to Broker: Unknown error";

    case QMqttClient::Mqtt5SpecificError:
        return "Level 5 MQTT protocol";

    default:
        return "Connection to Broker: Undefined Error";
    }
}

QString MyMqttClient::myClientConnectionMessage(const ClientState state) const
{
    switch( state ) {

    case QMqttClient::Disconnected :
        return "Connection to Broker: Disconnected";

    case QMqttClient::Connecting :
        return "Connection to Broker: Connecting";

    case QMqttClient::Connected :
        return "Connection to Broker: Connected";

    default :
        return "Connection to Broker: Connection status undefined";
    }
}
