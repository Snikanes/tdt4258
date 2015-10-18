#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define	SAMPLE_RATE	44100

/* Takes in a frequency (in MHz) */
#define SAMPLEHIGH 	 2048;
#define SAMPLELOW 	-2048;
static int counter = 0;

void sample(int frequency) {
	int samples_per_period = SAMPLE_RATE / frequency;
	squarewave(samples_per_period);
}

void squarewave(int samples_per_period, int timer_length) {
	if (counter >= samples_per_period / 2) {
		*DAC0_CH0CTRL = SAMPLELOW;
   		*DAC0_CH1CTRL = SAMPLELOW;
	}
	else {
		*DAC0_CH0CTRL = SAMPLEHIGH;
   		*DAC0_CH1CTRL = SAMPLEHIGH;
	}
	if (counter < samples_per_period) {
		counter++;
	}
	else if (counter == samples_per_period) {
		counter = 0;
	}
}