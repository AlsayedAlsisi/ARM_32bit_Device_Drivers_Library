/*********************************************************************************************************************
* Author : Alsayed Alsisi
* Date   : Saturday, November 07, 2020
* Version: 1.0
* Contact: alsayed.alsisi@gmail.com
* License:
* You have the right to use the file as you wish in any educational or  commercial  purposes  under  the  following
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
*  rcc_stm32f10xxx.c
*
** Description:
*  This file contains the function definitions for the device driver of RCC peripheral of the stm32f10xxx
*  family of microcontroller. This file can be used with any of the following members of this family:
*  stm32f103xx,
*********************************************************************************************************************/


/*********************************************************************************************************************
                                               << File Inclusions >>
*********************************************************************************************************************/
#include <stdint.h>   /** Requires C99 compiler, or to be provided by older compilers **/

#include "rcc_stm32f10xxx.h"
#include "bitmath.h"
/*********************************************************************************************************************
                                              << Private Constants >>
*********************************************************************************************************************/
/************** RCC Peripheral Register Definitions ***************/
#define   RCC_CR         *((volatile uint32_t*)0x40021000)
#define   RCC_CFGR       *((volatile uint32_t*)0x40021004)
#define   RCC_CIR        *((volatile uint32_t*)0x40021008)
#define   RCC_APB2RSTR   *((volatile uint32_t*)0x4002100C)
#define   RCC_APB1RSTR   *((volatile uint32_t*)0x40021010)
#define   RCC_AHBENR     *((volatile uint32_t*)0x40021014)
#define   RCC_APB2ENR    *((volatile uint32_t*)0x40021018)
#define   RCC_APB1ENR    *((volatile uint32_t*)0x4002101C)
#define   RCC_BDCR       *((volatile uint32_t*)0x40021020)
#define   RCC_CSR        *((volatile uint32_t*)0x40021024)

/********************************************** Register Bit Positions ********************************************/
/****************************************** RCC_CR Register Bit Positions *****************************************/
#define   PLLRDY         (25U)
#define   PLLON          (24U)
#define   CSSON          (19U)
#define   HSEBYP         (18U)
#define   HSERDY         (17U)
#define   HSEON          (16U)
/*
#define   HSICAL_BITS_MASK_WITH_ONEs    (U)
#define   HSITRIM_BITS_MASK_WITH_ZEROs   (U)
*/
#define   HSIRDY         (1U)
#define   HSION          (0U)

/****************************************** RCC_CFGR Register Bit Positions and Related MACROs ********************/
#define   MCO_BITS_MASK_WITH_ZEROs          (0xF8FFFFFF)  /* Bits 26:24 MCO: Microcontroller clock output*/
#define   USBPRE                            (22U)         /* Bit  22 USBPRE: USB prescaler */
#define   PLLMUL_BITS_MASK_WITH_ZEROs       (0xFFC3FFFF)  /* Bits 21:18 PLLMUL: PLL multiplication factor*/
#define   PLLXTPRE                          (17U)         /* Bit  17 PLLXTPRE: HSE divider for PLL entry */
#define   PLLSRC                            (16U)         /* Bit  16 PLLSRC: PLL entry clock source*/
#define   ADCPRE_BITS_MASK_WITH_ZEROs       (0xFFFF3FFF)  /* Bits 15:14 ADCPRE: ADC prescaler */
#define   PPRE2_BITS_MASK_WITH_ZEROs        (0xFFFFC7FF)  /* Bits 13:11 PPRE2: APB high-speed prescaler (APB2) */
#define   PPRE1_BITS_MASK_WITH_ZEROs        (0xFFFFF8FF)  /* Bits 10:8 PPRE1: APB low-speed prescaler (APB1) */
#define   HPRE_BITS_MASK_WITH_ZEROs         (0xFFFFFF0F)  /* Bits 7:4 HPRE: AHB prescaler */
#define   SWS_BITS_MASK_WITH_ONEs           (0x0000000C)  /* Bits 3:2 SWS: System clock switch status */
#define   SW_BITS_MASK_WITH_ZEROs           (0xFFFFFFFC)  /* Bits 1:0 SW: System clock switch MACROs  */

