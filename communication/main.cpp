#include <QCoreApplication>
#include "pultcontrolsystemprotocols.h"
#include "iserverdata.h"
#include <QDebug>

//тут приведен пример, где создаются 2 обмена: объект обмена, который будет создан в программе Северова (controlProtocol)
//и обмен, который будет создан в программе Пульта (pultProtocol)
//структуры данных, которые передаются, описаны в "pultcontrolsystemprotocols.h"
//ToPult - то что передается от СУ Северова в Пульт
//FromPult - то, что передается от Пульта в СУ Северова
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qDebug() << "create Pult protocol";
    Pult::PC_Protocol *pultProtocol = new Pult::PC_Protocol(QHostAddress("127.0.0.1"), 13021, QHostAddress::LocalHost,
                                                            13020, 10);

    qDebug() << "-----start exchange";
    pultProtocol->startExchange();

    return a.exec();
}
