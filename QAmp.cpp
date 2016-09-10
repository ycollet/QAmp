#include <iostream>

#include <QCloseEvent>
#include <QDebug>

#include "amp.h"
#include "QAmp.h"

QAmp::QAmp(QWidget *parent, Qt::WindowFlags wflags) : QWidget(parent, wflags) {
  std::cerr << "QAmp constructor called" << std::endl;
  std::cerr << "QAmp: parent = " << parent << " wflags = " << wflags << std::endl;
  
#if QT_VERSION >= 0x050000
  // HACK: Dark themes grayed/disabled color group fix...
  QPalette pal;
  if (pal.base().color().value() < 0x7f) {
    const QColor& color = pal.window().color();
    const int iGroups = int(QPalette::Active | QPalette::Inactive) + 1;
    for (int i = 0; i < iGroups; ++i) {
      const QPalette::ColorGroup group = QPalette::ColorGroup(i);
      pal.setBrush(group, QPalette::Light,    color.lighter(150));
      pal.setBrush(group, QPalette::Midlight, color.lighter(120));
      pal.setBrush(group, QPalette::Dark,     color.darker(150));
      pal.setBrush(group, QPalette::Mid,      color.darker(120));
      pal.setBrush(group, QPalette::Shadow,   color.darker(200));
    }
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, pal.mid().color());
    QWidget::setPalette(pal);
  }
#endif
  //setFixedHeight(100);
  //setFixedWidth(100);

  resize(100, 100);
  
  write_function = NULL;
  controller = NULL;
  
  qampValue = new QDial(this);
  qampValue->setRange(-100, 100);
  qampValue->setSingleStep(1);
  qampValue->setValue(90);
  gain = 90.0;
  
  QObject::connect(qampValue, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
  
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addStretch(1);
  vLayout->addWidget(qampValue);
  
  setLayout(vLayout);
  
  idleClosed = false;

  std::cerr << "QAmp: constructor exited" << std::endl;
}

QAmp::~QAmp() {}

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

  std::cerr << "volumeChanged: gain = " << gain << std::endl;
  
  if (write_function)
    write_function(controller, QAMP_GAIN, sizeof(gain), 0, &gain);
}

void QAmp::port_event(uint32_t port_index, uint32_t buffer_size,
		      uint32_t format, const void *buffer) {
  std::cerr << "port_event: called" << std::endl;
  if (format == 0 && buffer_size == sizeof(float)) {
    float fValue = *(float *) buffer;
    gain = fValue;
  }
  std::cerr << "port_event: exited" << std::endl;
}

// Close event handler.
void QAmp::closeEvent(QCloseEvent *pCloseEvent) {
  std::cerr << "closeEvent: called" << std::endl;
  closeEvent(pCloseEvent);
  
  if (pCloseEvent->isAccepted())
    idleClosed = true;
  std::cerr << "closeEvent: exited" << std::endl;
}

bool QAmp::isIdleClosed() const {
  std::cerr << "isIdleClosed: called" << std::endl;
  return idleClosed;
}
