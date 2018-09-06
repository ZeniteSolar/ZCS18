/*
 * mppt.c
 */ 
#include "mppt.h"

/**
 * @brief tries to limit input power to safe levels varying duty cycle against
 * MPP
 */
inline void panel_power_limit(void)
{
    if(control.pi[0] > RUNNING_PANEL_POWER_MAX){
        VERBOSE_MSG_MACHINE(usart_send_string("\n\n>>> PANEL POWER LIMIT!\n"));
        if(control.pi[0] < control.pi[1])
            control.D += 2*control.D_step;
        else
            control.D -= 2*control.D_step;
    }
}

/**
 * @brief tries to limit battery voltage to its maximum varying duty cycle
 */
inline void battery_voltage_limit(void)
{
    if(control.vo[0] > RUNNING_BATTERY_VOLTAGE_MAX){
        VERBOSE_MSG_MACHINE(usart_send_string("\n\n>>> BATTERY VOLTAGE LIMIT!\n"));
        if(control.vo[0] < control.vo[1])
            control.D += 4*control.D_step;
        else
            control.D -= 4*control.D_step;
    }
}

/**
 * @brief simple perturb and observe
 * ref: J. Ahmed, Z. Salam / Applied Energy 150 (2015) 97–108
 * doi:10.1016/j.apenergy.2015.04.006
 */
inline void perturb_and_observe(void)
{   
    if(control.dpi <= 0)    control.D_step = -control.D_step;
    control.D += control.D_step;
}

/**
 * @brief sweep D over full range to find MPP
 */
void sweep(void)
{
#ifdef ENABLE_SWEEP
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

#endif // ENABLE_SWEEP
}

