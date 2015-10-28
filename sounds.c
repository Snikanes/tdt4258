#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "shared.h"

void sample();

int current_tone = 0;
int samples_remaining = 0;
bool first_sample = true;
bool playing = true;
void goToSleep();
void wakeFromSleep();

void continuePlaying(int tones[], int tone_lengths[], int tones_in_song) {
	if (current_tone < tones_in_song) {
		if (first_sample) {
			samples_remaining = tone_lengths[current_tone];
			first_sample = false;
		}
		if (samples_remaining == 0) {
			current_tone++;
			samples_remaining = tone_lengths[current_tone];
		}
		
		if (tones[current_tone] != -1) {
			sample(tones[current_tone]);
		}
		samples_remaining--;
	}
	else {
		goToSleep();
	}
}

void resetMelody() {
	first_sample = true;
	current_tone = 0;
}