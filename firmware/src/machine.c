#include "machine.h"

/*
 * to-do:
 *      - modularize state sinalization
 *      - 
 *
 */

/**
 * @brief 
 */
void machine_init(void)
{
    /*TCCR2A  =   (1 << WGM21) | (0 << WGM20)         // Timer 2 in Mode 2 = CTC (clear on compar  e)
            | (0 << COM2A1) | (0 << COM2A0)         // do nothing with OC2A
            | (0 << COM2B1) | (0 << COM2B0);        // do nothing with OC2B
    TCCR2B  =   (0 << WGM22)                        // Timer 0 in Mode 2 = CTC (clear on compar  e)
            | (0 << FOC0A) | (0 << FOC0B)           // dont force outputs
            | (1 << CS22)                           // clock enabled, prescaller = 1024
            | (1 << CS21)
            | (1 << CS20);
    OCR2A   =   240; //80                               // Valor para igualdade de comparacao A par  a frequencia de 150 Hz
    TIMSK2 |=   (1 << OCIE2A);                      // Ativa a interrupcao na igualdade de comp  aração do TC2 com OCR2A
	*/

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

} 

/**
 * @brief checks the quantity of the faults.
 */
inline void check_pwm_fault(void)
{
	#ifdef PWM_ON
    if(pwm_fault_count >= FAULT_COUNT_LIMIT){
        error_flags.fault = 1;
        check_pwm_fault_times = 0;
        set_state_error();
    }else if(check_pwm_fault_times++ > CHECKS_BEFORE_RESET_FAULT_COUNTER){
        pwm_fault_count = 0;    
    }
	#endif
}

/**
 * @brief checks the buffer and waits it fills up
 */
inline void check_buffers(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string("Checking buffers..."));
#ifdef ADC_ON
    while(!CBUF_IsFull(cbuf_adc0));
    while(!CBUF_IsFull(cbuf_adc1));
    while(!CBUF_IsFull(cbuf_adc2));
#endif
    //VERBOSE_MSG_ERROR(usart_send_string("<No buffers to check>"));
    //VERBOSE_MSG_MACHINE(usart_send_string(" \t\tdone.\n")); 
}


/**
* @brief read and checks current levels
*/
inline void read_and_check_adcs(void)
{ 
#ifdef ADC_ON
    control.vi[0] = MA_PANEL_VOLTAGE * CONVERSION_PANEL_VOLTAGE_VALUE;
    control.ii[0] = MA_PANEL_CURRENT * CONVERSION_PANEL_CURRENT_VALUE;
    control.vo[0] = MA_BATTERY_VOLTAGE * CONVERSION_BATTERY_VOLTAGE_VALUE;

    switch(state_machine){
        case STATE_INITIALIZING:
            check_initializing_panel_voltage();
            check_initializing_panel_current();
            check_initializing_battery_voltage();

            break;
        case STATE_IDLE:
            check_idle_panel_voltage();
            check_idle_panel_current();
            check_idle_battery_voltage();

            break;
        case STATE_RUNNING:
            check_running_panel_voltage();
            check_running_panel_current();
            check_running_battery_voltage();

            break;
        default:
            break;
    } 
#endif
 
}

/**
* @brief checks if the current level is ok for INITIALIZING state
*/
inline void check_initializing_panel_current(void)
{ 
    //
}

/**
 * @brief checks if the voltage level is ok for INITIALIZING state
 */
inline void check_initializing_panel_voltage(void)
{
    //
}

/**
* @brief checks if the voltage of Battery level is ok for INITIALIZING state
*/
inline void check_initializing_battery_voltage(void)
{
    //
}


/**
 * @brief checks if the current level is ok for IDLE state
 */
inline void check_idle_panel_current(void)
{ 

	/*dif(control.ii[0] >= MAXIMUM_IDLE_PANEL_CURRENT ){		// MAXIMUM_RUNNING_PANEL_VOLTAGE sem valor em #define 
	   d error_flags.overcurrent = 1;
	}else if(control.ii[0] <= MINIMUM_IDLE_PANEL_CURRENT){
		error_flags.undercurrent = 1;
	}else error_flags.overcurrent = 0;*/
 
}

/**
 * @brief checks if the voltage level is ok for IDLE state
 */
inline void check_idle_panel_voltage(void)
{
   	/*if(control.vi[0] >= MAXIMUM_IDLE_PANEL_VOLTAGE){
	   	error_flags.overvolt_panel = 1;
   	}else if(control.vi[0] <= MINIMUM_IDLE_PANEL_VOLTAGE){
		error_flags.undervol_panel = 1;	
	}else error_flags.overvolt_panel = 0; 
    */
}

/**
* @brief checks if the voltage of Battery level is ok for IDLE state
*/
inline void check_idle_battery_voltage(void)
{
    check_running_battery_voltage();
}
 
/**
 * @brief checks if the current level is ok for running state
 */
inline void check_running_panel_current(void)
{
    if(control.ii[0] >= MAXIMUM_RUNNING_PANEL_CURRENT ){		// MAXIMUM_RUNNING_PANEL_VOLTAGE sem valor em #define 
		error_flags.overcurrent = 1;
	/*}else if(control.ii[0] <= MINIMUM_RUNNING_PANEL_CURRENT){
		error_flags.undercurrent = 1;*/
	}else error_flags.overcurrent = 0;
}

/**
 * @brief checks if the voltage level is ok for running state
 */
inline void check_running_panel_voltage(void)
{
   	/*
    if(control.vi[0] >= MAXIMUM_RUNNING_PANEL_VOLTAGE){		// MAXIMUM_RUNNING_PANEL_VOLTAGE sem valor em #define 
	   	error_flags.overvolt_panel = 1;
   	}else if(control.vi[0] <= MINIMUM_RUNNING_PANEL_VOLTAGE){
		error_flags.undervol_panel = 1;	
	}else error_flags.overvolt_panel = 0;
    */
}

