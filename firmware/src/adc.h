/**
 * @file adc.h
 *
 * @defgroup ADC    ADC Module
 *
 * @brief This module implements a simple ADC using a state machine to mux
 * between the adc channels.
 *
 */

#ifndef _ADC_H_
#define _ADC_H_

#include "avr/io.h"
#include "avr/interrupt.h"
#include "conf.h"
#include "dbg_vrb.h"
#include "usart.h"
#include "../lib/bit_utils.h"
#include "../lib/cbuf.h"
#include "../lib/log2.h"

#define ADC_LAST_CHANNEL 3                  //*< quantity of channels used
//#define ADC_8BITS

// Equations for mode 2 (CTC with TOP OCR2A)
// Note the resolution. For example.. at 150hz, ICR1 = PWM_TOP = 159, so it
//#define QUOTIENT  (((uint32_t)MACHINE_TIMER_PRESCALER)*((uint32_t)MACHINE_TIMER_FREQUENCY))
//#define ADC_TIMER_TOP (0.5*(F_CPU)/QUOTIENT)
#define ADC_TIMER_FREQUENCY     ((uint32_t)(ADC_FREQUENCY)*(uint8_t)(ADC_LAST_CHANNEL))
#define ADC_TIMER_TOP ((F_CPU)/(2.*((uint32_t)ADC_TIMER_PRESCALER)*((uint32_t)ADC_TIMER_FREQUENCY)) -1)

uint8_t adc_data_ready;
uint16_t avg_sum_samples;
void calc_avg(void);

typedef volatile enum adc_channels{ 
    ADC0, ADC1 ,ADC2, ADC3, ADC4, ADC5  
} adc_channels_t;                           //*< the adc_channel type 
static adc_channels_t ADC_CHANNEL = ADC0;   //*< current chosen adc channel

uint8_t adc_select_channel(adc_channels_t __ch);
void adc_init(void);

// MOVING AVERAGE BELOW //
//
void init_buffers(void);

#define cbuf_adc0_SIZE          1      // size of buffers
#define cbuf_adc0_SIZE_2        0       // size of buffers in 2^n
volatile struct cbuf_adc0{
    uint8_t     m_getIdx;
    uint8_t     m_putIdx;
#ifdef ADC_8BITS
    uint8_t     m_entry[cbuf_adc0_SIZE];
#else
    uint16_t    m_entry[cbuf_adc0_SIZE];
#endif

} cbuf_adc0;
#ifdef ADC_8BITS
uint8_t calc_ma_adc0(void);
uint8_t ma_adc0;
uint16_t ma_adc0_sum;
uint8_t avg_adc0;
#else
uint16_t calc_ma_adc0(void);
uint16_t ma_adc0;
uint32_t ma_adc0_sum;
uint16_t avg_adc0;
#endif

#define cbuf_adc1_SIZE          1      // size of buffers
#define cbuf_adc1_SIZE_2        0       // size of buffers in 2^n
volatile struct cbuf_adc1{
    uint8_t     m_getIdx;
    uint8_t     m_putIdx;
#ifdef ADC_8BITS
    uint8_t     m_entry[cbuf_adc1_SIZE];
#else
    uint16_t    m_entry[cbuf_adc1_SIZE];
#endif
} cbuf_adc1; 
#ifdef ADC_8BITS
uint8_t calc_ma_adc1(void);
uint8_t ma_adc1;
uint16_t ma_adc1_sum;
uint8_t avg_adc1;
#else
uint16_t calc_ma_adc1(void);
uint16_t ma_adc1;
uint32_t ma_adc1_sum;
uint16_t avg_adc1;
#endif

#define cbuf_adc2_SIZE          1      // size of buffers
#define cbuf_adc2_SIZE_2        0       // size of buffers in 2^n
volatile struct cbuf_adc2{
    uint8_t     m_getIdx;
    uint8_t     m_putIdx;
#ifdef ADC_8BITS
    uint8_t     m_entry[cbuf_adc2_SIZE];
#else
    uint16_t    m_entry[cbuf_adc2_SIZE];
#endif
} cbuf_adc2;  
#ifdef ADC_8BITS
uint8_t calc_ma_adc2(void);
uint8_t ma_adc2;
uint16_t ma_adc2_sum;
uint8_t avg_adc2;
#else
uint16_t calc_ma_adc2(void);
uint16_t ma_adc2;
uint32_t ma_adc2_sum;
uint16_t avg_adc2;
#endif

#endif /* ifndef _ADC_H_ */
