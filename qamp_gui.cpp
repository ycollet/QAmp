#include <iostream>

#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "amp.h"
#include "QAmp.h"

typedef struct {
  LV2UI_Widget volume_control;
} QAmpGui;

static LV2UI_Widget make_gui(LV2UI_Controller controller,
			 LV2UI_Write_Function write_function) {
  std::cerr << "DEBUG: make_gui called" << std::endl;
  
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
  std::cerr << "DEBUG: instanciate called" << std::endl;
  
  if (strcmp(plugin_uri, QAMP_URI) != 0) {
    std::cerr << "QAMP_UI error: this GUI does not support plugin with URI " << plugin_uri << std::endl;
    return NULL;
  }
  
  QAmpGui* pluginGui = (QAmpGui*)malloc(sizeof(QAmpGui));
  if (pluginGui == NULL) return NULL;
  
  pluginGui->volume_control = (LV2UI_Widget)make_gui(controller, write_function);
  return (LV2UI_Handle)pluginGui;
}

static void cleanup(LV2UI_Handle ui) {
  std::cerr << "DEBUG: cleanup called" << std::endl;
  
  QAmpGui *pluginGui = (QAmpGui *) ui;
  free(pluginGui);
}

static void port_event(LV2UI_Handle ui,
		       uint32_t port_index,
		       uint32_t buffer_size,
		       uint32_t format,
		       const void * buffer) {
  std::cerr << "DEBUG: port_event_called" << std::endl;
  
  QAmpGui *pluginGui = (QAmpGui *)ui;
  float * pval = (float *)buffer;
    
  if (format != 0) return;
  if ((port_index < 0) || (port_index >= QAMP_N_PORTS)) return;

  ((QAmp *)pluginGui->volume_control)->set_volume(*pval);
}

static const LV2UI_Descriptor descriptor = {
  QAMP_UI_URI, // URI
  instantiate, // instanciate function
  cleanup,     // cleanup function
  port_event,  // port_event function
  NULL         // extension_data function
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
  std::cerr << "DEBUG: lv2ui_descriptor called - index = " << index << " - descriptor = " << &descriptor << std::endl;
  
  switch (index) {
  case 0:
    return &descriptor;
  default:
    return NULL;
  }
}