/* Bits 26:24 MCO: Microcontroller clock output MACROs*/
#define   MCO_NO_CLOCK                      (0xF8FFFFFF) /* Represents the value: 0xx: No clock */
#define   MCO_SYSTEM_CLOCK_SELECTED         (0xFCFFFFFF) /* Represents the value: 100: System clock (SYSCLK) selected */
#define   MCO_HSI_CLOCK_SELECTED            (0xFDFFFFFF) /* Represents the value: 101: HSI clock selected */
#define   MCO_HSE_CLOCK_SELECTED            (0xFEFFFFFF) /* Represents the value: 110: HSE clock selected */
#define   MCO_PLL_CLOCK_DIV_BY_2_SELECTED   (0xFFFFFFFF) /* Represents the value: 111: PLL clock divided by 2 selected */
/* Bits 21:18 PLLMUL: PLL multiplication factor MACROs*/
#define   PLL_INPUT_CLOCK_x_2_SELECTED      (0x00000000) /* Represents the value: 0000: PLL input clock x 2  */
#define   PLL_INPUT_CLOCK_x_3_SELECTED      (0x00040000) /* Represents the value: 0001: PLL input clock x 3  */
#define   PLL_INPUT_CLOCK_x_4_SELECTED      (0x00080000) /* Represents the value: 0010: PLL input clock x 4  */
#define   PLL_INPUT_CLOCK_x_5_SELECTED      (0x000c0000) /* Represents the value: 0011: PLL input clock x 5  */
#define   PLL_INPUT_CLOCK_x_6_SELECTED      (0x00100000) /* Represents the value: 0100: PLL input clock x 6  */
#define   PLL_INPUT_CLOCK_x_7_SELECTED      (0x00140000) /* Represents the value: 0101: PLL input clock x 7  */
#define   PLL_INPUT_CLOCK_x_8_SELECTED      (0x00180000) /* Represents the value: 0110: PLL input clock x 8  */
#define   PLL_INPUT_CLOCK_x_9_SELECTED      (0x001C0000) /* Represents the value: 0111: PLL input clock x 9  */
#define   PLL_INPUT_CLOCK_x_10_SELECTED     (0x00200000) /* Represents the value: 1000: PLL input clock x 10 */
#define   PLL_INPUT_CLOCK_x_11_SELECTED     (0x00240000) /* Represents the value: 1001: PLL input clock x 11 */
#define   PLL_INPUT_CLOCK_x_12_SELECTED     (0x00280000) /* Represents the value: 1010: PLL input clock x 12 */
#define   PLL_INPUT_CLOCK_x_13_SELECTED     (0x002C0000) /* Represents the value: 1011: PLL input clock x 13 */
#define   PLL_INPUT_CLOCK_x_14_SELECTED     (0x00300000) /* Represents the value: 1100: PLL input clock x 14 */
#define   PLL_INPUT_CLOCK_x_15_SELECTED     (0x00340000) /* Represents the value: 1101: PLL input clock x 15 */
#define   PLL_INPUT_CLOCK_x_16_SELECTED     (0x00380000) /* Represents the value: 1111: PLL input clock x 16 */

/* Bits 15:14 ADCPRE: ADC prescaler MACROs */
#define   PCLK2_DIV_BY_2                    (0x00003000)  /* Represents the value: 00: PCLK2 divided by 2 */
#define   PCLK2_DIV_BY_4                    (0x00007000)  /* Represents the value: 01: PCLK2 divided by 4 */
#define   PCLK2_DIV_BY_6                    (0x0000B000)  /* Represents the value: 10: PCLK2 divided by 6 */
#define   PCLK2_DIV_BY_8                    (0x0000F000)  /* Represents the value: 11: PCLK2 divided by 8 */


