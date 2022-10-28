#ifndef UV_CONTROL_BOX_STEP_H
#define UV_CONTROL_BOX_STEP_H

#include "ControlBase.h"

class Step : public ControlBase {
    Q_OBJECT

public:
    Step();

private:
    QTimer *timer;
    e_actionTypes direction;

public slots:
    void stopByTiemr();
    void stopByButton();
    void start(ControlBase::e_actionTypes direction, float value, int time_msec);

signals:
    void stop_signalByTiemr();
};

#endif //UV_CONTROL_BOX_STEP_H
