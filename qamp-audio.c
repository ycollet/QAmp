#include <stdlib.h>
#include <math.h>

#include <lv2.h>

#include "qamp-audio.h"

#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

static LV2_Descriptor *ampDescriptor = NULL;

typedef struct {
  float *gain;
  float *input;
  float *output;
} QAmp_data;

static void cleanupAmp(LV2_Handle instance) {
#ifdef DEBUG
  fprintf(stderr, "cleanup called\n");
#endif
  free(instance);
}

static void connectPortAmp(LV2_Handle instance, uint32_t port, void *data) {
#ifdef DEBUG
  fprintf(stderr, "connectPort called\n");
#endif
  QAmp_data *plugin = (QAmp_data *)instance;

  switch (port) {
  case QAMP_GAIN:
    plugin->gain = data;
    break;
  case QAMP_INPUT:
    plugin->input = data;
    break;
  case QAMP_OUTPUT:
    plugin->output = data;
    break;
  }
}

static LV2_Handle instantiateAmp(const LV2_Descriptor *descriptor, double s_rate, const char *path,
				 const LV2_Feature * const* features) {
#ifdef DEBUG
  fprintf(stderr, "instantiate called\n");
#endif
  QAmp_data *plugin_data = (QAmp_data *)malloc(sizeof(QAmp));

  return (LV2_Handle)plugin_data;
}

static void runAmp(LV2_Handle instance, uint32_t sample_count) {
#ifdef DEBUG
  fprintf(stderr, "run called\n");
#endif
  QAmp_data *plugin_data = (QAmp_data *)instance;

  const float gain = *(plugin_data->gain);
  const float * const input = plugin_data->input;
  float * const output = plugin_data->output;
  float coef = DB_CO(gain);

  for(int pos = 0; pos < sample_count; pos++) {
    output[pos] = input[pos] * coef;
  }
}

static void initAmp() {
#ifdef DEBUG
  fprintf(stderr, "init called\n");
#endif
  ampDescriptor = (LV2_Descriptor *)malloc(sizeof(LV2_Descriptor));

  ampDescriptor->URI            = QAMP_URI;
  ampDescriptor->activate       = NULL;
  ampDescriptor->cleanup        = cleanupAmp;
  ampDescriptor->connect_port   = connectPortAmp;
  ampDescriptor->deactivate     = NULL;
  ampDescriptor->instantiate    = instantiateAmp;
  ampDescriptor->run            = runAmp;
  ampDescriptor->extension_data = NULL;
}

/*
  This function needs to have the name "lv2_descriptor" and needs to be tagged LV2_SYMBOL_EXPORT
  because the host, when loading this plugin, will look for lv2_descriptor function to perform
  the audio tasks.
 */
LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index) {
  if (!ampDescriptor) initAmp();
  
  switch (index) {
  case 0:
    return ampDescriptor;
  default:
    return NULL;
  }
}
