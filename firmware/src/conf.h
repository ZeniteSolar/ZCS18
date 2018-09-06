/**
 * @file conf.h
 *
 * @defgroup CONF Configurations 
 *
 * @brief General configuration of the system.
 *
 */

#ifndef CONF_H
//#define CONF_H

#ifndef F_CPU
#define F_CPU 16000000UL    //define a frequencia do microcontrolador - 16MHz
#endif /* ifndef F_CPU */


// CONFIGURACOES DE COMPILACAO
//#define DEBUG_ON
//#define VERBOSE_ON
//#define VERBOSE_ON_CAN_APP
#define VERBOSE_ON_MACHINE
//#define VERBOSE_ON_ADC
//#define VERBOSE_ON_PWM
#define VERBOSE_ON_INIT
#define VERBOSE_ON_ERROR


// MODULES ACTIVATION
#define USART_ON
//#define CAN_ON
//#define CAN_DEPENDENT
#define ADC_ON
#define MACHINE_ON
#define PWM_ON
#define LED_ON
#define WATCHDOG_ON
//#define SLEEP_ON


// PWM DEFINITIONS
#ifdef PWM_ON
// WARNING: DEFINITIONS FOR TEST THE CONVERTER WITH FIXED DUTY CYCLE!!!
//#define CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
//#define CONVERTER_TEST_WITH_FIXED_DUTYCYCLE_DT_VALUE 0.60*(PWM_TOP)
//#define PWM_TEST 
//#define PEO_TEST

#define PWM_FREQUENCY                       99000          //<! pwm frequency in Hz
#define PWM_D_NOMINAL                       0.6*(PWM_TOP)   //!< float value from 0 to 1
#define PWM_D_MIN                           0.2*(PWM_TOP)  //!< minimum D
#define PWM_D_MAX                           0.96*(PWM_TOP)   //!< maximum D
#define PWM_D_STEP 					        1
#define PWM_D_MIN_STEP				        1*PWM_D_STEP
#define PWM_D_MAX_STEP				        4*PWM_D_STEP

#endif // PWM_ON

#ifdef ADC_ON
// ADC CONFIGURATION
#define ADC_FREQUENCY                       20000
#define ADC_TIMER_PRESCALER                 64
#define ADC_PANEL_VOLTAGE                   adc0
#define ADC_PANEL_CURRENT                   adc1
#define ADC_BATTERY_VOLTAGE                 adc2
#define AVG_PANEL_VOLTAGE                   avg_adc0
#define AVG_PANEL_CURRENT                   avg_adc1
#define AVG_BATTERY_VOLTAGE                 avg_adc2
#define ADC_NOISE_VALUE                     10
#define ADC_PANEL_VOLTAGE_ANGULAR_COEF      49279 //(40000/((4/5)*1024))
//#define ADC_PANEL_VOLTAGE_LINEAR_COEF       0
#define ADC_PANEL_CURRENT_ANGULAR_COEF      16985 //(16000/(((16*200*1500e-6)/5)*1024))
//#define ADC_PANEL_CURRENT_LINEAR_COEF       0
#define ADC_BATTERY_VOLTAGE_ANGULAR_COEF    64962 //~(60000/1024)
//#define ADC_BATTERY_VOLTAGE_LINEAR_COEF     0

#define FAKE_ADC_ON
#ifdef FAKE_ADC_ON
#define FAKE_ADC0_VALUE                     100
#define FAKE_ADC1_VALUE                     100
#define FAKE_ADC2_VALUE                     200
#endif // FAKE_ADC_ON

#endif //ADC_ON

#ifdef MACHINE_ON
#define MACHINE_TIMER_FREQUENCY             120     //<! machine timer frequency in Hz
#define MACHINE_TIMER_PRESCALER             1024    //<! machine timer prescaler
#define MACHINE_CLK_DIVIDER_VALUE           4       //<! machine_run clock divider
#define MACHINE_FREQUENCY                   (MACHINE_TIMER_FREQUENCY)/(MACHINE_CLK_DIVIDER_VALUE)

// MPPT ALGORITHMS
#define ENABLE_SWEEP
#define ENABLE_PERTURB_AND_OBSERVE
#define ENABLE_SOFT_START
#define ENABLE_ZERO_POWER_DETECTION
#ifdef ENABLE_ZERO_POWER_DETECTION
#define ZERO_POWER_DETECTION_THRESHOLD		100
#endif // ENABLE_ZERO_POWER_DETECTION
#ifdef ENABLE_SWEEP
#define PERIODS_TO_SWEEP                    2
#endif // ENABLE_SWEEP

// SAFETY ALGORITHMS
//#define ENABLE_PANEL_POWER_LIMIT
#define ENABLE_SOFTWARE_BATTERY_VOLTAGE_LIMIT

