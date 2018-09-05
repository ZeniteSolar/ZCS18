#include "machine.h"

/*
 *
 */

/**
 * @brief 
 */
void machine_init(void)
{
	clr_bit(PRR0, PRTIM2);                          // Activates clock

    // MODE 2 -> CTC with TOP on OCR1
    TCCR2A  =    (1 << WGM21) | (0 << WGM20)        // mode 2
              | (0 << COM2B1) | (0 << COM2B0)       // do nothing
              | (0 << COM2A1) | (0 << COM2A0);      // do nothing

    //clr_bit(DDRB, PB3); // output for frequency debug

    TCCR2B =
#if MACHINE_TIMER_PRESCALER ==     1
                (0 << CS22) | (0 << CS21) | (1 << CS20) // Prescaler N=1
#elif MACHINE_TIMER_PRESCALER ==   8
                (0 << CS22) | (1 << CS21) | (0 << CS20) // Prescaler N=8
#elif MACHINE_TIMER_PRESCALER ==   32
                (0 << CS22) | (1 << CS21) | (1 << CS20) // Prescaler N=32
#elif MACHINE_TIMER_PRESCALER ==   64
                (1 << CS22) | (0 << CS21) | (0 << CS20) // Prescaler N=64
#elif MACHINE_TIMER_PRESCALER ==   128
                (1 << CS22) | (0 << CS21) | (1 << CS20) // Prescaler N=128
#elif MACHINE_TIMER_PRESCALER ==   256
                (1 << CS22) | (1 << CS21) | (0 << CS20) // Prescaler N=256
#elif MACHINE_TIMER_PRESCALER ==   1024
                (1 << CS22) | (1 << CS21) | (1 << CS20) // Prescaler N=1024
#else
                0
#endif
                | (0 << WGM22);      // mode 2

    OCR2A = MACHINE_TIMER_TOP;                       // OCR2A = TOP = fcpu/(N*2*f) -1

    TIMSK2 |=   (1 << OCIE2A);                      // Activates interruption

    set_machine_initial_state();
    set_state_initializing();
} 

/**
 * @brief set machine initial state
 */
inline void set_machine_initial_state(void)
{
#ifdef CAN_ON
    system_flags.mppt_on = 1;
#else
    system_flags.mppt_on = 0;
#endif // CAN_ON
    system_flags.enable = 0;

    error_flags.all = 0;

    control.D = 0;
    control.pi_limit = 0;
    control.updown = 0;
    control.vi[0] = control.vi[1] = 0;
    control.ii[0] = control.ii[1] = 0;
    control.pi[0] = control.pi[1] = 0;
    control.vo[0] = control.vo[1] = 0;
    control.io[0] = control.io[1] = 0;
    control.po[0] = control.po[1] = 0;
    control.dvi = control.dvo = control.dii = control.dio = control.dpi = 0;
    control.mpp_vi = control.mpp_ii = control.mpp_pi = control.mpp_D = 0;

    machine_clk = machine_clk_divider = led_clk_div = 0;
    something_changed = 0;
}

/**
 * @brief checks the buffer and waits it fills up
 */
inline void check_buffers(void)
{
#ifdef ADC_ON
    VERBOSE_MSG_MACHINE(usart_send_string("Checking buffers..."));
    while(!CBUF_IsFull(cbuf_adc0));
    while(!CBUF_IsFull(cbuf_adc1));
    while(!CBUF_IsFull(cbuf_adc2));
    VERBOSE_MSG_MACHINE(usart_send_string(" \t\tdone.\n")); 
#else
    VERBOSE_MSG_ERROR(usart_send_string("<No buffers to check>"));
#endif
}


