/*
 * mppt.h
 *
 * Created: 15/12/2017 19:20:56
 *  Author: jack
 */ 


#ifndef MPPT_H_
#define MPPT_H_

#include "../lib/cbuf.h"
#include "conf.h"
#ifdef MACHINE_ON
#include "machine.h"
#endif
#ifdef ADC_ON
#include "adc.h"
#endif

#ifdef ENABLE_ZERO_POWER_DETECTION
uint8_t zero_power_detection_counter;
void zero_power_detection(void);
#endif // ENABLE_ZERO_POWER_DETECTION 

#ifdef ENABLE_SWEEP
uint8_t sweep_completed;
void sweep(void);
#endif // ENABLE_SWEEP

#ifdef ENABLE_PERTURB_AND_OBSERVE
void perturb_and_observe(void);
#endif // ENABLE_PERTURB_AND_OBSERVE 

#ifdef ENABLE_SOFT_START
uint8_t soft_start_completed;
#endif // ENABLE_SOFT_START

#endif /* MPPT_H_ */
