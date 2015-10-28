#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "shared.h"

void sample();
void play_tone(int tones, int tone_length);
void continuePlaying(int tones[], int tone_lengths[], int tones_in_song);
void disableDAC();
void enableDAC();
void disableLETimer();
void resetMelody();
void enableLETimer();

void wakeFromSleep();
void goToSleep();

bool playing;
bool sleeping = false;
int songToPlay = 2;

//Plays the start of the mario theme
int mario_tones[] = {660, -1, 660, -1, 660, -1, 520, -1, 660, -1, 781, -1, 395};
int mario_tone_lengths[] = {5000, 1000, 5000, 5000, 5000, 5000, 5000, 1000, 5000, 5000, 7000, 15000, 5000};
int mario_length = sizeof(mario_tones)/sizeof(int);

//Plays a sound
int victory_tones[] = {130, -1, 311, -1, 520, -1, 700, -1, 520, -1, 900};
int victory_tone_lengths[] = {1000, 100, 1000, 100, 1000, 100, 1000, 100, 2000, 100, 5000};
int victory_length = sizeof(victory_tones)/sizeof(int);

//Plays a sound of someone firing a blaster
int fire_tones[] = {700, 600, 500, 400, 300, 200};
int fire_tone_lengths[] = {600, 600, 600, 600, 600, 600};
int fire_length = sizeof(fire_tone_lengths)/sizeof(int);

//Plays "Lisa gikk til skolen"
int lisa_tone_lengths[] = {20692, 6896, 20692, 6896, 20692, 6896, 20692, 6896, 48282, 6896, 48282, 6896, 
 	20692, 6896, 20692, 6896, 20692, 6896, 20692, 6896, 65800, 28027, 20692, 6896, 20692, 6896,
 	20692, 6896, 20692, 6896, 48282, 6896, 48282, 6896, 20692, 6896, 20692, 6896, 20692, 6896, 20692, 6896, 65800, 28027};

int lisa_tones[] = {523,-1, 587,-1, 659,-1, 698,-1, 783,-1, 783,-1, 880,-1, 
 	880,-1, 880,-1, 880,-1, 783,-1, 698,-1, 698,-1, 698,-1, 698, -1,
 	659,-1, 659,-1, 587,-1, 587,-1, 587,-1, 587,-1, 523};

int lisa_length = sizeof(lisa_tone_lengths)/sizeof(int);

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler() {
    *LETIMER0_IFC = 1;
    if (playing) {
    	switch(songToPlay) {
    		case 0:
				continuePlaying(victory_tones, victory_tone_lengths, victory_length);
				break;
			case 1:
				continuePlaying(lisa_tones, lisa_tone_lengths, lisa_length);
				break;
			case 2:
				continuePlaying(mario_tones, mario_tone_lengths, mario_length);
				break;
			case 3:
				continuePlaying(fire_tones, fire_tone_lengths, fire_length);
				break;
    	}
    }
    else {
    	if (!sleeping) {
    		goToSleep();
    	}
    }
}

void __attribute__ ((interrupt)) TIMER1_IRQHandler() {  
    continuePlaying(victory_tones, victory_tone_lengths, victory_length);
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() {
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
	switch ((*GPIO_PC_DIN)) {
		case 0xfe:	//SW2
			songToPlay = 0;
			if (sleeping) {
				wakeFromSleep();
			}
			else if (playing) {
				resetMelody();
			}
			break;
		case 0xbf:	//SW8
			songToPlay = 1;
			if (sleeping) {
				wakeFromSleep();
			}
			else if (playing) {
				resetMelody();
			}
			break;
		case 0xfb:	//SW4
			songToPlay = 2;
			if (sleeping) {
				wakeFromSleep();
			}
			else if (playing) {
				resetMelody();
			}
			break;
		case 0xef:
			songToPlay = 3;	//SW6
			if (sleeping) {
				wakeFromSleep();
			}
			else if (playing) {
				resetMelody();
			}
			break;
	}
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() {
    *GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
	if (sleeping) {
		wakeFromSleep();
	}
	else if (playing) {
		resetMelody();
	}
}

void goToSleep() {
	disableDAC();
	resetMelody();
	disableLETimer();
	playing = false;
	sleeping = true;
}

void wakeFromSleep() {
	enableDAC();
	enableLETimer();
	playing = true;
	sleeping = false;
}