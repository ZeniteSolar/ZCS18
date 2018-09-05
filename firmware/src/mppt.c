/*
 * mppt.c
 */ 
#include "mppt.h"


/**
 * @brief simple perturb and observe
 * ref: J. Ahmed, Z. Salam / Applied Energy 150 (2015) 97–108
 * doi:10.1016/j.apenergy.2015.04.006
 */
inline void perturb_and_observe(void)
{   
    static int8_t d_step = PWM_D_STEP;
    
    if(control.dpi <= 0)    d_step = -d_step;
    control.D += d_step;

}

/**
 * @brief sweep D over full range to find MPP
 */
void sweep(void)
{
    if(control.pi[0] > control.mpp_pi){
        control.mpp_pi = control.pi[0];
        control.mpp_vi = control.vi[0];
        control.mpp_ii = control.ii[0];
        control.mpp_D = control.D;
    }

    if(sweep_updown){
        if(control.D > PWM_D_MIN){
            control.D -= PWM_D_MIN_STEP;
        }else{
            sweep_updown ^= 1;
            sweep_periods--;
        }
    }else{
        if(control.D < PWM_D_MAX-PWM_D_MIN_STEP){
            control.D += PWM_D_MIN_STEP;
        }else{
            sweep_updown ^= 1;
        }
    }
    if(sweep_periods == 0)    sweep_last_up = 1;

    if(sweep_last_up){
        if(control.D < PWM_D_INITIAL){
            control.D += PWM_D_MIN_STEP;
        }else{
#ifdef ENABLE_SWEEP
            sweep_completed = 1;
#endif // ENABLE_SWEEP
#ifndef ENABLE_SOFT_START
            control.D = control.mpp_D;
#endif // ENABLE_SOFT_START
        }
    }
#ifdef ENABLE_SWEEP
    if(sweep_completed){
        VERBOSE_MSG_MACHINE(usart_send_string("\n\nGOT MPPT point: "));
        VERBOSE_MSG_MACHINE(usart_send_uint32(control.mpp_pi));
        VERBOSE_MSG_MACHINE(usart_send_char(' '));
        VERBOSE_MSG_MACHINE(usart_send_uint16(control.mpp_vi));
        VERBOSE_MSG_MACHINE(usart_send_char(' '));
        VERBOSE_MSG_MACHINE(usart_send_uint16(control.mpp_ii));
        VERBOSE_MSG_MACHINE(usart_send_char(' '));
        VERBOSE_MSG_MACHINE(usart_send_uint8(control.mpp_D)); 
        VERBOSE_MSG_MACHINE(usart_send_char('\n'));
    }
#endif // ENABLE_SWEEP

}

