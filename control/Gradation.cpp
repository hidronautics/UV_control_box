#include "Gradation.h"

Gradation::Gradation() :
        ControlBase("gradation") {
    direction = ControlBase::e_actionTypes::SET_DEPTH;
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(nextStep()));
}

void Gradation::start(ControlBase::e_actionTypes direction, float k_1, float k_2, float k_3,
                      int T1, int T2, int T3) {
    this -> direction = direction;
    this -> k_1 = k_1;
    this -> k_2 = k_2;
    this -> k_3 = k_3;
    this -> T1 = T1;
    this -> T2 = T2;
    this -> T3 = T3;
    this -> stage = 1;
    qDebug() << "Gradation::start";

    timer->start(T1);
    sendAction(direction, k_1);
}
void Gradation::nextStep() {
    timer->stop();

    switch (++stage) {
        case 2:
            timer->start(T2);
            sendAction(direction, k_2);
            break;
        case 3:
            timer->start(T3);
            sendAction(direction, k_3);
            break;
        case 4:
            stopByTiemr();
            break;
    }
}

void Gradation::stopByTiemr() {
    sendAction(direction, 0);
    emit(stop_signalByTiemr());
}

void Gradation::stopByButton() {
    timer->stop();
    sendAction(direction, 0);
}

