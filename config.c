#include "config.h"
#include "Car.h"
/*注意：系统采用STM32F103VET6CPU，为STM32F10X_HD高容量，启动文件也需要为HD*/

/* 自定义同义关键字    --------------------------------------------------------*/
static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

/*******************************************************************************
* 函数名	: RCC_Configuration
* 函数描述  : 打开系统各部分时钟
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void RCC_Configuration(void)
{
		/* 定义枚举类型变量 HSEStartUpStatus */
  	ErrorStatus HSEStartUpStatus;
  	/* 复位系统时钟设置 */
  	RCC_DeInit();
  	/* 开启 HSE */
  	RCC_HSEConfig(RCC_HSE_ON);
  	/* 等待 HSE 起振并稳定 */
  	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	  /* 判断 HSE 起是否振成功，是则进入if()内部 */
  	if(HSEStartUpStatus == SUCCESS)
  	{
    	/* 选择 HCLK（AHB）时钟源为SYSCLK 1分频 */
    	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    	
    	/* 选择 PCLK1 时钟源为 HCLK（AHB）2分频 */
    	RCC_PCLK1Config(RCC_HCLK_Div2);
			/* 选择 PCLK2 时钟源为 HCLK（AHB）1分频 */
    	RCC_PCLK2Config(RCC_HCLK_Div1); 
			
    	/* 设置 FLASH 延时周期数为2 */
    	FLASH_SetLatency(FLASH_Latency_2);
    	/* 使能 FLASH 预取缓存 */
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    	/* 选择锁相环（PLL）时钟源为 HSE 1分频，倍频数为9，则PLL输出频率为 8MHz * 9 = 72MHz */
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    	/* 使能 PLL */ 
    	RCC_PLLCmd(ENABLE);
    	/* 等待 PLL 输出稳定 */
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    	/* 选择 SYSCLK 时钟源为 PLL */
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    	/* 等待 PLL 成为 SYSCLK 时钟源 */
    	while(RCC_GetSYSCLKSource() != 0x08);
  	} 
		/* 打开APB1总线上的各类时钟*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |
													 RCC_APB1Periph_UART4  | RCC_APB1Periph_UART5  |
		                       RCC_APB1Periph_TIM2   |RCC_APB1Periph_TIM3    |
		                       RCC_APB1Periph_TIM4   | RCC_APB1Periph_TIM5,   
		                       ENABLE);
  	/* 打开APB2总线上的各类时钟*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_GPIOB  |
		                       RCC_APB2Periph_GPIOC  | RCC_APB2Periph_GPIOD  | 
		                       RCC_APB2Periph_GPIOE  | RCC_APB2Periph_USART1 |
													 RCC_APB2Periph_AFIO   ,ENABLE);
}

/*******************************************************************************
* 函数名  		: GPIO_Configuration
* 函数描述    	: 设置各GPIO端口功能
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;	
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		  //JTAG,SWD

		/********************PA口配置******************/
		/* 设置PA相应口为推挽输出，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |
																	GPIO_Pin_7 | GPIO_Pin_15 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
  	/* 设置PA相应口为上拉输入，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | 
																	 GPIO_Pin_8 | GPIO_Pin_9 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
		/********************PB口配置******************/
  	/* 设置PB相应口为开漏输出，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);
		
		/* 设置PB相应口为推挽输出，最大翻转频率为50MHz*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |
																	GPIO_Pin_8 | GPIO_Pin_9 |
																	GPIO_Pin_12 | GPIO_Pin_13 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);
		
		/* 设置PB相应口为上拉输入，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;																
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);	

		/********************PC口配置******************/
		/* 设置PC相应口为开漏输出，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 ;																	
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);
		/* 设置PC相应口为上拉输入，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_6 | 
																		GPIO_Pin_7 | GPIO_Pin_8 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);
		/* 设置PC相应口为推挽输出，最大翻转频率为50MHz*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOC , &GPIO_InitStructure);	
	
		/********************PD口配置******************/
		/* 设置PD相应口为推挽输出，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0   |  GPIO_Pin_1 | GPIO_Pin_3  |
																	GPIO_Pin_4 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOD , &GPIO_InitStructure);
		
		/* 设置PD相应口为开漏输出，最大翻转频率为50MHz*/		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  | GPIO_Pin_9  |
																	GPIO_Pin_10  ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOD , &GPIO_InitStructure);
		
		
		/* 设置PD相应口为上拉输入，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |
											 GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
											 GPIO_Pin_15 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOD , &GPIO_InitStructure);		
		
		/********************PE口配置******************/
		/* 设置PE相应口为开漏输出，最大翻转频率为50MHz*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8  | 	GPIO_Pin_9  |GPIO_Pin_10  |
																	GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_11|		GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOE , &GPIO_InitStructure);
		/*GPIOE Configuration: TIM1 channel 1 and 2 */   
