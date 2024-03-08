#include <QApplication>

#include "mqttdinterface.h"
#include "databasemanagement.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MQTTDatabaseClient client;
    client.show();

    return a.exec();
}