/**
* @brief read and checks current levels
*/
inline void read_and_check_adcs(void)
{ 
#ifdef ADC_ON
    calc_avg();
    uint64_t tmp;
    tmp = (uint64_t)(uint16_t)AVG_PANEL_VOLTAGE * (uint32_t)ADC_PANEL_VOLTAGE_ANGULAR_COEF ;
    control.vi[0] = tmp >> 10;
//        +ADC_PANEL_VOLTAGE_LINEAR_COEF;
    tmp = (uint64_t)(uint16_t)AVG_PANEL_CURRENT * (uint32_t)ADC_PANEL_CURRENT_ANGULAR_COEF ; 
    control.ii[0] = tmp >> 10;
//        +ADC_PANEL_CURRENT_LINEAR_COEF;
    tmp = (uint64_t)(uint16_t)AVG_BATTERY_VOLTAGE * (uint32_t)ADC_BATTERY_VOLTAGE_ANGULAR_COEF ; 
    control.vo[0] = tmp >> 10;
//        +ADC_BATTERY_VOLTAGE_LINEAR_COEF;

    switch(state_machine){
        default:
        case STATE_INITIALIZING:
#ifdef CHECK_INITIALIZING_CONDITIONS
            check_initializing_panel_voltage();
            check_initializing_panel_current();
            check_initializing_battery_voltage();
#endif // CHECK_INITIALIZING_CONDITIONS

            break;
        case STATE_IDLE:
#ifdef CHECK_IDLE_CONDITIONS
            check_idle_panel_voltage();
            check_idle_panel_current();
            check_idle_battery_voltage();
#endif //CHECK_IDLE_CONDITIONS

            break;
        case STATE_RUNNING:
#ifdef CHECK_RUNNING_CONDITIONS
            check_running_panel_voltage();
            check_running_panel_current();
            check_running_battery_voltage();
#endif // CHECK_RUNNING_CONDITIONS

            break;
    } 
#endif
 
}

/**
* @brief checks if the current level is ok for INITIALIZING state
*/
inline void check_initializing_panel_current(void)
{ 
    if(control.ii[0]> NOT_RUNNING_PANEL_CURRENT_MAX){
        // PANEL SHORTED 
        // or current sensor problem
        // there is no way to diagnose
        // -> error
        error_flags.panel_overcurrent = 1;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overcurrent = 1\n"));
    }else if(control.ii[0]< NOT_RUNNING_PANEL_CURRENT_MIN){
        // PANEL OPEN 
        // -> ok
        error_flags.panel_overcurrent = 0;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overcurrent = 0\n"));
    }
}

/**
 * @brief checks if the voltage level is ok for INITIALIZING state
 */
inline void check_initializing_panel_voltage(void)
{
    if(control.vi[0]> NOT_RUNNING_PANEL_VOLTAGE_MAX){
        // PANEL OVERVOLTAGE 
        // or panel voltage sensor damaged 
        // -> error
        error_flags.panel_overvoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overvoltage = 1\n"));
    }else{
        error_flags.panel_overvoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overvoltage = 0\n"));
    }

    if(control.vi[0]< NOT_RUNNING_PANEL_VOLTAGE_MIN){
        // PANEL DISCONNECTED 
        // or shorted: should check current
        // if panel current isn't above limits, 
        // the panel voltage sensor is damaged
        // -> error
        error_flags.panel_undervoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("panel.undervoltage = 1\n"));
    }else{
        error_flags.panel_undervoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("panel.undervoltage = 0\n"));
    }
}

/**
* @brief checks if the voltage of Battery level is ok for INITIALIZING state
*/
inline void check_initializing_battery_voltage(void)
{
    if(control.vo[0]> NOT_RUNNING_BATTERY_VOLTAGE_MAX){
        // BATTERY OVERVOLTAGE -> overcharged
        // or battery voltage sensor damaged
        // if it is thee case, it will not protect against overcharge.
        // -> error
        error_flags.battery_overvoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("battery.overvoltage = 1\n"));
    }else{
        error_flags.battery_overvoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("battery.overvoltage = 0\n"));
    }
        
    if(control.vo[0]< NOT_RUNNING_BATTERY_VOLTAGE_MIN ){
        // BATTERY UNDERVOLTAGE -> discharged
        // or battery voltage sensor damaged
        // if it is thee case, it will not protect against overcharge.
        // -> error
        error_flags.battery_undervoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("battery.undervoltage = 1\n"));
    }else{
        error_flags.battery_undervoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("battery.undervoltage = 0\n"));
    }

}


/**
 * @brief checks if the current level is ok for IDLE state
 */
inline void check_idle_panel_current(void)
{ 
   check_initializing_panel_current();
}

/**
 * @brief checks if the voltage level is ok for IDLE state
 */
inline void check_idle_panel_voltage(void)
{
    check_initializing_panel_voltage();
}

/**
* @brief checks if the voltage of Battery level is ok for IDLE state
*/
inline void check_idle_battery_voltage(void)
{
    check_initializing_battery_voltage();
}
 
/**
 * @brief checks if the current level is ok for running state
 */
