#include <iostream>
#include <cstring>

#include <QApplication>
#include <QDebug>

#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "amp.h"
#include "QAmp.h"

static QApplication *qamp_qapp_instance = NULL;
static unsigned int  qamp_qapp_refcount = 0;

static LV2UI_Handle instantiate(const struct _LV2UI_Descriptor * descriptor,
				const char * plugin_uri,
				const char * bundle_path,
				LV2UI_Write_Function write_function,
				LV2UI_Controller controller,
				LV2UI_Widget * widget,
				const LV2_Feature * const * features) {
  qDebug() << "instanciate called";
  
  if (std::strcmp(plugin_uri, QAMP_URI) != 0) {
    std::cerr << "QAMP_UI error: this GUI does not support plugin with URI " << plugin_uri << std::endl;
    return NULL;
  }

  if (qamp_qapp_instance == NULL) {
    static int s_argc = 1;
    static const char *s_argv[] = { __func__, NULL };
    qamp_qapp_instance = new QApplication(s_argc, (char **) s_argv);
  }
  qamp_qapp_refcount++;

  QAmp * pWidget = new QAmp();

  pWidget->set_controller(controller);
  pWidget->set_write_function(write_function);
  
  *widget = pWidget;
  
  return pWidget;
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
  qDebug() << "port_event called";

  QAmp *pWidget = static_cast<QAmp *> (ui);
  if (pWidget)
    pWidget->port_event(port_index, buffer_size, format, buffer);
  
  qDebug() << "port_event exited";
}

int qamp_lv2ui_show(LV2UI_Handle ui) {
  qDebug() << "lv2ui_show called";
  
  QAmp *pWidget = static_cast<QAmp *>(ui);
  if (pWidget) {
    pWidget->show();
    pWidget->raise();
    pWidget->activateWindow();
    return 0;
  } else {
    return 1;
  }
}

int qamp_lv2ui_hide(LV2UI_Handle ui) {
  qDebug() << "lv2ui_hide called";
    
  QAmp *pWidget = static_cast<QAmp *>(ui);
  if (pWidget) {
    pWidget->hide();
    return 0;
  } else {
    return 1;
  }
}

int qamp_lv2ui_idle(LV2UI_Handle ui) {
  qDebug() << "lv2ui_idle called";
  
  QAmp *pWidget = static_cast<QAmp *>(ui);
  if (pWidget && !pWidget->isIdleClosed()) {
    QApplication::processEvents();
    return 0;
  } else {
    return 1;
  }
}

static const LV2UI_Show_Interface qamp_lv2ui_show_interface = {
  qamp_lv2ui_show,
  qamp_lv2ui_hide
};

static const LV2UI_Idle_Interface qamp_lv2ui_idle_interface = {
  qamp_lv2ui_idle
};

static const void *extension_data(const char *uri) {
  qDebug() << "extension_data called";
  
  if (std::strcmp(uri, LV2_UI__idleInterface) == 0)
    return (void *) &qamp_lv2ui_idle_interface;
  else
    if (std::strcmp(uri, LV2_UI__showInterface) == 0)
      return (void *) &qamp_lv2ui_show_interface;
    else
      return NULL;
}

static const LV2UI_Descriptor descriptor = {
  QAMP_UI_URI,   // URI
  instantiate,   // instanciate function
  cleanup,       // cleanup function
  port_event,    // port_event function
  extension_data // extension_data function
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
