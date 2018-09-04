#include "adc.h"

/**
 * @brief initializes all adc circular buffers.
 */
void init_buffers(void)
{
    CBUF_Init(cbuf_adc0);
    CBUF_Init(cbuf_adc1);
    CBUF_Init(cbuf_adc2);
}

/**
 * @brief computes the average of a given adc channel
 *
 * Ma = (1/N)*Summation of x[i] from i=0 to N, 
 * if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
 *
 */
#ifdef ADC_8BITS
inline uint8_t calc_ma_adc0(void)
#else
inline uint16_t calc_ma_adc0(void)
#endif
{   
    uint16_t sum = 0;
    for(uint8_t i = cbuf_adc0_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc0, i);
    }
    ma_adc0 = sum >> cbuf_adc0_SIZE_2;
    return ma_adc0;
}

/**
* @brief computes the average of a given adc channel
*
* Ma = (1/N)*Summation of x[i] from i=0 to N, 
* if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
*
*/
#ifdef ADC_8BITS
inline uint8_t calc_ma_adc1(void)
#else
inline uint16_t calc_ma_adc1(void)
#endif
{   
    uint16_t sum = 0;
    for(uint8_t i = cbuf_adc1_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc1, i);
    }
    ma_adc1 = sum >> cbuf_adc1_SIZE_2;
    return ma_adc1;
}

/**
* @brief computes the average of a given adc channel 
*
* Ma = (1/N)*Summation of x[i] from i=0 to N, 
* if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
*
*/
#ifdef ADC_8BITS
inline uint8_t calc_ma_adc2(void)
#else
inline uint16_t calc_ma_adc2(void)
#endif 
{   
    uint16_t sum = 0;
    for(uint8_t i = cbuf_adc2_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc2, i);
    }
    ma_adc2 = sum >> cbuf_adc2_SIZE_2;
    return ma_adc2;
}

/**
 * @brief Changes ADC channel
 * @param __ch is the channel to be switched to
 * @return return the selected channel
 */
inline uint8_t adc_select_channel(adc_channels_t __ch)
{
    if(__ch < ADC_LAST_CHANNEL ) ADC_CHANNEL = __ch;

    ADMUX = (ADMUX & 0xF8) | ADC_CHANNEL; // clears the bottom 3 bits before ORing
    return ADC_CHANNEL;
}

/**
 * @brief inicializa o ADC, configurado para conversão engatilhada com o timer0.
 */
void adc_init(void)
{
    adc_data_ready = 0;
    clr_bit(PRR0, PRADC);                           // Activates clock to adc

    // configuracao do ADC
    PORTC   =   0b00000000;                         // disables pull-up for adcs pins
    DDRC    =   0b00000000;                         // all adcs as inputs
    DIDR0   =   0b11111111;                         // ADC0 to ADC2 as adc (digital disable)

    ADMUX   =   (0 << REFS1)                        // AVcc with external capacitor at AREF pin
            | (1 << REFS0)
#ifdef ADC_8BITS
            | (1 << ADLAR);                         // ADC left adjusted -> using 8bits ADCH only
#else
            | (0 << ADLAR);                         // ADC left adjusted -> using all 10 bits
#endif

    ADCSRB  =   (0 << ADTS2)                        // Auto-trigger source: timer0 Compare Match A
            | (1 << ADTS1)
            | (1 << ADTS0);

    adc_select_channel(ADC0);                       // Choose admux
    ADCSRA  =   (1 << ADATE)                        // ADC Auto Trigger Enable
            | (1 << ADIE)                           // ADC Interrupt Enable
            | (1 << ADEN)                           // ADC Enable
            | (1 << ADSC)                           // Do the first Start of Conversion
            | (1 << ADPS2)                          // ADC Prescaller = 128;
            | (1 << ADPS1)
            | (1 << ADPS0);

    // TIMER configurations

    clr_bit(PRR0, PRTIM0);                          // Activates clock to timer0
    // MODE 2 -> CTC with TOP on OCR1
    TCCR0A  =   (1 << WGM01) | (0 << WGM00)         // mode 2
            | (0 << COM0B1) | (0 << COM0B0)         // do nothing 
            | (0 << COM0A1) | (0 << COM0A0);        // do nothing

    TCCR0B  =
#if ADC_TIMER_PRESCALER ==     1
                (0 << CS02) | (0 << CS01) | (1 << CS00) // Prescaler N=1
#elif ADC_TIMER_PRESCALER ==   8
                (0 << CS02) | (1 << CS01) | (0 << CS00) // Prescaler N=8
#elif ADC_TIMER_PRESCALER ==   32
                (0 << CS02) | (1 << CS01) | (1 << CS00) // Prescaler N=32
#elif ADC_TIMER_PRESCALER ==   64
                (1 << CS02) | (0 << CS01) | (0 << CS00) // Prescaler N=64
#elif ADC_TIMER_PRESCALER ==   128
                (1 << CS02) | (0 << CS01) | (1 << CS00) // Prescaler N=128
#elif ADC_TIMER_PRESCALER ==   256
                (1 << CS02) | (1 << CS01) | (0 << CS00) // Prescaler N=256
#elif ADC_TIMER_PRESCALER ==   1024
                (1 << CS02) | (1 << CS01) | (1 << CS00) // Prescaler N=1024
#else
                0
#endif
                | (0 << WGM02);      // mode 2

    OCR0A = ADC_TIMER_TOP;                       	// OCR2A = TOP = fcpu/(N*2*f) -1


    TIMSK0 |=   (1 << OCIE0A);                      // Ativa a interrupcao na igualdade de comparação do TC0 com OCR0A

    init_buffers();

}


