#ifndef audio_drv_h
#define audio_drv_h

#include <stdint.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <diag.h>
#include "mmf2_module.h"
#include "module_audio.h"
#include "audio_api.h"

#define MINI_DMIC_CLK_PIN    PD_16
#define DMIC_CLK_PIN    PD_14
#define DMIC_DATA_PIN   PD_18
void audioDMICPinChange(mm_context_t *p);

#endif