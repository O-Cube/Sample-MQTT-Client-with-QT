#include "mqttdinterface.h"
#include "mymqttclient.h"
#include "mythread.h"

#include <QTime>
#include <QMessageBox>
#include <QByteArray>
#include <QMqttTopicFilter>
#include <QMqttSubscription>
#include <QMqttTopicName>
#include <QSqlDatabase>

MQTTDatabaseClient::MQTTDatabaseClient( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::UIClient )
    , client( new MyMqttClient( this ) )
{
    ui->setupUi( this );
    if ( ui->radioButton->isChecked() ) {
        ui->qosSpinBox->setDisabled( true );
        ui->usernameLineEdit->setDisabled( true );
        ui->passwaordLineEdit->setDisabled( true );
        ui->subscribeButton->setDisabled( true );
    }

    connect( ui->quitPushButton, &QPushButton::clicked, this, [] () {
        QApplication::quit();
    } );

    ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) + tr( " ") + client->getErrMessage() );
    ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" )+ tr( " ") + client->getConnectMessage() );

    // connections
    connect( ui->subscribeButton, &QPushButton::clicked, this, &MQTTDatabaseClient::onSubscribedButton_clicked );
    connect( ui->publishButton, &QPushButton::clicked, this, &MQTTDatabaseClient::onPublishButton_clicked);
    connect( ui->radioButton, &QRadioButton::clicked, this, &MQTTDatabaseClient::onRadioButton_clicked );
    connect( client, &MyMqttClient::messageReceived, this, [=] (const QByteArray &message, const QMqttTopicName &topic) {
        ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) \
                                   + tr( ": Message received: " ) + QString( message ) \
                                   + tr( "; Topic: ") + topic.name());
        processReceived_value(QString( message ), topic.name() );
        /* delete database connection after thread returns */
        QSqlDatabase::removeDatabase( "mysampledatabase" );
    } );
}

MQTTDatabaseClient::~MQTTDatabaseClient()
{
    delete ui;
    delete client;
}

void MQTTDatabaseClient::onSubscribedButton_clicked()
{
    const QString subscriptionMsg = ui->topicLineEdit->text();
    if ( subscriptionMsg.isEmpty() ) {
        QMessageBox::warning( this, tr( "No Subscription Topic" ), \
                                 tr( "No or wrong MQTT topic\n"
                                    "Please enter a correct MQTT topic" ), \
                                 QMessageBox::Close );

        ui->topicLineEdit->clear();
        ui->usernameLineEdit->clear();
        ui->passwaordLineEdit->clear();
        ui->qosSpinBox->setValue( 0 );
        return;
    }

    client->setUsername( ui->usernameLineEdit->text() );
    client->setPassword( ui->passwaordLineEdit->text() );

    const QMqttTopicFilter topicFilter( QString( ui->topicLineEdit->text() ) );

    if ( !topicFilter.isValid() ) {
        QMessageBox::warning( this, tr( "Invalid Topic"), \
                             tr( " The topic entered is invalid.\n"
                                 "Please enter another topic and try again" ), \
                             QMessageBox::Close );
        ui->topicLineEdit->clear();
        ui->usernameLineEdit->clear();
        ui->passwaordLineEdit->clear();
        ui->qosSpinBox->setValue( 0 );
        return;
    }
    const int qos = ui->qosSpinBox->value();

    if ( client->subscribe( topicFilter, qos ) != nullptr ) {
        ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) + tr(": Subscription to ") \
                                   + subscriptionMsg + " Successful" );
        return;
    } else {
        ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) + tr(": Subscription to ") \
                                   + subscriptionMsg + " Unsuccessful" );
        return;
    }
}

void MQTTDatabaseClient::onPublishButton_clicked()
{
    const QMqttTopicName topic( ui->publishLineEdit->text() );
    const QByteArray message( ui->msgLineEdit->text().toLocal8Bit() );
    const quint8 qos = static_cast<quint8>( ui->msgQoSpinBox->value() );

    client->publish( topic, message, qos );

    ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) + tr(": Message published to ") \
                               + ui->publishLineEdit->text() + " successful" );
    /* reset publish widgets */
    ui->publishLineEdit->clear();
    ui->msgLineEdit->clear();
    ui->msgQoSpinBox->setValue( 0 );
}

void MQTTDatabaseClient::onRadioButton_clicked()
{
    if ( !ui->radioButton->isChecked() ) {
        ui->subscribeButton->setDisabled( false );
        ui->unsubscribeButtin->setDisabled( true );
        ui->qosSpinBox->setDisabled( false );
        ui->usernameLineEdit->setDisabled( false );
        ui->passwaordLineEdit->setDisabled( false );
    } else {
        ui->subscribeButton->setDisabled( true );
        ui->unsubscribeButtin->setDisabled( false );
        ui->qosSpinBox->setDisabled( true );
        ui->usernameLineEdit->setDisabled( true );
        ui->passwaordLineEdit->setDisabled( true );
    }
}

void MQTTDatabaseClient::onQuery_status(const QString status)
{
    ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) + ": " + status );
}

void MQTTDatabaseClient::onQuery_error(const QString error)
{
    ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) + ": " + error );
}

void MQTTDatabaseClient::processReceived_value( const QString &message, const QString &topic )
{
    MyThread thread( message, topic, this );

    connect( &thread, &MyThread::queryStatus, this, &MQTTDatabaseClient::onQuery_status);
    connect( &thread, &MyThread::queryError, this, &MQTTDatabaseClient::onQuery_error);
    connect( &thread, &MyThread::finished, &thread, &MyThread::deleteLater );
    connect( &thread, &MyThread::db_status, this, [=] ( const QString &status ) {
        ui->logMsgTextEdit->append( QTime::currentTime().toString( "HH:mm:ss" ) + ": " + status );
    } );
    thread.start();
    thread.wait();
}