/* Bits 13:11 PPRE2: APB high-speed prescaler (APB2) */
#define   PPRE2_HCLK_NOT_DIVIDED            (0x00000000)  /* Represents the value: 0xx: HCLK not divided */
#define   PPRE2_HCLK_DIV_BY_2               (0x00002000)  /* Represents the value: 100: HCLK divided by 2 */
#define   PPRE2_HCLK_DIV_BY_4               (0x00002800)  /* Represents the value: 101: HCLK divided by 4 */
#define   PPRE2_HCLK_DIV_BY_8               (0x00003000)  /* Represents the value: 110: HCLK divided by 8 */
#define   PPRE2_HCLK_DIV_BY_16              (0x00003800)  /* Represents the value: 111: HCLK divided by 16 */

/* Bits 10:8 PPRE1:  APB low-speed prescaler  (APB1) */
#define   PPRE1_HCLK_NOT_DIVIDED            (0x00000000)  /* Represents the value: 0xx: HCLK not divided */
#define   PPRE1_HCLK_DIV_BY_2               (0x00000400)  /* Represents the value: 100: HCLK divided by 2 */
#define   PPRE1_HCLK_DIV_BY_4               (0x00000500)  /* Represents the value: 101: HCLK divided by 4 */
#define   PPRE1_HCLK_DIV_BY_8               (0x00000600)  /* Represents the value: 110: HCLK divided by 8 */
#define   PPRE1_HCLK_DIV_BY_16              (0x00000700)  /* Represents the value: 111: HCLK divided by 16 */

/* Bits 7:4 HPRE: AHB prescaler */
#define   SYSCLK_NOT_DIVIDED                (0x00000000)  /* Represents the value: 0xxx: SYSCLK not divided    */
#define   SYSCLK_DIV_BY_2                   (0x00008000)  /* Represents the value: 1000: SYSCLK divided by 2   */
#define   SYSCLK_DIV_BY_4                   (0x00009000)  /* Represents the value: 1001: SYSCLK divided by 4   */
#define   SYSCLK_DIV_BY_8                   (0x0000A000)  /* Represents the value: 1010: SYSCLK divided by 8   */
#define   SYSCLK_DIV_BY_16                  (0x0000B000)  /* Represents the value: 1011: SYSCLK divided by 16  */
#define   SYSCLK_DIV_BY_64                  (0x0000C000)  /* Represents the value: 1100: SYSCLK divided by 64  */
#define   SYSCLK_DIV_BY_128                 (0x0000D000)  /* Represents the value: 1101: SYSCLK divided by 128 */
#define   SYSCLK_DIV_BY_256                 (0x0000E000)  /* Represents the value: 1110: SYSCLK divided by 256 */
#define   SYSCLK_DIV_BY_512                 (0x0000F000)  /* Represents the value: 1111: SYSCLK divided by 512 */
/* Bits 3:2 SWS: System clock switch status MACROs */
#define   SWS_HSI_OSCILLATOR_USED           (0x00000000) /* Represents the value: 00: HSI oscillator used as system clock */
#define   SWS_HSE_OSCILLATOR_USED           (0x00000004) /* Represents the value: 01: HSE oscillator used as system clock */
#define   SWS_PLL_USED                      (0x00000008) /* Represents the value: 10: PLL used as system clock */
/* Bits 1:0 SW: System clock switch MACROs */
#define   SW_HSI_OSCILLATOR_SELECTED        (0x00000000) /* Represents the value: 00: HSI selected as system clock */
#define   SW_HSE_OSCILLATOR_SELECTED        (0x00000001) /* Represents the value: 01: HSE selected as system clock */
#define   SW_PLL_SELECTED                   (0x00000002) /* Represents the value: 10: PLL selected as system clock */


/** RCC_CIR       Register Bit Positions **/
/** RCC_APB2RSTR  Register Bit Positions **/
/** RCC_APB1RSTR  Register Bit Positions **/
/** RCC_AHBENR Register Bit Positions **/
#define   SDIOEN    (10U)
#define   FSMCEN    (8U)
#define   CRCEN     (6U)
#define   FLITFEN   (4U)
#define   SRAMEN    (2U)
#define   DMA2EN    (1U)
#define   DMA1EN    (0U)

