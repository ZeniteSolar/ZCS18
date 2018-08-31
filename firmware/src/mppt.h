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

void perturb_and_observe(void);
void sweep(void);

#endif /* MPPT_H_ */
