/*********************************************************************************************************************
* Author : Alsayed Alsisi
* Date   : Saturday, November 07, 2020
* Version: 1.0
* Contact: alsayed.alsisi@gmail.com
* License:
* You have the right to use the file as you wish in any educational or commercial purposes under the following
* conditions:
* - This file is to be used as is. No modifications are to be made to any part of the file, including this section.
* - This section is not to be removed under any circumstances.
* - Parts of the file may be used separately under the condition they are not modified, and preceded by this section.
* - Any bug encountered in this file or parts of it should be reported to the email address given above to be fixed.
* - No warranty is expressed or implied by the publication or distribution of this source code.
*********************************************************************************************************************/
/*********************************************************************************************************************
* File Information:
** File Name:
*  rcc_stm32f10xxx.h
* 
** Description:
*  This file contains the public programming interfaces for the device driver of RCC peripheral of the stm32f10xxx 
*  family of microcontroller. This file can be used with any of the following members of this family:
*  stm32f103xx, 
*********************************************************************************************************************/

/*********************************************************************************************************************
                                               << Header Guard >>
*********************************************************************************************************************/
#ifndef RCC_INTERFACE_H		
#define RCC_INTERFACE_H

/*********************************************************************************************************************
                                               << File Inclusions >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                               << Public Constants >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                               << Public Data Types >>
*********************************************************************************************************************/
typedef enum
{
	E_OK = 0,
	E_NOT_OK
	
} rcc_std_return_error_t;

typedef enum
{
	AHB = 0, 
	APB1, 
	APB2
} Idt_BusType_T;

typedef enum
{
	RCC_CLOCK_INPUT_EXTERNAL,
	RCC_CLOCK_INPUT_INTERNAL,
	RCC_CLOCK_INPUT_PLL
} rcc_clock_source_t;



typedef enum
{
	/** AHB Peripherals **/
		DMA1 = 0                    ,
		DMA2                        ,
		SRAM                        ,
		FLITF                       ,
		CRC                         ,
		FSMC                        ,
		SDIO                        ,
		/** APB2 Peripherals **/
		AFIO                        ,
		GPIO_PORT_A                 ,
		GPIO_PORT_B                 ,
		GPIO_PORT_C                 ,
		GPIO_PORT_D                 ,
		GPIO_PORT_E                 ,
		GPIO_PORT_F                 ,
		GPIO_PORT_G                 ,
		ADC1                        ,
		ADC2                        ,
		TIM1_TIMER                  ,
		SPI1                        ,
		TIM8_TIMER                  ,
		USART1                      ,
		ADC3                        ,
		TIM9_TIMER                  ,
		TIM10_TIMER                 ,
		TIM11_TIMER                 ,
		/** APB1 Peripherals **/
		TIM2_TIMER                  ,
		TIM3_TIMER                  ,
		TIM4_TIMER                  ,
		TIM5_TIMER                  ,
		TIM6_TIMER                  ,
		TIM7_TIMER                  ,
		TIM12_TIMER                 ,
		TIM13_TIMER                 ,
	    TIM14_TIMER                 ,
	    WINDOW_WATCHDOG_WWDG        ,
		SPI2                        ,
		SPI3                        ,
		USART2                      ,
		USART3                      ,
		USART4                      ,
		UART5                       ,
		I2C1                        ,
		I2C2                        ,
		USB                         ,
		CAN                         ,
		BACKUP_REGISTERS_BKP        ,
		POWER_CONTROL_PWR           ,
		DAC
} rcc_Peripheral_id_t;
typedef enum
{
	PLL_INPUT_HSI_DIVIDED_BY_2 = 0,
    PLL_INPUT_HSE_DIVIDED_BY_2,
	PLL_INPUT_HSE
} rcc_pll_input_source_t;


typedef enum
{
	PLL_INPUT_CLOCK_x_2 = 0,
	PLL_INPUT_CLOCK_x_3,
	PLL_INPUT_CLOCK_x_4,
	PLL_INPUT_CLOCK_x_5,
	PLL_INPUT_CLOCK_x_6,
	PLL_INPUT_CLOCK_x_7,
	PLL_INPUT_CLOCK_x_8,
	PLL_INPUT_CLOCK_x_9,
	PLL_INPUT_CLOCK_x_10,
	PLL_INPUT_CLOCK_x_11,
	PLL_INPUT_CLOCK_x_12,
	PLL_INPUT_CLOCK_x_13,
	PLL_INPUT_CLOCK_x_14,
	PLL_INPUT_CLOCK_x_15,
	PLL_INPUT_CLOCK_x_16
} rcc_pll_multiplication_factor_t;

/*********************************************************************************************************************
                                          << Public Variable Declarations >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                           << Public Function Declarations >>
*********************************************************************************************************************/

extern rcc_std_return_error_t rcc_clock_source_enable(rcc_clock_source_t clock_source_enabled);
extern rcc_std_return_error_t rcc_clock_source_disable(rcc_clock_source_t clock_source_disabled);
extern rcc_std_return_error_t rcc_system_clock_source_select(rcc_clock_source_t system_clock_source_selected);
extern rcc_std_return_error_t rcc_system_clock_source_getinputused(rcc_clock_source_t* system_clock_source_used);
extern rcc_std_return_error_t rcc_peripheral_clock_enable(rcc_Peripheral_id_t peripheral_id);
extern rcc_std_return_error_t rcc_peripheral_clock_disable(rcc_Peripheral_id_t peripheral_id);
extern rcc_std_return_error_t rcc_pll_clock_config(rcc_pll_input_source_t pll_input_source, rcc_pll_multiplication_factor_t pll_multiplication_factor);


#endif /* RCC_INTERFACE_H */

/*********************************************************************************************************************
---------------------------------------------------- End of File -----------------------------------------------------
*********************************************************************************************************************/
