#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define	SAMPLE_RATE	32768
#define SAMPLEHIGH 	5
#define SAMPLELOW	0

int counter = 0;

void squarewave(int samples_per_period) {
	if (counter >= (samples_per_period) / 2) {
		*DAC0_CH0DATA = SAMPLELOW;
   		*DAC0_CH1DATA = SAMPLELOW;
	}
	else {
		*DAC0_CH0DATA = SAMPLEHIGH;
   		*DAC0_CH1DATA = SAMPLEHIGH;
	}
	if (counter >= samples_per_period) {
		counter = 0;
	}
	counter++;
}

void sample(int frequency) {
	int samples_per_period = SAMPLE_RATE / frequency;
	squarewave(samples_per_period);
}
