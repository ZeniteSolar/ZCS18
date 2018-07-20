/*
 * PeO.h
 *
 * Created: 15/12/2017 19:20:56
 *  Author: jack
 */ 


#ifndef PEO_H_
#define PEO_H_

#include "../lib/cbuf.h"
#include "conf.h"
#ifdef MACHINE_ON
#include "machine.h"
#endif
#ifdef ADC_ON
#include "adc.h"
#endif
#define D_STEP 1

void pertub_and_observe(void);

#endif /* PEO_H_ */