inline void check_running_panel_current(void)
{
    if(control.ii[0]>= RUNNING_PANEL_CURRENT_MAX){
        // PANEL SHORTED
        // or current sensor problem
        // there is no way to diagnose
        // ...or OUTPUT SHORTED
        // than the battery voltage sensor should indicate an undervoltage
        // in any case it should stop running.
        // -> error
        error_flags.panel_overcurrent = 1;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overcurrent = 1\n"));
    }else{
        error_flags.panel_overcurrent = 0;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overcurrent = 0\n"));
    }
    if(control.ii[0]<= RUNNING_PANEL_CURRENT_MIN){
        // PANEL DISCONNECTED
        // or FUSE BLOWED
        // or CONTROL LOST MPP TRACK
        // or PWM DRIVER FAILED
        // or MOSFET FAILED OPEN
        // or DIODE FAILED OPEN
        // or just NOT ENOUGH SUN
        // TODO: diagnose each case!!!
        
        VERBOSE_MSG_ERROR(usart_send_string("TODO: DIAGNOSE: PANEL DISCONNECTED?\n"));
  
    }else{
    }
}

/**
 * @brief checks if the voltage level is ok for running state
 */
inline void check_running_panel_voltage(void)
{
    if(control.vi[0]> RUNNING_PANEL_VOLTAGE_MAX){
        // PANEL OVERVOLTAGE 
        // or panel voltage sensor damaged 
        // -> error but not critical
        error_flags.panel_overvoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overvoltage = 1\n"));
    }else{
        error_flags.panel_overvoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("panel.overvoltage = 0\n"));
    }

    if(control.vi[0]< RUNNING_PANEL_VOLTAGE_MIN){
        // PANEL DISCONNECTED???
        // TODO: check the sensor measurements in the prototype for this case
        // or shorted: should check current
        // if panel current isn't above limits, 
        // the panel voltage sensor is damaged
        // -> error but not critical
        error_flags.panel_undervoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("panel.undervoltage = 1\n"));
    }else{
        error_flags.panel_undervoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("panel.undervoltage = 0\n"));
    }
}

/**
 * @brief checks if the voltage of Battery level is ok for running state
 */
inline void check_running_battery_voltage(void) // sem panel
{
    if(control.vo[0]> RUNNING_BATTERY_VOLTAGE_MAX){
        // BATTERY OVERVOLTAGE -> overcharged
        // or battery voltage sensor damaged
        // if it is thee case, it will not protect against overcharge.
        // -> error
        error_flags.battery_overvoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("battery.overvoltage = 1\n"));
    }else{
        error_flags.battery_overvoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("battery.overvoltage = 1\n"));
    }
        
    if(control.vo[0]< RUNNING_BATTERY_VOLTAGE_MIN ){
        // BATTERY UNDERVOLTAGE -> discharged
        // or battery voltage sensor damaged
        // if it is thee case, it will not protect against overcharge.
        // -> error
        error_flags.battery_undervoltage = 1;
        VERBOSE_MSG_ERROR(usart_send_string("battery.undervoltage = 1\n"));
    }else{
        error_flags.battery_undervoltage = 0;
        VERBOSE_MSG_ERROR(usart_send_string("battery.undervoltage = 0\n"));
    }
}

/**
 * @brief set error state
 */
inline void set_state_error(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n>>>STATE ERROR\n"));
    state_machine = STATE_ERROR;
}

/**
* @brief set initializing state
*/ 
inline void set_state_initializing(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n>>>INITIALIZING STATE\n"));
    state_machine = STATE_INITIALIZING;
}

/**
* @brief set idle state
*/ 
inline void set_state_idle(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n>>>IDLE STATE\n"));
    state_machine = STATE_IDLE;
#ifdef PWM_ON
    pwm_reset();
#endif
}

/**
* @brief set running state
*/ 
inline void set_state_running(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n>>>RUNNING STATE\n"));
    state_machine = STATE_RUNNING;
#ifdef ENABLE_PERTURB_AND_OBSERVE
#ifndef ENABLE_SOFT_START
#ifndef ENABLE_SWEEP
    control.D = PWM_D_NOMINAL;
#endif
#endif
#endif

#ifdef ENABLE_SWEEP
    sweep_periods = (PERIODS_TO_SWEEP-1);
#endif
}

/**
 * @brief set reset state
 */
inline void set_state_reset(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n>>>RESET STATE\n"));
    state_machine = STATE_RESET;
}

/**
 * @breif prints the configurations and definitions
 */
