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

    sweep_completed = 0;

#endif
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

inline void sweep(void)
{

    static uint8_t updown = 0;
    static uint8_t periods = 1;
    static uint8_t last_up = 0;

    if(control.pi[0] > control.mpp_pi){
        control.mpp_pi = control.pi[0];
        control.mpp_vi = control.vi[0];
        control.mpp_ii = control.ii[0];
        control.mpp_D = control.D;
    }

    if(updown){
        if(control.D > PWM_D_MIN){
            control.D -= PWM_D_MIN_STEP;
        }else{
            updown ^= 1;
            periods--;
        }
    }else{
        if(control.D < PWM_D_MAX-PWM_D_MIN_STEP){
            control.D += PWM_D_MIN_STEP;
        }else{
            updown ^= 1;
        }
    }
    if(periods == 0)    last_up = 1;

    if(last_up){
        if(control.D < PWM_D_INITIAL){
            control.D += PWM_D_MIN_STEP;
        }else{
            sweep_completed = 1;
            control.D = control.mpp_D;
        }
    }
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

}

/**
 * @brief computs duty-cycle for PWM
 */

inline void pwm_compute(void)
{	
#ifdef MACHINE_ON

    // Compute derivates
    control.pi[0] = (uint32_t)control.vi[0] * (uint32_t)control.ii[0];
    control.dpi = ((int32_t)control.pi[0]) -((int32_t)control.pi[1]);
    control.dvi = ((int32_t)control.vi[0]) -((int32_t)control.vi[1]);

    if(sweep_completed){
        perturb_and_observe();
    }else{
        sweep();
    }

    VERBOSE_MSG_PWM(usart_send_string("PeO. "));
	
	// treats faults
    /*if(error_flags.overvoltage || error_flags.overcurrent){
        VERBOSE_MSG_PWM(usart_send_string("Err. "));
        error_flags.overvoltage = error_flags.overcurrent = 0;
        if(control.D >= (D_STEP+1))      control.D -= (D_STEP+1);
        else                    control.D = 0;
    }*/

    // apply some threshhold saturation limits
    if(control.D > PWM_D_MAX)        control.D = PWM_D_MAX;
    else if(control.D < PWM_D_MIN)   control.D = PWM_D_MIN;

    // apply dutycycle
    VERBOSE_MSG_PWM(usart_send_string("c.D. "));
    set_pwm_duty_cycle(control.D);

    VERBOSE_MSG_PWM(usart_send_string("PWM computed as: "));
    VERBOSE_MSG_PWM(usart_send_uint16(OCR1A));
    VERBOSE_MSG_PWM(usart_send_char('\n'));


    // recycles
    control.pi[1] = control.pi[0];
    control.vi[1] = control.vi[0];
    control.ii[1] = control.ii[0];
    control.vo[1] = control.vo[0];


#endif
}

/**
 * @brief decreases pwm by 10% in case of mosfet fault detected by IR2127.
 */
inline void pwm_treat_fault(void)
{
    /*
#ifdef MACHINE_ON
    if(control.D_raw_target > 10)
        control.D_raw_target -= 6;      // -10%
#endif
        */
    /*
    if(OCR1A > 10)
        OCR1A -= 6;
    VERBOSE_MSG_PWM(usart_send_string("\nPWM fault treated\n"));
    VERBOSE_MSG_ERROR(usart_send_string("\nPWM fault treated\n"));
    */
}