/****************************************** RCC_APB2ENR Register Bit Positions and Related MACROs ********************/
#define   AFIOEN     (0U)
#define   IOPAEN     (2U)
#define   IOPBEN     (3U)
#define   IOPCEN     (4U)
#define   IOPDEN     (5U)
#define   IOPEEN     (6U)
#define   IOPFEN     (7U)
#define   IOPGEN     (8U)
#define   ADC1EN     (9U)
#define   ADC2EN     (10U)
#define   TIM1EN     (11U)
#define   SPI1EN     (12U)
#define   TIM8EN     (13U)
#define   USART1EN   (14U)
#define   ADC3EN     (15U)
#define   TIM9EN     (19U)
#define   TIM10EN    (20U)
#define   TIM11EN    (21U)

/****************************************** RCC_APB1ENR Register Bit Positions and Related MACROs ********************/
#define   TIM2EN     (0U)
#define   TIM3EN     (1U)
#define   TIM4EN     (2U)
#define   TIM5EN     (3U)
#define   TIM6EN     (4U)
#define   TIM7EN     (5U)
#define   TIM12EN    (6U)
#define   TIM13EN    (7U)
#define   TIM14EN    (8U)
#define   WWDGEN     (11U)
#define   SPI2EN     (14U)
#define   SPI3EN     (15U)
#define   USART2EN   (17U)
#define   USART3EN   (18U)
#define   USART4EN   (19U)
#define   UART5EN    (20U)
#define   I2C1EN     (21U)
#define   I2C2EN     (22U)
#define   USBEN      (23U)
#define   CANEN      (25U)
#define   BKPEN      (27U)
#define   PWREN      (28U)
#define   DACEN      (29U)
/****************************************** RCC_BDCR Register Bit Positions and Related MACROs ********************/

/****************************************** RCC_CSR Register Bit Positions and Related MACROs ********************/



/*********************************************************************************************************************
                                              << Private Data Types >>
*********************************************************************************************************************/

/*********************************************************************************************************************
                                          << Private Variable Definitions >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                          << Private Variable Definitions >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                         << Private Functions Prototypes >>
*********************************************************************************************************************/
static rcc_std_return_error_t pll_multiplication_factor_select(rcc_pll_multiplication_factor_t pll_multiplication_factor);
static rcc_std_return_error_t pll_input_source_select(rcc_pll_input_source_t pll_input_source);

/*********************************************************************************************************************
                                          << Public Function Definitions >>
*********************************************************************************************************************/
/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/
rcc_std_return_error_t rcc_clock_source_enable(rcc_clock_source_t clock_source_enabled)
{
    /******************* Local Variables **********************/
	rcc_std_return_error_t l_return_error= E_OK;

	switch(clock_source_enabled)
	{
		case RCC_CLOCK_INPUT_EXTERNAL:
	    BIT_SET(RCC_CR, HSEON);
	    break;

	    case RCC_CLOCK_INPUT_INTERNAL:
	    BIT_SET(RCC_CR, HSION);
	    break;

		case RCC_CLOCK_INPUT_PLL:
		BIT_SET(RCC_CR, PLLON);
		break;

	    default:
	    l_return_error = E_NOT_OK; /* Can be used in debugging */
    }

	return l_return_error;
}

/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/
rcc_std_return_error_t rcc_clock_source_disable(rcc_clock_source_t clock_source_disabled)
{
	    /******************* Local Variables **********************/
	rcc_std_return_error_t l_return_error = E_OK;

	switch(clock_source_disabled)
	{
	    case RCC_CLOCK_INPUT_EXTERNAL:
	    BIT_CLEAR(RCC_CR, HSEON);
	    break;

	    case RCC_CLOCK_INPUT_INTERNAL:
	    BIT_CLEAR(RCC_CR, HSION);
	    break;

		case RCC_CLOCK_INPUT_PLL:
		BIT_CLEAR(RCC_CR, PLLON);
		break;

	    default:
	    l_return_error = E_NOT_OK; /* Can be used in debugging */
    }

	return l_return_error;
}

