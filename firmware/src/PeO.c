/*
 * PeO.c
 *
 * Created: 15/12/2017 19:18:18
 *  Author: jack
 */ 
#include "PeO.h"
/**
 * @brief P&O algorithm
 */
inline void pertub_and_observe(void)
{	
#ifdef MACHINE_ON

	// Computes power input
	control.pi[0] = (uint32_t)control.vi[0] * (uint32_t)control.ii[0];
	control.pi[1] = (uint32_t)control.vi[1] * (uint32_t)control.ii[1];

    // Derivate power
	int32_t dpi = ((int32_t)control.pi[0]) -((int32_t)control.pi[1]);
    // Derivate voltage
    int32_t dvi = ((int32_t)control.vi[0]) -((int32_t)control.vi[1]);


#ifdef CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
    // WARNING: DEFINITIONS FOR TEST THE CONVERTER WITH FIXED DUTY CYCLE!!!
    control.D = CONVERTER_TEST_WITH_FIXED_DUTYCYCLE_DT_VALUE;
#else
 	if(dpi){
     	uint8_t c = 2*D_STEP   +2*control.vi[0]/(1+control.ii[0]);
     	//uint8_t c = D_STEP;
		if(dpi < 0){
		    control.updown ^= 1;
            if(dvi > 0){
                control.D += c;
            }else{
                control.D -= c;
            }
        }else{
            if(dvi > 0){
                control.D -= c;
            }else{
                control.D += c;
            }
        }
	}
#endif

	// recycles
    //if(control.pi[0] != control.pi[1])    control.pi[1] = control.pi[0];
    if(control.vi[0] != control.vi[1])
        control.vi[1] = control.vi[0];
    //if(control.ii[0] != control.ii[1])
        control.ii[1] = control.ii[0];
    //if(control.vo[0] != control.vo[1])
        control.vo[1] = control.vo[0];
#endif
}
