/**
 * @file machine.h
 *
 * @defgroup MACHINE State Machine Module
 *
 * @brief Implements the main state machine of the system.
 *
 */

#ifndef MACHINE_H
#define MACHINE_H 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "conf.h"

// Equations for mode 2 (CTC with TOP OCR2A)
// Note the resolution. For example.. at 150hz, ICR1 = PWM_TOP = 159, so it
//#define QUOTIENT  (((uint32_t)MACHINE_TIMER_PRESCALER)*((uint32_t)MACHINE_TIMER_FREQUENCY))
//#define MACHINE_TIMER_TOP (0.5*(F_CPU)/QUOTIENT)
#define MACHINE_TIMER_TOP ((F_CPU)/(2.*((uint32_t)MACHINE_TIMER_PRESCALER)*((uint32_t)MACHINE_TIMER_FREQUENCY)) -1)

#ifdef ADC_ON
#include "adc.h"
#endif
#ifdef USART_ON
#include "usart.h"
#endif
#include "dbg_vrb.h"
#ifdef PWM_ON
#include "pwm.h"
#endif
#ifdef CAN_ON
#include "can.h"
#include "can_app.h"
extern const uint8_t can_filter[];
#endif

typedef enum state_machine{
    STATE_INITIALIZING,
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_RESET,
} state_machine_t;

typedef union system_flags{
    struct{
        uint8_t     mppt_on         :1;     // turns on/off via CAN
        uint8_t     enable          :1;     // turns on/off via hardware
    };
    uint8_t   all;
} system_flags_t;

typedef union error_flags{
    struct{
        uint8_t     panel_overvoltage       :1;
        uint8_t     panel_undervoltage      :1;
        uint8_t     panel_overcurrent       :1;
        uint8_t     battery_overvoltage     :1;
        uint8_t     battery_undervoltage    :1;
        uint8_t     battery_overcurrent     :1;
        uint8_t     no_canbus               :1;
    };
    uint8_t   all;
}error_flags_t;

typedef struct control{
    int16_t     D;              // value converted from 0 to TOP
    uint8_t     pi_limit;       // value for an input power limitation controlled by the pilot
	uint8_t		updown;			// value of comp. in function perturb_and_observe()
    uint16_t    vi[2];          // value of panel voltage in VOLTS
    uint16_t    ii[2];          // value of panel current in AMPS
	uint16_t	vo[2];		    // value of battery voltage in VOLTS
    uint16_t    io[2];          // value of battery current in AMPS
	int16_t	    dvi;		    // value of delta panel voltage in VOLTS
	int16_t	    dii;		    // value of delta panel current in AMPS
	int16_t	    dvo;		    // value of delta battery voltage in VOLTS
	int16_t	    dio;		    // value of delta battery current in AMPS
    uint16_t    mpp_vi;         // value of panel voltage at MPP
    uint16_t    mpp_ii;         // value of panel current at MPP
    uint8_t     mpp_D;          // value of duty cycle at MPP
#ifdef ADC_8BITS
	uint16_t	pi[2];			// value of power in WATTS
	uint16_t	po[2];			// value of power in WATTS
	int16_t	    dpi;			// value of delta power in WATTS
    uint16_t    mpp_pi;
#else
	uint32_t	pi[2];			// value of power in WATTS
	uint32_t	po[2];			// value of power in WATTS
	int32_t	    dpi;			// value of delta power in WATTS
    uint32_t    mpp_pi;
#endif

}control_t;


// machine checks
void read_and_check_adcs(void);
void check_initializing_panel_current(void);
void check_initializing_panel_voltage(void);
void check_initializing_battery_voltage(void);
void check_idle_panel_current(void);
void check_idle_panel_voltage(void);
void check_idle_battery_voltage(void);
void check_running_panel_current(void);
void check_running_panel_voltage(void);
void check_running_battery_voltage(void);
void check_buffers(void);

// debug functions
void print_configurations(void);
void print_system_flags(void);
void print_error_flags(void);
void print_infos(void);
void print_control_others(void);
void print_control_d(void);
void print_control_vi(void);
void print_control_dvi(void);
void print_control_ii(void);
void print_control_dii(void);
void print_control_vo(void);
void print_control_dvo(void);
void print_control_pi(void);
void print_control_dpi(void);

// machine tasks
void task_initializing(void);
void task_idle(void);
void task_running(void);
void task_error(void);
void task_reset(void);

// the machine itself
void set_machine_initial_state(void);
void machine_init(void);
void machine_run(void);
void set_state_error(void);
void set_state_initializing(void);
void set_state_idle(void);
void set_state_running(void);
void set_state_reset(void);

// machine variables
volatile state_machine_t state_machine;
volatile system_flags_t system_flags;
volatile error_flags_t error_flags;
volatile control_t control;
volatile uint8_t machine_clk;
volatile uint8_t machine_clk_divider;
volatile uint8_t something_changed;
volatile uint8_t total_errors;           // Contagem de ERROS

// other variables
volatile uint8_t led_clk_div;

// ISRs
ISR(TIMER2_COMPA_vect);
ISR(PCINT2_vect);

#endif /* ifndef MACHINE_H */
