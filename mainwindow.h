#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSettings>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include <QDebug>
#include <QShortcut>
#include <QApplication>
#include <QThread>
#include <QTimer>

#include "UV/iuserinterfacedata.h"
#include "communication/pultcontrolsystemprotocols.h"
#include "control/Joystick.h"
#include "control/Step.h"
#include "control/Gradation.h"
#include "control/ControlBase.h"
#include "UV/uv_state.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateUi_fromROV(); //обновляет на экране данные, пришедшие с аппарата
    void updateUi_fromControl(); //обновляет на экране данные, пришедшие с джостика

    void cSModeChange_auto();
    void cSModeChange_identification1();
    void cSModeChange_identification2();
    void cSModeChange_handle();
    void cSModeChange_thruster(bool checked);

    void typicalInput_start();
    void typicalInput_stopByButton();
    void typicalInput_stopByTiemr();

    void powerThrusterToggled(bool state);

    void stabilizeYawToggled(bool state);
    void stabilizePitchToggled(bool state);
    void stabilizeRollToggled(bool state);
    void stabilizeMarchToggled(bool state);
    void stabilizeDepthToggled(bool state);
    void stabilizeLagToggled(bool state);

signals:
    void updateCompass(double yaw);
    void updateDepth(int depth);
    void updateDepth(double depth);

private:
    Step* step;
    Gradation* gradation;
    QTimer* updateControl_timer;
    //    QTimer *typicalInput_timer;
    //    int typicalInput_timeCounter = 0;

    Ui::MainWindow* ui;

    IUserInterfaceData uv_interface;
    Pult::PC_Protocol* pultProtocol;

    void cSModeChange_thrusterOn();
    void cSModeChange_thrusterOff();

    Joystick* joystick = nullptr;

};
#endif // MAINWINDOW_H
