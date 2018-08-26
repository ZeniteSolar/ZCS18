/**
 * @file pwm.h
 *
 * @defgroup PWM Module
 *
 * @brief This module implements a simple PWM
 *
 */

#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "conf.h"
#include "dbg_vrb.h"
#include "PeO.h"
#include "../lib/cbuf.h"
#ifdef USART_ON
#include "usart.h"
#endif
#ifdef MACHINE_ON
#include "machine.h"
#endif

// DEFAULT CONFIGURATION. conf.h should overwrite it
#ifndef INITIAL_D
#define INITIAL_D       0
#endif
#ifndef PWM_FREQUENCY
#define PWM_FREQUENCY   1000
#endif
#ifndef PWM_PRESCALE
#define PWM_PRESCALE    1
#endif

// Equations for mode 14 (Fast PWM with TOP in ICR1)
// Note the resolution. For example.. at 100Khz, ICR1 = PWM_TOP = 159, so it has 7.3 bits of resolution.
#define PWM_TOP ((F_CPU)/(((uint32_t)PWM_PRESCALE)*((uint32_t)PWM_FREQUENCY)) -1)
#define PWM_RESOLUTION log10(PWM_TOP +1)/log10(2)

// // pwm macros
#define set_pwm_duty_cycle(d)       OCR1A = d      //!< apply duty cycle 'd'
#define set_pwm_off()               OCR1A = 0;     //!< d = 0

// pwm functions
void pwm_init(void);
void pwm_reset(void);
void pwm_compute(void);
void pwm_treat_fault(void);
uint8_t pwm_zero_width(uint16_t duty_cycle);

// // pwm variables
uint8_t pwm_d_clk_div;

#endif /* ifndef PWM_H */
