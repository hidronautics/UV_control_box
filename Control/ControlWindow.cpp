#include "ControlWindow.h"

ControlWindow::ControlWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ControlWindow) {
    ui->setupUi(this);
    ui->CheckBoxMouse3d->setCheckState(Qt::Checked); // TODO: сделать автоматическую установку дефолтного чекбокса
//    connect(ui->CheckBoxKeyBoard, SIGNAL(stateChanged(state)), this, SLOT(MainWindow::enableController(Control::KEYBOARD, state)));
    connect(ui->CheckBoxKeyBoard, SIGNAL(stateChanged(int)), this, SLOT(KeyboardSelection(int)));
    connect(ui->CheckBoxMouse3d, SIGNAL(stateChanged(int)), this, SLOT(Mouse3dSelection(int)));
    connect(ui->CheckBoxJoystickLogitech, SIGNAL(stateChanged(int)), this, SLOT(JoystickLogitechSelection(int)));
}

ControlWindow::~ControlWindow() {
    delete ui;
}

void ControlWindow::KeyboardSelection(int state) {
    emit controllersEnabelChanged(Control::KEYBOARD, state);
}
void ControlWindow::Mouse3dSelection(int state) {
    emit controllersEnabelChanged(Control::MOUSE3D, state);
}
void ControlWindow::JoystickLogitechSelection(int state) {
    emit controllersEnabelChanged(Control::JOYSTICK, state);
}
