#include <iostream>

#include "amp.h"
#include "QAmp.h"

QAmp::QAmp() {
  qampValue = new QSpinBox;
  qampValue->setRange(0, 100);
  qampValue->setSingleStep(1);
    
  QVBoxLayout *controlsLayout = new QVBoxLayout;
  controlsLayout->addWidget(qampValue);

  qampValue->setValue(100);
  gain = 100.0;
  
  setWindowTitle(tr("QAmp"));

  connect(this, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
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
  std::cerr << "DEBUG: gain = " << value << std::endl;
}
