#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define	SAMPLES_PER_SECOND	44100

/* Takes in a frequency (in MHz) */

void sample(int frequency) {
	int number_of_samples = SAMPLES_PER_SECOND / frequency;
	*DAC0_CH0DATA = number_of_samples;
	*DAC0_CH1DATA = number_of_samples;
}