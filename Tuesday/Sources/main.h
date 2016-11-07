#pragma once
#ifndef MAINH
#define MAINH

extern void DoClock(int state);
extern volatile int measured;

enum
{
	ADC_INX,
	ADC_INY,
	ADC_INTENSITY,
	ADC_TEMPO,
	ADC_Count
};

extern int adcchannels[ADC_Count];
extern void doTimer();
extern void PatternReset();

extern volatile int i2csending;
extern volatile int i2creceiving;
#endif