/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/
rcc_std_return_error_t rcc_system_clock_source_select(rcc_clock_source_t system_clock_source_selected)
{
    /******************* Local Variables **********************/
	rcc_std_return_error_t l_ReturnError = E_OK;

	/* Selecting system clock input source based on 'SystemClockSourceSelected' parameter */
	switch(system_clock_source_selected)
	{
	    case RCC_CLOCK_INPUT_INTERNAL:
	    RCC_CFGR = ((RCC_CFGR & SW_BITS_MASK_WITH_ZEROs) | SW_HSI_OSCILLATOR_SELECTED);
	    break;
		case RCC_CLOCK_INPUT_EXTERNAL:
	    RCC_CFGR = ((RCC_CFGR & SW_BITS_MASK_WITH_ZEROs) | SW_HSE_OSCILLATOR_SELECTED);
	    break;
     	case RCC_CLOCK_INPUT_PLL:
		RCC_CFGR = ((RCC_CFGR & SW_BITS_MASK_WITH_ZEROs) | SW_PLL_SELECTED);
		break;
	    default:
	    l_ReturnError = E_NOT_OK; /* Can be used in debugging */
    }

	return l_ReturnError;
}

/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/
rcc_std_return_error_t rcc_system_clock_source_getinputused(rcc_clock_source_t* system_clock_source_used)
{
	/******************* Local Variables **********************/
	rcc_std_return_error_t l_return_error = E_OK;
	uint32_t rcc_CFGR_SWS_bits_value;
	rcc_CFGR_SWS_bits_value = (RCC_CFGR & SWS_BITS_MASK_WITH_ONEs); /* Masking the SWS bits in the RCC_CFGR register */

	switch(rcc_CFGR_SWS_bits_value)
	{
		case SWS_HSI_OSCILLATOR_USED:
		*system_clock_source_used = RCC_CLOCK_INPUT_INTERNAL;
		break;
		case SWS_HSE_OSCILLATOR_USED:
		*system_clock_source_used = RCC_CLOCK_INPUT_EXTERNAL;
		break;
		case SWS_PLL_USED:
		*system_clock_source_used = RCC_CLOCK_INPUT_PLL;
		break;
		default:
		/* Do Nothing*/
		break;
	}

	return l_return_error;
}

