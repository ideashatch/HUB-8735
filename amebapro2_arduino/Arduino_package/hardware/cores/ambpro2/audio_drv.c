#include "audio_drv.h"
#include "module_audio.h"

#define DEFAULT_AUDIO_SETTINGS 0

void audioDMICPinChange(mm_context_t *p)
{
	audio_dmic_depinmux((audio_t *)p, MINI_DMIC_CLK_PIN, DMIC_DATA_PIN);
	audio_dmic_pinmux((audio_t *)p, DMIC_CLK_PIN, DMIC_DATA_PIN);
}