#include "pwm.h"

/**
 * @brief configura o PWM usando o timer TC1
 */
void pwm_init()
{
#ifdef PWM_ON
    /* OLD CONFIGURATION
  	TCCR1A |= 0b10000010;
  	TCCR1B |= 0b00010001;
    ICR1   = 80;                                    // valor TOP para f_pwm = 100kHz
    OCR1A  = INITIAL_D;                             // D = %*ICR1
    set_bit(PWM_DDR, PWM);                          // PWM como saida
    */
      
    clr_bit(PRR0, PRTIM1);                          // Activates clock
    
    // MODE 14 -> Fast PWM with TOP on ICR1
    TCCR1A  =    (1 << WGM11) | (0 << WGM10)        // mode 14
            | (0 << COM1B1) | (0 << COM1B0)         // do nothing
            | (1 << COM1A1) | (0 << COM1A0);        // pwm

    TCCR1B =    (0 << CS12) | (0 << CS11) | (1 << CS10) // Prescaler N=1
            | (1 << WGM13) | (1 << WGM12);          // mode 14
    
    ICR1 = 159;                                     // ICR1 = TOP = fcpu/(N*f) -1
    OCR1A = INITIAL_D;                              // set initial Duty Cycle
    set_bit(PWM_DDR, PWM);                          // PWM as output
    
    // note that the resolution is R = log10(TOP +1)/log10(2);
    // for IRC1 = 159, it has 7.3 bits of resolution.

#endif
}

/**
 * @brief reset pwm and its control buffers.
 */
inline void pwm_reset(void)
#include <util/delay.h>
{
    set_pwm_off();
#ifdef MACHINE_ON
    control.D = 0;
#endif
    VERBOSE_MSG_PWM(usart_send_string("PWM turned off!\n"));
}


/**
 * @brief computs duty-cycle for PWM
 */

inline void pwm_compute(void)
{	
#ifdef MACHINE_ON
    if(adc_data_ready){
	    pertub_and_observe();
        //adc_data_ready = 0;
        VERBOSE_MSG_PWM(usart_send_string("PeO. "));
    }
	
	// treats faults
    if(error_flags.overvoltage || error_flags.overcurrent){
        VERBOSE_MSG_PWM(usart_send_string("Err. "));
        error_flags.overvoltage = error_flags.overcurrent = 0;
        if(control.D >= (D_STEP+1))      control.D -= (D_STEP+1);
        else                    control.D = 0;
    }

    // apply some threshhold saturation limits
    if(control.D > PWM_D_MAX_THRESHHOLD)        control.D = PWM_D_MAX;
    else if(control.D < PWM_D_MIN_THRESHHOLD)   control.D = PWM_D_MIN;

    // apply dutycycle
    if(adc_data_ready){
        VERBOSE_MSG_PWM(usart_send_string("c.D. "));
        set_pwm_duty_cycle(control.D);
        adc_data_ready = 0;
    }

    VERBOSE_MSG_PWM(usart_send_string("PWM computed as: "));
    VERBOSE_MSG_PWM(usart_send_uint16(OCR1A));
    VERBOSE_MSG_PWM(usart_send_char('\n'));
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
    if(OCR1A > 10)
        OCR1A -= 6;
    VERBOSE_MSG_PWM(usart_send_string("PWM fault treated\n"));
}