// NAIVE IO AND PO COMPUTATION BASED ON D and PANEL CURRENT
#define ENABLE_IO_COMPUTATION
#define ENABLE_PO_COMPUTATION

#ifdef CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
#undef ENABLE_SOFT_START
#undef ENABLE_ZERO_POWER_DETECTION
#undef ENABLE_SWEEP
#endif // CONVERTER_TEST_WITH_FIXED_DUTYCYCLE

// SCALE TO CONVERT ADC DEFINITIONS
#define VSCALE                              (uint16_t)1000
#define ISCALE                              (uint16_t)1000
#define PSCALE                              (uint32_t)(VSCALE)*(ISCALE)

// BATTERY DEFINITIONS
#define BATTERY_CELL_NOMINAL_VOLTAGE        12.0    *(VSCALE)
#define BATTERY_CELL_FAST_CHARGE_VOLTAGE    15.6    *(VSCALE)
#define BATTERY_CELL_NORMAL_CHARGE_VOLTAGE  13.8    *(VSCALE)
#define BATTERY_CELL_DISCHARGED_VOLTAGE     7.00    *(VSCALE)
#define BATTERY_BANK_SERIES_CELL            3
#define BATTERY_BANK_NOMINAL_VOLTAGE        (uint32_t)(BATTERY_BANK_SERIES_CELL)*(BATTERY_CELL_NOMINAL_VOLTAGE)
#define BATTERY_BANK_FAST_CHARGE_VOLTAGE    (uint32_t)(BATTERY_BANK_SERIES_CELL)*(BATTERY_CELL_FAST_CHARGE_VOLTAGE)
#define BATTERY_BANK_NORMAL_CHARGE_VOLTAGE  (uint32_t)(BATTERY_BANK_SERIES_CELL)*(BATTERY_CELL_NORMAL_CHARGE_VOLTAGE)
#define BATTERY_BANK_DISCHARGED_VOLTAGE     (uint32_t)(BATTERY_BANK_SERIES_CELL)*(BATTERY_CELL_DISCHARGED_VOLTAGE)

// PANEL DEFINITIONS
#define PANEL_MODEL_NAME                    JKM260P
#define PANEL_STC_OPEN_VOLTAGE              38.1    *(VSCALE)
#define PANEL_STC_SHORT_CURRENT             8.98    *(ISCALE)
#define PANEL_STC_MPP_VOLTAGE               31.1    *(VSCALE)
#define PANEL_STC_MPP_CURRENT               8.37    *(ISCALE)
#define PANEL_STC_MPP_POWER                 260.    *(PSCALE)
#define PANEL_NOTC_OPEN_VOLTAGE             35.2    *(VSCALE)
#define PANEL_NOTC_SHORT_CURRENT            7.31    *(ISCALE)
#define PANEL_NOTC_MPP_VOLTAGE              28.7    *(VSCALE)
#define PANEL_NOTC_MPP_CURRENT              7.31    *(ISCALE)
#define PANEL_NOTC_MPP_POWER                193.    *(PSCALE)

// MPPT DEFINITIONS
#define NOT_RUNNING_PANEL_VOLTAGE_MAX       1.25*(PANEL_STC_OPEN_VOLTAGE)    
#define NOT_RUNNING_PANEL_VOLTAGE_MIN       (ADC_NOISE_VALUE)*(ADC_PANEL_VOLTAGE_ANGULAR_COEF >> 10)
#define NOT_RUNNING_PANEL_CURRENT_MAX       (ADC_NOISE_VALUE)*(ADC_PANEL_CURRENT_ANGULAR_COEF >> 10)
#define NOT_RUNNING_PANEL_CURRENT_MIN       (ADC_NOISE_VALUE)*(ADC_PANEL_CURRENT_ANGULAR_COEF >> 10)
#define NOT_RUNNING_BATTERY_VOLTAGE_MAX     (BATTERY_BANK_FAST_CHARGE_VOLTAGE)
#define NOT_RUNNING_BATTERY_VOLTAGE_MIN     0.75*(BATTERY_BANK_DISCHARGED_VOLTAGE)
#define NOT_RUNNING_BATTERY_CURRENT_MAX     (ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)
#define NOT_RUNNING_BATTERY_CURRENT_MIN     (ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)
#define NOT_RUNNING_PANEL_POWER_MAX         (ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)*(ADC_PANEL_VOLTAGE_ANGULAR_COEF >> 10)*(ADC_PANEL_CURRENT_ANGULAR_COEF >> 10)
#define NOT_RUNNING_PANEL_POWER_MIN         (ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)*(ADC_PANEL_VOLTAGE_ANGULAR_COEF >> 10)*(ADC_PANEL_CURRENT_ANGULAR_COEF >> 10)
#define NOT_RUNNING_BATTERY_POWER_MAX       (ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)
#define NOT_RUNNING_BATTERY_POWER_MIN       (ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)

