#include <iostream>

#include <QApplication>
#include <QDebug>

#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "amp.h"
#include "QAmp.h"

static QApplication *qamp_qapp_instance = NULL;
static unsigned int  qamp_qapp_refcount = 0;

static LV2UI_Widget make_gui(LV2UI_Controller controller,
			 LV2UI_Write_Function write_function) {
  qDebug() << "make_gui called";
  
  QAmp * widget = new QAmp(NULL);

  widget->set_controller(controller);
  widget->set_write_function(write_function);
  widget->show();

  return (LV2UI_Widget)widget;
}

static LV2UI_Handle instantiate(const struct _LV2UI_Descriptor * descriptor,
				const char * plugin_uri,
				const char * bundle_path,
				LV2UI_Write_Function write_function,
				LV2UI_Controller controller,
				LV2UI_Widget * widget,
				const LV2_Feature * const * features) {
  qDebug() << "instanciate called";
  
  if (strcmp(plugin_uri, QAMP_URI) != 0) {
    std::cerr << "QAMP_UI error: this GUI does not support plugin with URI " << plugin_uri << std::endl;
    return NULL;
  }

  if (qamp_qapp_instance == NULL) {
    static int s_argc = 1;
    static const char *s_argv[] = { __func__, NULL };
    qamp_qapp_instance = new QApplication(s_argc, (char **) s_argv);
  }
  qamp_qapp_refcount++;
  
  return (LV2UI_Handle)make_gui(controller, write_function);
}

static void cleanup(LV2UI_Handle ui) {
  qDebug() << "cleanup called";
  QWidget *pWidget = static_cast<QWidget *> (ui);
  if (pWidget) {
    delete pWidget;
    if (--qamp_qapp_refcount == 0 && qamp_qapp_instance) {
      delete qamp_qapp_instance;
      qamp_qapp_instance = NULL;
    }
  }
}

static void port_event(LV2UI_Handle ui,
		       uint32_t port_index,
		       uint32_t buffer_size,
		       uint32_t format,
		       const void * buffer) {
  qDebug() << "port_event_called";
  
  float * pval = (float *)buffer;
    
  if (format != 0) return;
  if ((port_index < 0) || (port_index >= QAMP_N_PORTS)) return;

  ((QAmp *)ui)->set_volume(*pval);
}

static const LV2UI_Descriptor descriptor_qt5 = {
  QAMP_UI_URI, // URI
  instantiate, // instanciate function
  cleanup,     // cleanup function
  port_event,  // port_event function
  NULL         // extension_data function
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
  qDebug() << "lv2ui_descriptor called - index = " << index << " - descriptor = " << &descriptor;
  
  switch (index) {
  case 0:
    return &descriptor;
  default:
    return NULL;
  }
}