/**
 * @brief checks if the voltage of Battery level is ok for running state
 */
inline void check_running_battery_voltage(void) // sem panel
{
   	if(control.vo[0] >= MAXIMUM_BATTERY_VOLTAGE){		// MAXIMUM_RUNNING_PANEL_VOLTAGE sem valor em #define 
	   	error_flags.overvoltage = 1;
   	/*}else if(control.vo[0] <= MINIMUM_BATTERY_VOLTAGE){
		error_flags.undervoltage = 1; */
	}else error_flags.overvoltage = 0; 
}

/**
 * @brief set error state
 */
inline void set_state_error(void)
{
    state_machine = STATE_ERROR;
}

/**
* @brief set initializing state
*/ 
inline void set_state_initializing(void)
{
    state_machine = STATE_INITIALIZING;
}

/**
* @brief set idle state
*/ 
inline void set_state_idle(void)
{
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
    state_machine = STATE_RUNNING;
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
    /*
    VERBOSE_MSG_MACHINE(usart_send_string("\nOvrI: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overcurrent));
    
    VERBOSE_MSG_MACHINE(usart_send_string(" OvrV: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overvoltage));

    VERBOSE_MSG_MACHINE(usart_send_string(" UndV: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.undervoltage));

    VERBOSE_MSG_MACHINE(usart_send_string(" OvrT: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overheat));

    VERBOSE_MSG_MACHINE(usart_send_string(" F: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.fault)); 

    VERBOSE_MSG_MACHINE(usart_send_string(" NOCAN: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.no_canbus));
    */
    VERBOSE_MSG_MACHINE(usart_send_string("\nerrFl: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overcurrent));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.undercurrent));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overvoltage));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.undervoltage));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overvolt_panel));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.undervolt_panel));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.overheat));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.fault));
    VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.no_canbus));

}
 
/**
* @brief prints control d infos
*/
inline void print_control_others(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" ctrl: "));
    VERBOSE_MSG_MACHINE(usart_send_uint8(control.mppt_pot_limit));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint8(control.fault));

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
* @brief prints control vi infos
*/
inline void print_control_vi(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Vp: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.vi[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.vi[1]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}
 
/**
* @brief prints control ii infos
*/
inline void print_control_ii(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Ip: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.ii[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.ii[1]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
} 
 
/**
* @brief prints control vo infos
*/
inline void print_control_vo(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Vb: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.vo[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.vo[1]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
} 

/**
* @brief prints control vo infos
*/
inline void print_control_pi(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" Pp: "));
#ifdef ADC_8BITS
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.pi[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.pi[1]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
#else
    VERBOSE_MSG_MACHINE(usart_send_uint32(control.pi[0]));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    VERBOSE_MSG_MACHINE(usart_send_uint32(control.pi[1]));
    VERBOSE_MSG_MACHINE(usart_send_char(' ')); 
#endif
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
    set_pwm_off();
#endif
    pwm_fault_count = 0;

    check_buffers();
    check_idle_panel_current();
    check_idle_panel_voltage();
    check_idle_battery_voltage();
    
    VERBOSE_MSG_INIT(usart_send_string("System initialized without errors.\n"));
    set_state_idle();
}

/**
 * @brief waits for commands while checking the system:
 *  - checks the deadman's switch state
 *  - checks the on_off_switch state
 *  - checks the potentiometer state
 *  then if its ok, enable the system to operate
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

    if(system_flags.mppt_on && system_flags.enable){
        if(init_pwm_increment_divider++){
            init_pwm_increment_divider = 0;
            if(control.D < PWM_INITIAL_D) control.D++;
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
    }
#endif

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
#endif

    if(system_flags.mppt_on && system_flags.enable){
#ifdef PWM_ON
        pwm_compute();
#endif
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

    if(error_flags.overcurrent)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Panel over-current!\n"));
    if(error_flags.overvoltage)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Battery over-voltage!\n"));
    if(error_flags.fault)
        VERBOSE_MSG_ERROR(usart_send_string("\t - FAULT!\n"));
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
        for(;;);    // waits the watchdog to reset.
    }
    
#ifdef LED_ON
    cpl_led();
#endif
    set_state_initializing();
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
                print_control_ii();
                break;
            case 5:
                print_control_vo();
                break;
            case 6:
                print_control_pi();
                break;
            case 7:
                print_control_others(); 
            default:
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

#define MACHINE_CLK_DIVIDER_VALUE           10
    static uint8_t machine_clk_divider = 0;

    print_infos();

    if(machine_clk){
        machine_clk = 0;
        if(machine_clk_divider++ == MACHINE_CLK_DIVIDER_VALUE){
            machine_clk_divider = 0;

            if(adc_data_ready){
                adc_data_ready = 0;
                read_and_check_adcs();
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

                default:
                    break;
            }
        }
    }
}

/**
 * @brief Interrupcao das chaves: se alguma chave desligar, o motor desliga.
 */

ISR(INT0_vect) //overvoltage
{
	control.fault = 1;
}
ISR(INT1_vect) //enable
{    
    /*if(bit_is_clear(FAULT_PIN, FAULT)){
        pwm_treat_fault();
#ifdef LED_ON
        cpl_led();
#endif
        pwm_fault_count++;
    }
    */
    system_flags.enable = 0;
    DEBUG1;
}

/**
* @brief ISR para ações de controle
*/
ISR(TIMER2_COMPA_vect)
{
    VERBOSE_MSG_ERROR(if(machine_clk) usart_send_string("\nERROR: CLOCK CONFLICT!!!\n"));
	machine_clk = 1;
}