/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/
rcc_std_return_error_t rcc_peripheral_clock_enable(rcc_Peripheral_id_t peripheral_id)
{
	/******************* Local Variables **********************/
	rcc_std_return_error_t l_return_error = E_OK;

	switch(peripheral_id)
	{
		/** AHB Peripherals **/
		case SDIO:
        BIT_SET(RCC_AHBENR, SDIOEN);
		break;

		case FSMC:
		BIT_SET(RCC_AHBENR, FSMCEN);
		break;

		case CRC:
		BIT_SET(RCC_AHBENR, CRCEN);
		break;

		case FLITF:
		BIT_SET(RCC_AHBENR, FLITFEN);
		break;

		case SRAM:
		BIT_SET(RCC_AHBENR, SRAMEN);
		break;

		case DMA2:
		BIT_SET(RCC_AHBENR, DMA2EN);
		break;

		case DMA1:
		BIT_SET(RCC_AHBENR, DMA1EN);
		break;

		/** APB2 Peripherals **/
		case AFIO:
        BIT_SET(RCC_APB2ENR, AFIOEN);
		break;
		case GPIO_PORT_A:
        BIT_SET(RCC_APB2ENR, IOPAEN);
		break;
		case GPIO_PORT_B:
        BIT_SET(RCC_APB2ENR, IOPBEN);
		break;
		case GPIO_PORT_C:
        BIT_SET(RCC_APB2ENR, IOPCEN);
		break;
		case GPIO_PORT_D:
        BIT_SET(RCC_APB2ENR, IOPDEN);
		break;
		case GPIO_PORT_E:
        BIT_SET(RCC_APB2ENR, IOPEEN);
		break;
		case GPIO_PORT_F:
        BIT_SET(RCC_APB2ENR, IOPFEN);
		break;
		case GPIO_PORT_G:
        BIT_SET(RCC_APB2ENR, IOPGEN);
		break;
		case ADC1:
        BIT_SET(RCC_APB2ENR, ADC1EN);
		break;
		case ADC2:
        BIT_SET(RCC_APB2ENR, ADC2EN);
		break;
		case TIM1_TIMER:
        BIT_SET(RCC_APB2ENR, TIM1EN);
		break;
		case SPI1:
        BIT_SET(RCC_APB2ENR, SPI1EN);
		break;
		case TIM8_TIMER:
        BIT_SET(RCC_APB2ENR, TIM8EN);
		break;
		case USART1:
        BIT_SET(RCC_APB2ENR, USART1EN);
		break;
		case ADC3:
        BIT_SET(RCC_APB2ENR, ADC3EN);
		break;
		case TIM9_TIMER:
        BIT_SET(RCC_APB2ENR, TIM9EN);
		break;
		case TIM10_TIMER:
        BIT_SET(RCC_APB2ENR, TIM10EN);
		break;
		case TIM11_TIMER:
        BIT_SET(RCC_APB2ENR, TIM11EN);
		break;

		/** APB1 Peripherals **/
		case TIM2_TIMER:
        BIT_SET(RCC_APB1ENR, TIM2EN);
		break;
		case TIM3_TIMER:
        BIT_SET(RCC_APB1ENR, TIM3EN);
		break;
		case TIM4_TIMER:
        BIT_SET(RCC_APB1ENR, TIM4EN);
		break;
		case TIM5_TIMER:
        BIT_SET(RCC_APB1ENR, TIM5EN);
		break;
		case TIM6_TIMER:
        BIT_SET(RCC_APB1ENR, TIM6EN);
		break;
		case TIM7_TIMER:
        BIT_SET(RCC_APB1ENR, TIM7EN);
		break;
		case TIM12_TIMER:
        BIT_SET(RCC_APB1ENR, TIM12EN);
		break;
		case TIM13_TIMER:
        BIT_SET(RCC_APB1ENR, TIM13EN);
		break;
		case TIM14_TIMER:
        BIT_SET(RCC_APB1ENR, TIM14EN);
		break;
		case WINDOW_WATCHDOG_WWDG:
        BIT_SET(RCC_APB1ENR, WWDGEN);
		break;
		case SPI2:
        BIT_SET(RCC_APB1ENR, SPI2EN);
		break;
		case SPI3:
        BIT_SET(RCC_APB1ENR, SPI3EN);
		break;
		case USART2:
        BIT_SET(RCC_APB1ENR, USART2EN);
		break;
		case USART3:
        BIT_SET(RCC_APB1ENR, USART3EN);
		break;
		case USART4:
        BIT_SET(RCC_APB1ENR, USART4EN);
		break;
		case UART5:
        BIT_SET(RCC_APB1ENR, UART5EN);
		break;
		case I2C1:
        BIT_SET(RCC_APB1ENR, I2C1EN);
		break;
		case I2C2:
        BIT_SET(RCC_APB1ENR, I2C2EN);
		break;
		case USB:
        BIT_SET(RCC_APB1ENR, USBEN);
		break;
		case CAN:
        BIT_SET(RCC_APB1ENR, CANEN);
		break;
		case BACKUP_REGISTERS_BKP:
        BIT_SET(RCC_APB1ENR, BKPEN);
		break;
		case POWER_CONTROL_PWR:
        BIT_SET(RCC_APB1ENR, PWREN);
		break;
		case DAC:
        BIT_SET(RCC_APB1ENR, DACEN);
		break;

		default:
			l_return_error = E_NOT_OK; /** Can be used in debugging **/
    }

    return l_return_error;
}

/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/

