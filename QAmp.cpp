#include <iostream>

#include <QDebug>

#include "amp.h"
#include "QAmp.h"

QAmp::QAmp(QWidget *parent, Qt::WindowFlags wflags) : QWidget(parent, wflags) {
  setFixedHeight(100);
  setFixedWidth(100);
  
  qampValue = new QDial();
  qampValue->setRange(-100, 100);
  qampValue->setSingleStep(1);
  qampValue->setValue(90);
  gain = 90.0;
  
  QObject::connect(qampValue, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
  
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addStretch(1);
  vLayout->addWidget(qampValue);
  
  setLayout(vLayout);
  
  setWindowTitle(tr("QAmp"));
}

void QAmp::set_volume(float volume) {
  qampValue->setValue(volume);
}

float QAmp::get_volume() const {
  return (float)qampValue->value();
}

void QAmp::set_controller(LV2UI_Controller func) {
  controller = func;
}

void QAmp::set_write_function(LV2UI_Write_Function func) {
  write_function = func;
}

void QAmp::volumeChanged(int value) {
  gain = (float)value;
  write_function(controller, QAMP_GAIN, sizeof(gain), 0, &gain);
}
