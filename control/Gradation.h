#ifndef UV_CONTROL_BOX_GRADATION_H
#define UV_CONTROL_BOX_GRADATION_H

#include "ControlBase.h"

class Gradation : public ControlBase {
Q_OBJECT

public:
    Gradation();

private:
    QTimer *timer;
    e_actionTypes direction;
    float k_1 = 0;
    float k_2 = 0;
    float k_3 = 0;
    int T1 = 0;
    int T2 = 0;
    int T3 = 0;

    int stage;

public slots:
    void stopByTiemr();
    void stopByButton();
    void start(ControlBase::e_actionTypes direction, float k_1, float k_2, float k_3,
               int T1, int T2, int T3);
    void nextStep();

signals:
    void stop_signalByTiemr();
};

#endif //UV_CONTROL_BOX_GRADATION_H
