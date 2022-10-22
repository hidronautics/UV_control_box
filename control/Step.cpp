#include "Step.h"

Step::Step() :
        ControlBase("step") {
    direction = ControlBase::e_actionTypes::SET_DEPTH;
    timer = new QTimer;
}

void Step::start(ControlBase::e_actionTypes direction, float value, int time_msec) {
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(stop()));
    timer->start(time_msec);

    this->direction = direction;
    sendAction(direction, value);
}

void Step::stop() {
    timer->stop();
    sendAction(direction, 0);
    delete timer;
    emit(stop_signal());
}
