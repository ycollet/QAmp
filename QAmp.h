#ifndef QTAMP_HPP
#define QTAMP_HPP

#ifdef QT5
#  include <QtWidgets>
#else
#  include <QtGui>
#endif

#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

class QAmp : public QWidget
{
Q_OBJECT

public:
  QAmp();
  void set_volume(float volume);
  float get_volume() const;
  void set_controller(LV2UI_Controller func);
  void set_write_function(LV2UI_Write_Function func);
private slots:
  void volumeChanged(int value);
private:
  QSpinBox *qampValue;
  float gain;
  LV2UI_Controller controller;
  LV2UI_Write_Function write_function;
};

#endif