rcc_std_return_error_t rcc_peripheral_clock_disable(rcc_Peripheral_id_t peripheral_id)
{
	/******************* Local Variables **********************/
	rcc_std_return_error_t l_return_error_peripheral_id = E_OK;

	switch(peripheral_id)
	{
		/** AHB Peripherals **/
		case SDIO:
        BIT_CLEAR(RCC_AHBENR, SDIOEN);
		break;
		case FSMC:
		BIT_CLEAR(RCC_AHBENR, FSMCEN);
		break;
		case CRC:
		BIT_CLEAR(RCC_AHBENR, CRCEN);
		break;
		case FLITF:
		BIT_CLEAR(RCC_AHBENR, FLITFEN);
		break;
        case SRAM:
		BIT_CLEAR(RCC_AHBENR, SRAMEN);
		break;
		case DMA2:
		BIT_CLEAR(RCC_AHBENR, DMA2EN);
		break;
		case DMA1:
		BIT_CLEAR(RCC_AHBENR, DMA1EN);
		break;

		/** APB2 Peripherals **/
		case AFIO:
        BIT_CLEAR(RCC_APB2ENR, AFIOEN);
		break;
		case GPIO_PORT_A:
        BIT_CLEAR(RCC_APB2ENR, IOPAEN);
		break;
		case GPIO_PORT_B:
        BIT_CLEAR(RCC_APB2ENR, IOPBEN);
		break;
		case GPIO_PORT_C:
        BIT_CLEAR(RCC_APB2ENR, IOPCEN);
		break;
		case GPIO_PORT_D:
        BIT_CLEAR(RCC_APB2ENR, IOPDEN);
		break;
		case GPIO_PORT_E:
        BIT_CLEAR(RCC_APB2ENR, IOPEEN);
		break;
		case GPIO_PORT_F:
        BIT_CLEAR(RCC_APB2ENR, IOPFEN);
		break;
		case GPIO_PORT_G:
        BIT_CLEAR(RCC_APB2ENR, IOPGEN);
		break;
		case ADC1:
        BIT_CLEAR(RCC_APB2ENR, ADC1EN);
		break;
		case ADC2:
        BIT_CLEAR(RCC_APB2ENR, ADC2EN);
		break;
		case TIM1_TIMER:
        BIT_CLEAR(RCC_APB2ENR, TIM1EN);
		break;
		case SPI1:
        BIT_CLEAR(RCC_APB2ENR, SPI1EN);
		break;
		case TIM8_TIMER:
        BIT_CLEAR(RCC_APB2ENR, TIM8EN);
		break;
		case USART1:
        BIT_CLEAR(RCC_APB2ENR, USART1EN);
		break;
		case ADC3:
        BIT_CLEAR(RCC_APB2ENR, ADC3EN);
		break;
		case TIM9_TIMER:
        BIT_CLEAR(RCC_APB2ENR, TIM9EN);
		break;
		case TIM10_TIMER:
        BIT_CLEAR(RCC_APB2ENR, TIM10EN);
		break;
		case TIM11_TIMER:
        BIT_CLEAR(RCC_APB2ENR, TIM11EN);
		break;

		/** APB1 Peripherals **/
		case TIM2_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM2EN);
		break;
		case TIM3_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM3EN);
		break;
		case TIM4_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM4EN);
		break;
		case TIM5_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM5EN);
		break;
		case TIM6_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM6EN);
		break;
		case TIM7_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM7EN);
		break;
		case TIM12_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM12EN);
		break;
		case TIM13_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM13EN);
		break;
		case TIM14_TIMER:
        BIT_CLEAR(RCC_APB1ENR, TIM14EN);
		break;
		case WINDOW_WATCHDOG_WWDG:
        BIT_CLEAR(RCC_APB1ENR, WWDGEN);
		break;
		case SPI2:
        BIT_CLEAR(RCC_APB1ENR, SPI2EN);
		break;
		case SPI3:
        BIT_CLEAR(RCC_APB1ENR, SPI3EN);
		break;
		case USART2:
        BIT_CLEAR(RCC_APB1ENR, USART2EN);
		break;
		case USART3:
        BIT_CLEAR(RCC_APB1ENR, USART3EN);
		break;
		case USART4:
        BIT_CLEAR(RCC_APB1ENR, USART4EN);
		break;
		case UART5:
        BIT_CLEAR(RCC_APB1ENR, UART5EN);
		break;
		case I2C1:
        BIT_CLEAR(RCC_APB1ENR, I2C1EN);
		break;
		case I2C2:
        BIT_CLEAR(RCC_APB1ENR, I2C2EN);
		break;
		case USB:
        BIT_CLEAR(RCC_APB1ENR, USBEN);
		break;
		case CAN:
        BIT_CLEAR(RCC_APB1ENR, CANEN);
		break;
		case BACKUP_REGISTERS_BKP:
        BIT_CLEAR(RCC_APB1ENR, BKPEN);
		break;
		case POWER_CONTROL_PWR:
        BIT_CLEAR(RCC_APB1ENR, PWREN);
		break;
		case DAC:
        BIT_CLEAR(RCC_APB1ENR, DACEN);
		break;

		default:
		l_return_error_peripheral_id = E_NOT_OK; /** Can be used in debugging **/
    }

    return (l_return_error_peripheral_id);

}

