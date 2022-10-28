#include "Step.h"

Step::Step() :
        ControlBase("step") {
    direction = ControlBase::e_actionTypes::SET_DEPTH;
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(stopByTiemr()));
}

void Step::start(ControlBase::e_actionTypes direction, float value, int time_msec) {
    timer->start(time_msec);

    this->direction = direction;
    sendAction(direction, value);
}

void Step::stopByTiemr() {
    timer->stop();
    sendAction(direction, 0);
    emit(stop_signalByTiemr());
}

void Step::stopByButton() {
    timer->stop();
    sendAction(direction, 0);
}