//		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOE, &GPIO_InitStructure);
		
		/********************NAV IO上拉输入******************/
		//NAV0 PE5 .		//NAV1 PE6.		//NAV2 PE4 .		//NAV3 PC13. 	//NAV4 PE3 .
		//NAV5 PC14.		//NAV6 PE2 .		//NAV7 PC15.		//NAV8 PE1.		//NAV9 PD5.
		//NAV10 PE0.		//NAV11 PD6.		//NAV12 PB5.		//NAV13 PD7.		//NAV14 PB4.		//NAV15 PB3.
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |	GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5 |GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &GPIO_InitStructure);		
		
		/********************串口接口配置******************/
		/* 设置PA9,PA10(串口1)Tx为第二功能推挽输出模式 Rx脚为浮空输入脚*/
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
	
		/* 设置PA2,PA3(串口2)Tx为第二功能推挽输出模式 Rx脚为浮空输入脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
		
		/* 设置PB10,PB11(串口3)Tx为第二功能推挽输出模式 Rx脚为浮空输入脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB , &GPIO_InitStructure);   
		
		/* 设置PC10,PC11(串口4)Tx为第二功能推挽输出模式 Rx脚为浮空输入脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC , &GPIO_InitStructure);  
		
		/* 设置PC12,PD2(串口5)Tx为第二功能推挽输出模式 Rx脚为浮空输入脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD , &GPIO_InitStructure); 		
}
void IWDG_Configuration(void)
{
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能预分频寄存器和重装载寄存器
		IWDG_SetPrescaler(IWDG_Prescaler_256);//预分频值 40K/256=156，一秒震荡156次
		IWDG_SetReload(500);//重装载值，计数500次。
		IWDG_ReloadCounter();	//喂狗
		IWDG_Enable();//使能
		IWDG_GetFlagStatus(IWDG_FLAG_PVU);//得到标志位的状态，是否复位
}
/******************************************************************************
* 函数名  		: Systick_Configuration
* 函数描述    	: 设置Systick定时器,重装载时间为1s
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************/
void Systick_Configuration(void)
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=72/8;		    
	fac_ms=(u16)fac_us*1000;		
}
//延时nus
//us为要延时的us数.		    								   
void Delay_us(u16 us)
{		
	u32 temp;	    	 
	SysTick->LOAD=us*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do	
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
/******************************************************************************
* 函数名  		: Delay_ms
* 函数描述    	: n ms定时
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************/
void Delay_ms(u16 ms)
{
	u32 temp;		   
	SysTick->LOAD=(u32)ms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  
}


void Delay_s(u8 s)
{
	u8 tt;
	for(tt=0;tt<s;tt++)
		Delay_ms(1000);
}

void Init_Device(void)
{
	RCC_Configuration();
	GPIO_Configuration();
//	IWDG_Configuration();
	Systick_Configuration();
	EXTI_Config();
	Frequt_Init(65535,17);//18分频//频率输出口可以改为PWM输出，调用PWM_Init(9000,0);	//不分频。PWM频率=72000/900=8Khz
//	nDAC_OUT(0x0F,0);//4通道DAC(0-5V)输出初始化为0
//	TIM2_Config();
//	TIM3_Config();
	TIM5_Config();
//	TIM6_Config(10000);
//	TIM7_Config();
//	USART1_Configuration();
	//USART2_Configuration();
	USART3_Configuration();
	USART5_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	DMA1_Config_Receive2 ();//配置串口2的DMA接受	
	DMA1_Config_Send_2 ();  //配置串口2的发送
	DMA1_Config_Receive3 ();//配置串口3的DMA接受	
	DMA1_Config_Send_3 ();  //配置串口3的发送
}
u16 ConvertSpeed(float argspeed)
{
	return (u16)(177/argspeed);
}




