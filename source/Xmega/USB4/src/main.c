#include <asf.h>
#include "conf_usb.h"

#define RAM_BUFFER_SIZE_2000   2000
#define RAM_BUFFER_SIZE_1000   1000

#define ADC_GAIN               1
#define ADC_NR_TRIGGER_CHANNEL 2
#define ADC_EVENT_CHANNEL      3
#define ADC_CLOCK_RATE         4000000

#define DMA_CHANNEL_0          0
#define DMA_CHANNEL_1          1

#define TIMER_RESOLUTION       1000000
#define TIMER_PERIOD           1

#define PWM_FREQUENCY          12500
#define PWM_DUTY_CYCLE         50

#define IO_TRIGGER             IOPORT_CREATE_PIN(PORTC, 1)
#define IO_CTRL_LED            IOPORT_CREATE_PIN(PORTC, 4)

static void adc_init(void);
static void dma_init(void);
static void evsys_init(void);
static void tc_init(void);
static void pwm_enable(void);

struct buffer
{
	uint8_t buffer_channel_0[RAM_BUFFER_SIZE_1000];
	uint8_t buffer_channel_1[RAM_BUFFER_SIZE_1000];
};

struct buffer buffer_samples;
char trigger_char;

int main(void)
{
	sysclk_init();
	pmic_init();
	adc_init();
	dma_init();
	evsys_init();
	tc_init();
	ioport_init();
	
	ioport_set_pin_dir(IO_TRIGGER, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(IO_TRIGGER, IOPORT_MODE_PULLUP);
	ioport_set_pin_dir(IO_CTRL_LED, IOPORT_DIR_OUTPUT);
	
	cpu_irq_enable();
	adc_enable(&ADCA);
	pwm_enable();
	udc_start();
	
	while (1)
	{
		while(!udi_cdc_getc());
		while (0 == ioport_get_pin_level(IO_TRIGGER));
		while (1 == ioport_get_pin_level(IO_TRIGGER));
		
		dma_channel_enable(DMA_CHANNEL_0);
		dma_channel_enable(DMA_CHANNEL_1);

		if (udi_cdc_is_tx_ready())
		{
			while(dma_channel_is_busy(DMA_CHANNEL_1));
			udi_cdc_write_buf(&buffer_samples, RAM_BUFFER_SIZE_2000);
			ioport_toggle_pin_level(IO_CTRL_LED);
		}
	}
}

static void evsys_init(void)
{
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	EVSYS.CH3MUX = EVSYS_CHMUX_TCC1_OVF_gc;
}
static void tc_init(void)
{
	tc_enable(&TCC1);
	tc_set_wgm(&TCC1, TC_WG_NORMAL);
	tc_write_period(&TCC1, TIMER_PERIOD);
	tc_set_resolution(&TCC1, TIMER_RESOLUTION);
}

static void pwm_enable(void)
{
	struct pwm_config pwm_cfg0;
	
	pwm_init(&pwm_cfg0, PWM_TCC0, PWM_CH_A, PWM_FREQUENCY);
	pwm_start(&pwm_cfg0, PWM_DUTY_CYCLE);
}

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(&ADCA, &adc_conf);
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_VCC);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SWEEP, ADC_NR_TRIGGER_CHANNEL, ADC_EVENT_CHANNEL);
	adc_set_clock_rate(&adc_conf, ADC_CLOCK_RATE);
	adc_write_configuration(&ADCA, &adc_conf);
	
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, ADC_GAIN);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN5, ADCCH_NEG_NONE, ADC_GAIN);
	adcch_write_configuration(&ADCA, ADC_CH1, &adcch_conf);
}

static void dma_init(void){
	
	struct dma_channel_config dmach_conf0;
	struct dma_channel_config dmach_conf1;
	
	memset(&dmach_conf0, 0, sizeof(dmach_conf0));
	dma_channel_set_burst_length(&dmach_conf0, DMA_CH_BURSTLEN_1BYTE_gc);
	dma_channel_set_transfer_count(&dmach_conf0, RAM_BUFFER_SIZE_1000);
	dma_channel_set_src_reload_mode(&dmach_conf0, DMA_CH_SRCRELOAD_BURST_gc);
	dma_channel_set_dest_reload_mode(&dmach_conf0,DMA_CH_DESTRELOAD_TRANSACTION_gc);
	dma_channel_set_src_dir_mode(&dmach_conf0, DMA_CH_SRCDIR_INC_gc);
	dma_channel_set_dest_dir_mode(&dmach_conf0, DMA_CH_DESTDIR_INC_gc);
	dma_channel_set_source_address(&dmach_conf0,(uint16_t)(uintptr_t)&ADCA.CH0RES);
	dma_channel_set_destination_address(&dmach_conf0,(uint16_t)(uintptr_t)buffer_samples.buffer_channel_0);
	dma_channel_set_trigger_source(&dmach_conf0, DMA_CH_TRIGSRC_ADCA_CH0_gc );
	dma_channel_set_single_shot(&dmach_conf0);
	
	memset(&dmach_conf1, 0, sizeof(dmach_conf1));
	dma_channel_set_burst_length(&dmach_conf1, DMA_CH_BURSTLEN_1BYTE_gc);
	dma_channel_set_transfer_count(&dmach_conf1, RAM_BUFFER_SIZE_1000);
	dma_channel_set_src_reload_mode(&dmach_conf1, DMA_CH_SRCRELOAD_BURST_gc);
	dma_channel_set_dest_reload_mode(&dmach_conf1,DMA_CH_DESTRELOAD_TRANSACTION_gc);
	dma_channel_set_src_dir_mode(&dmach_conf1, DMA_CH_SRCDIR_INC_gc);
	dma_channel_set_dest_dir_mode(&dmach_conf1, DMA_CH_DESTDIR_INC_gc);
	dma_channel_set_source_address(&dmach_conf1,(uint16_t)(uintptr_t)&ADCA.CH1RES);
	dma_channel_set_destination_address(&dmach_conf1,(uint16_t)(uintptr_t)buffer_samples.buffer_channel_1);
	dma_channel_set_trigger_source(&dmach_conf1, DMA_CH_TRIGSRC_ADCA_CH1_gc );
	dma_channel_set_single_shot(&dmach_conf1);
	
	dma_enable();
	dma_channel_write_config(DMA_CHANNEL_0, &dmach_conf0);
	dma_channel_write_config(DMA_CHANNEL_1, &dmach_conf1);
}