/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t:
*
*********************************************************************************************************************/
rcc_std_return_error_t rcc_pll_clock_config(rcc_pll_input_source_t pll_input_source, rcc_pll_multiplication_factor_t pll_multiplication_factor)
{
	/******************* Local Variables **********************/
	rcc_std_return_error_t l_return_error_pll_input_source          = E_OK;
	rcc_std_return_error_t l_return_error_pll_multiplication_factor = E_OK;

	l_return_error_pll_input_source           = pll_input_source_select(pll_input_source);
	l_return_error_pll_multiplication_factor = pll_multiplication_factor_select(pll_multiplication_factor);

	return (l_return_error_pll_input_source || l_return_error_pll_multiplication_factor);
}



/*********************************************************************************************************************
                                          << Private Function Definitions >>
*********************************************************************************************************************/
/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/
static rcc_std_return_error_t pll_input_source_select(rcc_pll_input_source_t pll_input_source)
{
    /******************* Local Variables **********************/
	rcc_std_return_error_t l_ReturnError = E_OK;

	switch(pll_input_source)
	{
		/*  */
		case PLL_INPUT_HSI_DIVIDED_BY_2:
		BIT_CLEAR(RCC_CFGR, PLLSRC);
		BIT_CLEAR(RCC_CFGR, PLLXTPRE);
		break;

		/*  */
		case PLL_INPUT_HSE_DIVIDED_BY_2:
		BIT_SET(RCC_CFGR, PLLXTPRE);
		BIT_SET(RCC_CFGR, PLLSRC);
		break;
		/*  */
		case PLL_INPUT_HSE:
		BIT_CLEAR(RCC_CFGR, PLLXTPRE);
		BIT_SET(RCC_CFGR, PLLSRC);
		break;
		default:
		l_ReturnError = E_NOT_OK; /** Can be used in debugging **/
	}

	return l_ReturnError;
}


/*********************************************************************************************************************
** Function Name:
*
*
** Description:
*
*
** Input Parameters:
*  - :
*    This parameters used to pass to the function
*
** Return Value:
*  - rcc_std_return_error_t
*
*********************************************************************************************************************/
static rcc_std_return_error_t pll_multiplication_factor_select(rcc_pll_multiplication_factor_t pll_multiplication_factor)
{
	rcc_std_return_error_t l_ReturnError = E_OK;

	switch(pll_multiplication_factor)
	{
		case PLL_INPUT_CLOCK_x_2:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_2_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_3:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_3_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_4:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_4_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_5:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_5_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_6:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_6_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_7:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_7_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_8:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_8_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_9:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_9_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_10:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_10_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_11:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_11_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_12:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_12_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_13:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_13_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_14:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_14_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_15:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_15_SELECTED);
		break;
		case PLL_INPUT_CLOCK_x_16:
		RCC_CFGR = ((RCC_CFGR & PLLMUL_BITS_MASK_WITH_ZEROs) | PLL_INPUT_CLOCK_x_16_SELECTED);
		break;
		default:
		l_ReturnError = E_NOT_OK;
	}

	return l_ReturnError;
}
/*********************************************************************************************************************
                                          << End of File >>
*********************************************************************************************************************/
