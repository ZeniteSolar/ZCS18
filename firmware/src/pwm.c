#include "pwm.h"

/**
 * @brief configura o PWM usando o timer TC1
 */
void pwm_init()
{
#ifdef PWM_ON
      
    clr_bit(PRR0, PRTIM1);                          // Activates clock
    
    // MODE 14 -> Fast PWM with TOP on ICR1
    TCCR1A  =    (1 << WGM11) | (0 << WGM10)        // mode 14
            | (0 << COM1B1) | (0 << COM1B0)         // do nothing
            | (1 << COM1A1) | (0 << COM1A0);        // pwm

    TCCR1B =
#if PWM_PRESCALE ==     1
            (0 << CS12) | (0 << CS11) | (1 << CS10) // Prescaler N=1
#elif PWM_PRESCALE ==   8
            (0 << CS12) | (0 << CS11) | (1 << CS10) // Prescaler N=8
#elif PWM_PRESCALE ==   64
            (0 << CS12) | (1 << CS11) | (0 << CS10) // Prescaler N=64
#elif PWM_PRESCALE ==   256
            (0 << CS12) | (1 << CS11) | (1 << CS10) // Prescaler N=256
#elif PWM_PRESCALE ==   1024
            (1 << CS12) | (0 << CS11) | (1 << CS10) // Prescaler N=1024
#else 
            0
#endif
            | (1 << WGM13) | (1 << WGM12);          // mode 14
    
    ICR1 = PWM_TOP;                                 // ICR1 = TOP = fcpu/(N*f) -1
    OCR1A = PWM_D_INITIAL;                              // set initial Duty Cycle
    set_bit(PWM_DDR, PWM);                          // PWM as output

#ifdef ENABLE_SWEEP
    sweep_completed = 0;
#endif // ENABLE_SWEEP
#ifdef ENABLE_ZERO_POWER_DETECTION
    zero_power_detection_counter = 0;
#endif // ENABLE_ZERO_POWER_DETECTION 

#endif // PWM_ON
}

/**
 * @brief reset pwm and its control buffers.
 */
inline void pwm_reset(void)
{
    set_pwm_off();
#ifdef MACHINE_ON
    control.D = 0;
#endif
    VERBOSE_MSG_PWM(usart_send_string("PWM turned off!\n"));
}

/**
 * @brief 
 */
inline void zero_power_detection(void)
{
#ifdef ENABLE_ZERO_POWER_DETECTION
    if(control.pi[0] < RUNNING_PANEL_POWER_MIN){
        if(zero_power_detection_counter++ >= ZERO_POWER_DETECTION_THRESHOLD){
            zero_power_detection_counter = 0;
#ifdef ENABLE_SWEEP
            sweep_completed = 0;
            sweep_periods = (PERIODS_TO_SWEEP -1);
            sweep_last_up = 0;
            sweep_updown = 0;
#endif // ENABLE_SWEEP 
#ifdef ENABLE_SOFT_START
            soft_start_completed = 0;
#endif // ENABLE_SOFT_START 
            control.D = PWM_D_MIN;
            VERBOSE_MSG_MACHINE(usart_send_string("\n>>> ZERO POWER DETECTED!\n"));
        }
    }else{
        zero_power_detection_counter = 0;
    }
#endif //ENABLE_ZERO_POWER_DETECTION
}

/**
 * @brief computs duty-cycle for PWM
 */
void pwm_compute(void)
{	
#ifdef MACHINE_ON
    // Compute derivates
    control.pi[0] = (uint32_t)(uint32_t)control.vi[0] * (uint32_t)control.ii[0];
    control.dpi = (uint32_t)((int32_t)control.pi[0]) -((int32_t)control.pi[1]);
    control.dvi = (uint32_t)((int32_t)control.vi[0]) -((int32_t)control.vi[1]);


    #ifdef ENABLE_SWEEP
    #ifdef ENABLE_SOFT_START

    if(sweep_completed){
        if(!soft_start_completed){
            if(control.D < control.mpp_D){
                control.D++;
            }else{
                soft_start_completed = 1;
                VERBOSE_MSG_MACHINE(usart_send_string("\n>>> SOFT START COMPLETED\n"));
            }
        }else{
            #ifdef ENABLE_PERTURB_AND_OBSERVE
        perturb_and_observe();
            #endif // ENABLE_PERTURB_AND_OBSERVE
            #ifdef ENABLE_ZERO_POWER_DETECTION
        zero_power_detection();
            #endif //ENABLE_ZERO_POWER_DETECTION
        }
    }else{
        sweep();
    }
    #endif // ENABLE_SOFT_START
    #endif // ENABLE_SWEEP

    #ifdef ENABLE_SWEEP
    #ifndef ENABLE_SOFT_START

    if(sweep_completed){
            #ifdef ENABLE_PERTURB_AND_OBSERVE
    perturb_and_observe();
            #endif // ENABLE_PERTURB_AND_OBSERVE
            #ifdef ENABLE_ZERO_POWER_DETECTION
    zero_power_detection();
            #endif //ENABLE_ZERO_POWER_DETECTION 
    }else{
        sweep();
    }
    #endif // ENABLE_SOFT_START
    #endif // ENABLE_SWEEP

    #ifndef ENABLE_SWEEP
    #ifdef ENABLE_SOFT_START
    if(!soft_start_completed){
        if(control.D < PWM_D_NOMINAL){
            control.D++;
        }else{
            soft_start_completed = 1;
            VERBOSE_MSG_MACHINE(usart_send_string("\n>>> SOFT START COMPLETED\n"));
        }
    }else{
            #ifdef ENABLE_PERTURB_AND_OBSERVE
        perturb_and_observe();
            #endif // ENABLE_PERTURB_AND_OBSERVE
            #ifdef ENABLE_ZERO_POWER_DETECTION
        zero_power_detection();
            #endif //ENABLE_ZERO_POWER_DETECTION        
    }
    #endif // ENABLE_SOFT_START
    #endif // ENABLE_SWEEP

    #ifndef ENABLE_SWEEP
    #ifndef ENABLE_SOFT_START
        #ifdef ENABLE_PERTURB_AND_OBSERVE
    perturb_and_observe();
        #endif // ENABLE_PERTURB_AND_OBSERVE
        #ifdef ENABLE_ZERO_POWER_DETECTION
    zero_power_detection();
        #endif //ENABLE_ZERO_POWER_DETECTION         
    #endif // ENABLE_SOFT_START
    #endif // ENABLE_SWEEP


    pwm_limit();
    set_pwm_duty_cycle(control.D);

    VERBOSE_MSG_PWM(usart_send_string("PWM computed as: "));
    VERBOSE_MSG_PWM(usart_send_uint16(OCR1A));
    VERBOSE_MSG_PWM(usart_send_char('\n'));

    control.io[0] = control.ii[0] * (((PWM_TOP)-control.D)/control.D);
    control.po[0] = (uint32_t)control.vo[0] * (uint32_t)control.io[0]; 

    // recycles
    control.pi[1] = control.pi[0];
    control.vi[1] = control.vi[0];
    control.ii[1] = control.ii[0];
    control.vo[1] = control.vo[0];
    control.io[1] = control.io[0];
    control.po[1] = control.po[0];

#endif // MACHINE_ON
}

/**
 * @brief apply some threshhold saturation limits 
 */
inline void pwm_limit(void)
{
    if(control.D > PWM_D_MAX)        control.D = PWM_D_MAX;
    else if(control.D < PWM_D_MIN)   control.D = PWM_D_MIN;
}


