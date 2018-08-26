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

// limits of value
#define MAXIMUM_IDLE_PANEL_CURRENT 1 // 12,500 A
#define MINIMUM_IDLE_PANEL_CURRENT 0
#define MAXIMUM_IDLE_PANEL_VOLTAGE 50200 // 50,2 V
#define MINIMUM_IDLE_PANEL_VOLTAGE 0

#define MAXIMUM_RUNNING_PANEL_CURRENT 11000 // 12,500 A
#define MINIMUM_RUNNING_PANEL_CURRENT 0
#define MAXIMUM_RUNNING_PANEL_VOLTAGE 50200 // 50,2 V
#define MINIMUM_RUNNING_PANEL_VOLTAGE 0
#define MAXIMUM_BATTERY_VOLTAGE 46800 // 51,61 V  maximum value of voltage from battery in VOLTS
#define MINIMUM_BATTERY_VOLTAGE 15000 // 15 V minimum value of voltage from battery in VOLTS

#define CONVERSION_PANEL_CURRENT_VALUE   1    //102
#define CONVERSION_PANEL_VOLTAGE_VALUE   1    //231
#define CONVERSION_BATTERY_VOLTAGE_VALUE 1    //256 //400 

#define ADC_PANEL_VOLTAGE       adc0
#define ADC_PANEL_CURRENT       adc1
#define ADC_BATTERY_VOLTAGE     adc2

#define AVG_PANEL_VOLTAGE        avg_adc0
#define AVG_PANEL_CURRENT        avg_adc1
#define AVG_BATTERY_VOLTAGE      avg_adc2

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
} state_machine_t;

typedef union system_flags{
    struct{
        uint8_t     mppt_on         :1;
        uint8_t     enable          :1;
    };
    uint8_t   all;
} system_flags_t;

typedef union error_flags{
    struct{
        uint8_t     overcurrent :1;
		uint8_t		undercurrent:1;
        uint8_t     overvoltage :1;
		uint8_t		undervoltage:1;
		uint8_t		overvolt_panel:1;
		uint8_t		undervolt_panel:1;
        uint8_t     overheat    :1;
        uint8_t     fault       :1;
        uint8_t     no_canbus   :1;
    };
    uint8_t   all;
}error_flags_t;

typedef struct control{
    uint16_t    D;              // value converted from 0 to TOP
    uint8_t     mppt_pot_limit; //
    uint8_t     fault;          // counts the faults from ir2127
	uint8_t		updown;			// value of comp. in function pertub_and_observe()
    uint16_t    ii[2];          // value of panel current in AMPS
    uint16_t    vi[2];          // value of panel voltage in VOLTS
	uint16_t	vo[2];		    // value of battery voltage in VOLTS
#ifdef ADC_8BITS
	uint16_t	pi[2];			// value of power in WATTS
#else
	uint32_t	pi[2];			// value of power in WATTS
#endif

}control_t;

control_t control;

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
void check_pwm_fault(void);
void check_buffers(void);

// debug functions
void print_system_flags(void);
void print_error_flags(void);
void print_infos(void);
void print_control_others(void);
void print_control_d(void);
void print_control_vi(void);
void print_control_ii(void);
void print_control_vo(void);
void print_control_pi(void);

// machine tasks
void task_initializing(void);
void task_idle(void);
void task_running(void);
void task_error(void);

// the machine itself
void machine_init(void);
void machine_run(void);
void set_state_error(void);
void set_state_initializing(void);
void set_state_idle(void);
void set_state_running(void);

// machine variables
state_machine_t state_machine;
system_flags_t system_flags;
error_flags_t error_flags;
volatile uint8_t machine_clk;
uint8_t total_errors;   // Contagem de ERROS
uint8_t something_changed;

// pwm variables
uint8_t pwm_fault_count;
uint8_t check_pwm_fault_times;
uint8_t init_pwm_increment_divider;

// other variables
uint8_t led_clk_div;

// ISRs
ISR(TIMER2_COMPA_vect);
ISR(PCINT2_vect);

#endif /* ifndef MACHINE_H */