#define RUNNING_PANEL_VOLTAGE_MAX    		1.25*(PANEL_STC_OPEN_VOLTAGE)    
#define RUNNING_PANEL_VOLTAGE_MIN    		(ADC_NOISE_VALUE)*(ADC_PANEL_VOLTAGE_ANGULAR_COEF >> 10)
#define RUNNING_PANEL_CURRENT_MAX    		1.25*(PANEL_STC_SHORT_CURRENT)
#define RUNNING_PANEL_CURRENT_MIN    		(ADC_NOISE_VALUE)*(ADC_PANEL_CURRENT_ANGULAR_COEF >> 10) 
#define RUNNING_BATTERY_VOLTAGE_MAX  		1.1*(BATTERY_BANK_FAST_CHARGE_VOLTAGE)
#define RUNNING_BATTERY_VOLTAGE_MIN  		(BATTERY_BANK_DISCHARGED_VOLTAGE)
#define RUNNING_BATTERY_CURRENT_MAX  		1.25*(((PWM_TOP-control.D)/control.D)*control.ii)
#define RUNNING_BATTERY_CURRENT_MIN  	    0.75*(((PWM_TOP-control.D)/control.D)*control.ii)
#define RUNNING_PANEL_POWER_MAX      		1.15*(PANEL_STC_MPP_POWER)
#define RUNNING_PANEL_POWER_MIN      		(ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)*(ADC_PANEL_VOLTAGE_ANGULAR_COEF >> 10)*(ADC_PANEL_CURRENT_ANGULAR_COEF >> 10) 
#define RUNNING_BATTERY_POWER_MAX    		1.15*(PANEL_STC_MPP_POWER)
#define RUNNING_BATTERY_POWER_MIN    		(ADC_NOISE_VALUE)*(ADC_NOISE_VALUE)*(ADC_PANEL_VOLTAGE_ANGULAR_COEF >> 10)*(ADC_PANEL_CURRENT_ANGULAR_COEF >> 10) 

#define ENABLE_HARDWARE_OVERVOLTAGE_INTERRUPT
#define ENABLE_HARDWARE_ENABLE_SWITCH_INTERRUPT

//#define CHECK_INITIALIZING_CONDITIONS
//#define CHECK_IDLE_CONDITIONS
//#define CHECK_RUNNING_CONDITIONS

// INPUT PINS DEFINITIONS
#ifdef ENABLE_HARDWARE_OVERVOLTAGE_INTERRUPT  
#define     BatOverVoltageInterrupt_PORT PORTD
#define     BatOverVoltageInterrupt_PIN  PIND
#define     BatOverVoltageInterrupt_DDR  DDRD
#define     BatOverVoltageInterrupt      PD2
#endif // ENABLE_HARDWARE_OVERVOLTAGE_INTERRUPT

#ifdef ENABLE_HARDWARE_ENABLE_SWITCH_INTERRUPT
#define     Enable_PORT             PORTD
#define     Enable_PIN              PIND
#define     Enable_DDR              DDRD
#define     Enable                  PD3
#endif // ENABLE_HARDWARE_ENABLE_SWITCH_INTERRUPT


// OUTPUT PINS DEFINITIONS
#ifdef PWM_ON
#define     PWM_PORT                PORTB
#define     PWM_PIN                 PINB
#define     PWM_DDR                 DDRB
#define     PWM                     PB1
#endif // PWM_ON

#ifdef LED_ON
#define     LED_PORT                PORTD
#define     LED_PIN                 PIND
#define     LED_DDR                 DDRD
#define     LED                     PD7
#define     cpl_led()               cpl_bit(LED_PORT, LED)
#define     set_led()               set_bit(LED_PORT, LED)
#define     clr_led()               clr_bit(LED_PORT, LED)
#else
#define     cpl_led()               
#define     set_led()               
#define     clr_led()               
#endif // LED_ON

#endif // MACHINE_ON

#ifdef CAN_ON
// CANBUS DEFINITONS
// ----------------------------------------------------------------------------
/* Global settings for building the can-lib and application program.
 *
 * The following two #defines must be set identically for the can-lib and
 * your application program. They control the underlying CAN struct. If the
 * settings disagree, the underlying CAN struct will be broken, with
 * unpredictable results.
 * If can.h detects that any of the #defines is not defined, it will set them
 * to the default values shown here, so it is in your own interest to have a
 * consistent setting. Ommiting the #defines in both can-lib and application
 * program will apply the defaults in a consistent way too.
 *
 * Select if you want to use 29 bit identifiers.
 */
#define	SUPPORT_EXTENDED_CANID	0

/* Select if you want to use timestamps.
 * Timestamps are sourced from a register internal to the AT90CAN.
 * Selecting them on any other controller will have no effect, they will
 * be 0 all the time.
 */
#define	SUPPORT_TIMESTAMPS		0
#endif // CAN_ON

#endif /* ifndef CONF_H */
