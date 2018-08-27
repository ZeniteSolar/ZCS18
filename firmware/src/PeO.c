/*
 * PeO.c
 */ 
#include "PeO.h"
/**
 * @brief P&O modified algorithm 
 */
inline void pertub_and_observe(void)
{	
#ifdef MACHINE_ON

#ifdef FORCE_VARIATION_OF_D_WHEN_ZERO_POWER_DETECTED
    static uint8_t zero_power_counter = 0;    //!< record how many iterations without power
#endif  //FORCE_VARIATION_OF_D_WHEN_ZERO_POWER_DETECTED

	// Computes power input
	control.pi[0] = (uint32_t)control.vi[0] * (uint32_t)control.ii[0];

    // Derivate power
	control.dpi = ((int32_t)control.pi[0]) -((int32_t)control.pi[1]);
    // Derivate voltage
    control.dvi = ((int32_t)control.vi[0]) -((int32_t)control.vi[1]);

#ifdef CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
    // WARNING: DEFINITIONS FOR TEST THE CONVERTER WITH FIXED DUTY CYCLE!!!
    control.D = CONVERTER_TEST_WITH_FIXED_DUTYCYCLE_DT_VALUE;
#else
#ifdef DYNAMIC_D_STEP_SIZE
    int8_t d_step = PWM_D_MIN_STEP +(control.vi[0]/(1+control.ii[0]) >>2); // 6 is 10% of D_MAX
    if(d_step > PWM_D_MAX_STEP) d_step = PWM_D_MAX_STEP;
    if(d_step < PWM_D_MIN_STEP) d_step = PWM_D_MIN_STEP;
#else
    int8_t d_step = PWM_D_MIN_STEP;
#endif //DYNAMIC_D_STEP_SIZE
 	if(control.dpi != 0){
    //if(1){
#ifdef FORCE_VARIATION_OF_D_WHEN_ZERO_POWER_DETECTED
        zero_power_counter = 0;
#endif  //FORCE_VARIATION_OF_D_WHEN_ZERO_POWER_DETECTED
		if(control.dpi > 0){            //<<<
            if(control.dvi < 0){
                control.D += d_step;
            }else{
                control.D -= d_step;
            }
        }else if(control.dpi < 0){
            if(control.dvi < 0){        //>>>
                control.D -= d_step;
            }else{
                control.D += d_step;
            }
        }
	}else{
#ifdef FORCE_VARIATION_OF_D_WHEN_ZERO_POWER_DETECTED
        if(!control.pi[0]){
            if(zero_power_counter <= MAX_ZERO_POWER_TIMES){
                zero_power_counter++;
            }else if((control.D >= PWM_D_MAX-D_STEP) || (control.D <= PWM_D_MIN+D_STEP)){
                control.updown ^= 1;
                d_step = control.updown? PWM_D_MIN_STEP : -PWM_D_MIN_STEP;
                control.D += d_step;
            }
        }
#endif //FORCE_VARIATION_OF_D_WHEN_ZERO_POWER_DETECTED
    }
#endif //CONVERTER_TEST_WITH_FIXED_DUTYCYCLE 

	// recycles
    control.pi[1] = control.pi[0];
    control.vi[1] = control.vi[0];
    control.ii[1] = control.ii[0];
    control.vo[1] = control.vo[0];
#endif //MACHINE_ON
}