inline void print_configurations(void)
{    

    VERBOSE_MSG_MACHINE(usart_send_string("CONFIGURATIONS:\n"));
    VERBOSE_MSG_MACHINE(usart_send_string("\nvi: "));
    VERBOSE_MSG_MACHINE(usart_send_uint32( NOT_RUNNING_PANEL_VOLTAGE_MAX ));
    VERBOSE_MSG_MACHINE(usart_send_char(','));
    VERBOSE_MSG_MACHINE(usart_send_uint32( NOT_RUNNING_PANEL_VOLTAGE_MIN ));
    VERBOSE_MSG_MACHINE(usart_send_string("\nii: "));
    VERBOSE_MSG_MACHINE(usart_send_uint32( NOT_RUNNING_PANEL_CURRENT_MAX ));
    VERBOSE_MSG_MACHINE(usart_send_char(','));
    VERBOSE_MSG_MACHINE(usart_send_uint32( NOT_RUNNING_PANEL_CURRENT_MIN ));
    VERBOSE_MSG_MACHINE(usart_send_string("\nvo: "));
    VERBOSE_MSG_MACHINE(usart_send_uint32( NOT_RUNNING_BATTERY_VOLTAGE_MAX ));
    VERBOSE_MSG_MACHINE(usart_send_char(','));
    VERBOSE_MSG_MACHINE(usart_send_uint32( NOT_RUNNING_BATTERY_VOLTAGE_MIN )); 

    VERBOSE_MSG_MACHINE(usart_send_string("\nvi: "));
    VERBOSE_MSG_MACHINE(usart_send_uint32( RUNNING_PANEL_VOLTAGE_MAX ));
    VERBOSE_MSG_MACHINE(usart_send_char(','));
    VERBOSE_MSG_MACHINE(usart_send_uint32( RUNNING_PANEL_VOLTAGE_MIN ));
    VERBOSE_MSG_MACHINE(usart_send_string("\nii: "));
    VERBOSE_MSG_MACHINE(usart_send_uint32( RUNNING_PANEL_CURRENT_MAX ));
    VERBOSE_MSG_MACHINE(usart_send_char(','));
    VERBOSE_MSG_MACHINE(usart_send_uint32( RUNNING_PANEL_CURRENT_MIN ));
    VERBOSE_MSG_MACHINE(usart_send_string("\nvo: "));
    VERBOSE_MSG_MACHINE(usart_send_uint32( RUNNING_BATTERY_VOLTAGE_MAX ));
    VERBOSE_MSG_MACHINE(usart_send_char(','));
    VERBOSE_MSG_MACHINE(usart_send_uint32( RUNNING_BATTERY_VOLTAGE_MIN ));
    VERBOSE_MSG_MACHINE(usart_send_char('\n'));
}

/**
* @brief prints the system flags
*/
inline void print_system_flags(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" ON: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.mppt_on));
    
    VERBOSE_MSG_MACHINE(usart_send_string(" EN "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.enable));
}

/**
* @brief prints the error flags
*/
inline void print_error_flags(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" errFl: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.panel_overvoltage));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.panel_undervoltage));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.panel_overcurrent));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.battery_overvoltage));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.battery_undervoltage));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.battery_overcurrent));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.no_canbus));
}
 
/**
* @brief prints control d infos
*/
inline void print_control_others(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" ctrl: "));
    VERBOSE_MSG_MACHINE(usart_send_uint8(control.pi_limit));
}

/**
* @brief prints control d infos
*/
inline void print_control_d(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" D: "));
    VERBOSE_MSG_MACHINE(usart_send_uint8(control.updown));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.D));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}