/**
 * @brief executes a simple average
 */
inline void calc_avg(void)
{
    avg_adc0 = ma_adc0_sum / avg_sum_samples;
    avg_adc1 = ma_adc1_sum / avg_sum_samples;
    avg_adc2 = ma_adc2_sum / avg_sum_samples;
    ma_adc0_sum = ma_adc1_sum = ma_adc2_sum = avg_sum_samples = 0;
}


/**
 * @brief MUX do ADC
 */
ISR(ADC_vect){
    switch(ADC_CHANNEL){
        case ADC0:
            VERBOSE_MSG_ADC(usart_send_string(" \tadc0: "));
#ifdef ADC_8BITS
            CBUF_Push(cbuf_adc0, ADCH); 
#else
            CBUF_Push(cbuf_adc0, ADC); 
#endif
            calc_ma_adc0();
            ma_adc0_sum += ma_adc0;
            ADC_CHANNEL++;
            break;
        case ADC1:
            VERBOSE_MSG_ADC(usart_send_string(" \tadc1: "));
#ifdef ADC_8BITS
            CBUF_Push(cbuf_adc1, ADCH); 
#else
            CBUF_Push(cbuf_adc1, ADC); 
#endif
            calc_ma_adc1();
            ma_adc1_sum += ma_adc1;
            ADC_CHANNEL++;
            break;
        case ADC2:
            VERBOSE_MSG_ADC(usart_send_string(" \tadc2: "));
#ifdef ADC_8BITS
            CBUF_Push(cbuf_adc2, ADCH);
#else
            CBUF_Push(cbuf_adc2, ADC);
#endif
            calc_ma_adc2();
            ma_adc2_sum += ma_adc2;
			ADC_CHANNEL++;
            //break;
        default:
            avg_sum_samples++;
            adc_data_ready = 1;
            ADC_CHANNEL = ADC0;             // recycles
            VERBOSE_MSG_ADC(usart_send_string("\n"));
            break;
    }        
#ifdef ADC_8BITS
    VERBOSE_MSG_ADC(usart_send_uint8(ADCH));
#else
    VERBOSE_MSG_ADC(usart_send_uint16(ADC));
#endif
    adc_select_channel(ADC_CHANNEL);
}
 
/**
 * @brief ISR necessária para auto-trigger do ADC. Caso contrário, dispara
 * BADISR_vect.
 */
EMPTY_INTERRUPT(TIMER0_COMPA_vect);

