
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void CLK_Config(void);
void Led_Config(void);
void Button_Config(void);
void EXTI_Config(void);
void NVIC_Config(void);
void Delay(uint32_t time);


int main(void)
{
	CLK_Config();
	Led_Config();
	Button_Config();
	EXTI_Config();
	NVIC_Config();
  while (1)
  {
	  GPIOD->ODR = 0x00000000;   // pins reset
  }
}


void CLK_Config() // Clock speed for 168MHz
{
	RCC->CR |= 0x00010000;                 // HSEON ENABLE
	while(!(RCC->CR & 0x00020000));        // HSEON Ready Flag wait
	RCC->CR |= 0x00080000;              // CSS ENABLE
	RCC->CR |= 0x01000000;				// PLL ON
	RCC->PLLCFGR |= 0x00400000;        // PLL SRC HSE is selected
	RCC->PLLCFGR |= 0x00000004;       // PLL M 4
	RCC->PLLCFGR |= 0x00005A00;        // PLL N 168
	RCC->PLLCFGR |= 0x00000000;       // PLL P 2
	RCC->CFGR |= 0x00000000;          // AHB PRESCALER 1
	RCC->CFGR |= 0x00080000;          // APB2 PRESCALER 2
	RCC->CFGR |= 0x00001400;          // APB1 PRESCALER 4
	RCC->CIR |= 0x00080000;             // HSE READY FLAG CLEAR
	RCC->CIR |= 0x00800000;             // CSS FLAG CLEAR
}

void Led_Config(void)  // User led configuration
{
	RCC->AHB1ENR |= 0x1U << 3U; // D port clock enable

	GPIOD->MODER |= 0x55000000; // pins D12, D13, D14, D15 is selected output mode
	GPIOD->OSPEEDR |= 0xFF000000; // very high speed is selected
	GPIOD->PUPDR |= 0x00000000; // no pull up, pull down
}

void Button_Config(void)
{
	RCC->AHB1ENR |= 0x1U << 1U; // A port clock enable

	GPIOD->MODER |= 0x00000000; // pin A0 is selected input mode
	GPIOD->OTYPER |= 0x00000000;  // output push-pull
	GPIOD->OSPEEDR |= 0x00000003; // very high speed is selected
	GPIOD->PUPDR |= 0x00000002; // pull down
}


void EXTI_Config(void)
{
	RCC->APB2ENR |= 1 << 14;     // exti clock bus is activated

	SYSCFG->EXTICR[0] |= 0x00000000;   // external interrupt configuration register is activated for A0 pin
	EXTI->IMR |= 1 << 0;               // Interrupt request from line 0 is not masked
	EXTI->RTSR |= 1 << 0;               // Rising trigger enabled for input line

}


void NVIC_Config(void)
{
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
}


void EXTI0_IRQHandler()
{
	if(EXTI->PR & 0x00000001)       // checked that the interrupt has occurred
	{
		 GPIOD->ODR = 0x0000F000;   // pins set
		 Delay(20000000);           // wait for 1 second
		 EXTI->PR |= 0x00000001;    // interrupt reset
	}
}


void Delay(uint32_t time)
{
	while(time--);
}






/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