/**
* @brief prints control.vi[0]infos
*/
inline void print_control_vi(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Vp: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.vi[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint16(control.vi[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' '));
}

/**
* @brief prints control dvi infos
*/
inline void print_control_dvi(void)
{
    VERBOSE_MSG_MACHINE(usart_send_int32(control.dvi));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}
 
/**
* @brief prints control.ii[0]infos
*/
inline void print_control_ii(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Ip: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.ii[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint16(control.ii[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' '));
} 

/**
* @brief prints control.io[0]infos
*/
inline void print_control_io(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Ib: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.io[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint16(control.io[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' '));
} 
/**
* @brief prints control dii infos
*/
inline void print_control_dii(void)
{
    VERBOSE_MSG_MACHINE(usart_send_int32(control.dii));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}
 
/**
* @brief prints control dio infos
*/
inline void print_control_dio(void)
{
    VERBOSE_MSG_MACHINE(usart_send_int32(control.dio));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}
 
 
/**
* @brief prints control.vo[0]infos
*/
inline void print_control_vo(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Vb: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.vo[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint16(control.vo[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' '));
} 

/**
* @brief prints control dvo infos
*/
inline void print_control_dvo(void)
{
    VERBOSE_MSG_MACHINE(usart_send_int32(control.dvo));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}
 

/**
* @brief prints control pi infos
*/
inline void print_control_pi(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Pp: "));
#ifdef ADC_8BITS
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.pi[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint16(control.pi[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' '));
#else
    VERBOSE_MSG_MACHINE(usart_send_uint32(control.pi[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint32(control.pi[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' ')); 
#endif
}
 
/**
* @brief prints control po infos
*/
inline void print_control_po(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Pb: "));
#ifdef ADC_8BITS
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.po[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint16(control.po[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' '));
#else
    VERBOSE_MSG_MACHINE(usart_send_uint32(control.po[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    //VERBOSE_MSG_MACHINE(usart_send_uint32(control.po[1]));
    //VERBOSE_MSG_MACHINE(usart_send_char(' ')); 
#endif
}
 

/**
* @brief prints control dpi infos
*/
inline void print_control_dpi(void)
{
    VERBOSE_MSG_MACHINE(usart_send_int32(control.dpi));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}

 

/**
 * @brief Checks if the system is OK to run:
 *  - all ring_buffers needed to be full
 *  - checks the current
 *  - checks the voltage
 *
 */
inline void task_initializing(void)
{
#ifdef LED_ON
    set_led();
#endif
#ifdef PWM_ON
    pwm_reset();
    set_pwm_off();
#endif

    set_machine_initial_state();

    
    check_buffers();

    VERBOSE_MSG_INIT(usart_send_string("System initialized without errors.\n"));
    set_state_idle();
}

/**
 * @brief waits for commands while checking the system
 */
inline void task_idle(void)
{
#ifdef LED_ON
    if(led_clk_div++ >= 50){
        cpl_led();
        led_clk_div = 0;
    }        
#endif

#ifdef PWM_ON
    /*
    if(system_flags.mppt_on && system_flags.enable){
        if(init_pwm_increment_divider++){
            init_pwm_increment_divider = 0;
            if(control.D < PWM_D_INITIAL) control.D++;
            else{
                if(!error_flags.all){
                    VERBOSE_MSG_MACHINE(usart_send_string("Enjoy, the system is at its RUNNING STATE!!\n"));
                    set_state_running();
                }
                else{
                    VERBOSE_MSG_ERROR(usart_send_string("Sorry. I have have found errors in the initialilation process. \n\nI will begin to process it...\n"));
                    set_state_error();
                }
            }
        }
        pwm_compute();
    }*/

    if(system_flags.mppt_on && system_flags.enable){
        set_state_running();
    }

#endif // PWM_ON

}


/**
 * @brief running task checks the system and apply the control action to pwm.
 */
inline void task_running(void)
{
#ifdef LED_ON
    if(led_clk_div++ >= 10){
        cpl_led();
        led_clk_div = 0;
    }
#endif // LED_ON

    if(system_flags.mppt_on && system_flags.enable){
#ifdef PWM_ON

#ifdef CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
        control.D = CONVERTER_TEST_WITH_FIXED_DUTYCYCLE_DT_VALUE;
        set_pwm_duty_cycle(control.D);
#else
        pwm_compute();
#endif // CONVERTER_TEST_WITH_FIXED_DUTYCYCLE

#endif //PWM_ON

    }else{
        set_state_idle();
    }
}


/**
 * @brief error task checks the system and tries to medicine it.
 */
inline void task_error(void)
{
#ifdef LED_ON
    if(led_clk_div++ >= 5){
        cpl_led();
        led_clk_div = 0;
    }
#endif

#ifdef PWM_ON
    pwm_reset();
#endif

    total_errors++;         // incrementa a contagem de erros
    VERBOSE_MSG_ERROR(usart_send_string("The error code is: "));
    VERBOSE_MSG_ERROR(usart_send_uint16(error_flags.all));
    VERBOSE_MSG_ERROR(usart_send_char('\n'));

    if(error_flags.panel_overcurrent)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Panel over-current!\n"));
    if(error_flags.battery_overvoltage)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Battery over-voltage!\n"));
    if(error_flags.no_canbus)
        VERBOSE_MSG_ERROR(usart_send_string("\t - No canbus communication with MIC17!\n"));
    if(!error_flags.all)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Oh no, it was some unknown error.\n"));
 
    VERBOSE_MSG_ERROR(usart_send_string("The error level is: "));
    VERBOSE_MSG_ERROR(usart_send_uint16(total_errors));
    VERBOSE_MSG_ERROR(usart_send_char('\n'));
    
    if(total_errors < 2){
        VERBOSE_MSG_ERROR(usart_send_string("I will reset the machine state.\n"));
    }
    if(total_errors >= 20){
        VERBOSE_MSG_ERROR(usart_send_string("The watchdog will reset the whole system.\n"));
        set_state_reset();
    }
    
#ifdef LED_ON
    cpl_led();
#endif
    set_state_initializing();
}

/**
 * @brief reset error task just freezes the processor and waits for watchdog
 */
inline void task_reset(void)
{   
#ifdef PWM_ON
    pwm_reset();
    set_pwm_off();
#endif

#ifndef WATCHDOG_ON
    wdt_init();
#endif

    cli();  // disable interrupts

    VERBOSE_MSG_ERROR(usart_send_string("WAITING FOR A RESET!\n"));
    for(;;);
}

void print_infos(void)
{
    static uint8_t i = 0;

    if(something_changed){
        switch(i++){
            case 0:
                print_system_flags();
                break;
            case 1:
                print_error_flags();
                break;
            case 2:
                print_control_d();
                break;
            case 3:
                print_control_vi();
                break;
            case 4:
                //print_control_dvi();
                break;
            case 5:
                print_control_ii();
                break;
            case 6:
                //print_control_dii();
                break;
            case 7:
                print_control_vo();
                break;
            case 8:
                //print_control_dvo();
                break;
            case 9:
                print_control_io();
                break;
            case 10:
                print_control_pi();
                break;
            case 11:
                //print_control_dpi();
                break;
            case 12:
                print_control_po();
                break;
            case 13:
                //print_control_others(); 
            default:
                VERBOSE_MSG_MACHINE(usart_send_char('\n'));
                if(something_changed){
                    something_changed = 0;
                }
                break;
        }
    }else{
        i = 0;   
    }
}

/**
 * @brief this is the machine state itself.
 */
inline void machine_run(void)
{
	#ifdef CAN_ON
    can_app_task();
    #else
    system_flags.enable = system_flags.mppt_on = 1;
	#endif

    print_infos();

    if(machine_clk){
        machine_clk = 0;

        if(adc_data_ready){
            adc_data_ready = 0;

            read_and_check_adcs();

            if(error_flags.all){
                print_system_flags();
                print_error_flags();
                print_infos();
                print_control_others();
                print_control_d();
                print_control_vi();
                print_control_dvi();
                print_control_ii();
                print_control_dii();
                print_control_vo();
                print_control_dvo();
                print_control_pi();
                print_control_dpi();
                set_state_error();
            }

            something_changed = 1;

            switch(state_machine){
                case STATE_INITIALIZING:
                    task_initializing();

                    break;
                case STATE_IDLE:
                    task_idle();

                    break;
                case STATE_RUNNING:
                    task_running();

                    break;
                case STATE_ERROR:
                    task_error();

                case STATE_RESET:
                default:
                    task_reset();
                    break;
            }
        } 
    }
}

/**
 * @brief Interrupcao do pino de sobretensão via hardware
 */
ISR(INT0_vect)
{
#ifdef ENABLE_HARDWARE_OVERVOLTAGE_INTERRUPT
    error_flags.battery_overvoltage = 1;
    set_state_error();
    VERBOSE_MSG_ERROR(usart_send_string("OVERVOLTAGE INTERRUPT\n"));
#endif
}

/**
* @brief Interrupcao do pino da chave enable via hardware
*/ 
ISR(INT1_vect)
{    
#ifdef ENABLE_HARDWARE_ENABLE_SWITCH_INTERRUPT
    system_flags.enable = 0;
    set_state_idle();
    VERBOSE_MSG_ERROR(usart_send_string("ENABLE SWITCH INTERRUPT\n"));
#endif
}

/**
* @brief ISR para ações de controle
*/
ISR(TIMER2_COMPA_vect)
{
    if(machine_clk_divider++ == MACHINE_CLK_DIVIDER_VALUE){
        /*if(machine_clk){
            for(;;){
                pwm_reset();
                VERBOSE_MSG_ERROR(if(machine_clk) usart_send_string("\nERROR: CLOCK CONFLICT!!!\n"));
            }
        }*/
        machine_clk = 1;
        machine_clk_divider = 0;
    }
}

