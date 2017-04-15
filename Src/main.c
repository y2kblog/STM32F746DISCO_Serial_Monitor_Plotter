/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_DelayUntil/Src/main.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Imported variables --------------------------------------------------------*/
extern UG_WINDOW wnd_SMP;
extern bool isPause;

/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef UartHandle_COM;

/* Private function prototypes -----------------------------------------------*/
static void UART_COM_Config(void);
static void UART_Config(uint32_t);
static void LCD_Config(void);
static void TS_Config(void);
static void StartThread(void const *argument);
static void uGUIUpdateThread(void const *argument);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
UG_RESULT _HW_DrawLine(UG_S16, UG_S16, UG_S16, UG_S16, UG_COLOR);
UG_RESULT _HW_FillFrame(UG_S16, UG_S16, UG_S16, UG_S16, UG_COLOR);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    /* Enable the CPU Cache */
    CPU_CACHE_Enable();

    /* STM32F7xx HAL library initialization:
     - Configure the Flash ART accelerator on ITCM interface
     - Configure the Systick to generate an interrupt each 1 msec
     - Set NVIC Group Priority to 4
     - Low Level Initialization
     */
    HAL_Init();

    /* Configure the system clock to 216 Mhz */
    SystemClock_Config();

    /* Init variables */

    /* FreeRTOS Queue */
    UARTqueue = xQueueCreate(UART_QUEUE_BUFFER_SIZE, 1);

    /* Initialize LEDs */
    BSP_LED_Init(LED1);

    /* Initialize Peripherals */
    UART_COM_Config();
    UART_Config(DEFAULT_UART_BAUD_RATE);

    LCD_Config();
    TS_Config();

    /* uGUI Init */
    UG_Init(&gui, (void (*)(UG_S16, UG_S16, UG_COLOR)) pset, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    UG_FillScreen(C_WHITE);
    console_x_start_default = gui.console.x_start; console_x_end_default = gui.console.x_end;
    console_y_start_default = gui.console.y_start; console_y_end_default = gui.console.y_end;

    /* uGUI hardware accelerator */
    /*UG_DriverRegister(DRIVER_DRAW_LINE, (void*) _HW_DrawLine);
    UG_DriverRegister(DRIVER_FILL_FRAME, (void*) _HW_FillFrame);
    UG_DriverEnable(DRIVER_DRAW_LINE);
    UG_DriverEnable(DRIVER_FILL_FRAME);*/

    /* uGUI : create window */
    createSMPWindow();
    createMainMenuWindow();
    

    /*##-1- Start task #########################################################*/
    // Priority --- 0:Idle, 1:Low, 2:BelowNormal, 3:Normal(default), 4:AboveNormal, 5:High, 6:Realtime
    xTaskCreate((TaskFunction_t) StartThread, "StartTask",
            configMINIMAL_STACK_SIZE+256, NULL, Priority_Low, &xHandle_StartThread);

    /*##-2- Start scheduler ####################################################*/
    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler */
    for (;;);
}

/* ---------------------------------------------------------------- */
/* -- Peripheral Configuration                                   -- */
/* ---------------------------------------------------------------- */
static void UART_COM_Config(void)
{
    UartHandle_COM.Instance = DISCOVERY_COM1;

    UartHandle_COM.Init.BaudRate        = 115200;
    UartHandle_COM.Init.WordLength      = UART_WORDLENGTH_8B;
    UartHandle_COM.Init.StopBits        = UART_STOPBITS_1;
    UartHandle_COM.Init.Parity          = UART_PARITY_NONE;
    UartHandle_COM.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    UartHandle_COM.Init.Mode            = UART_MODE_TX_RX;
    UartHandle_COM.Init.OverSampling    = UART_OVERSAMPLING_16;

    BSP_COM_Init(COM1, &UartHandle_COM);
}

static void UART_Config(uint32_t baudrate_bps)
{
    UartHandle.Instance = USARTx;

    UartHandle.Init.BaudRate    = baudrate_bps;
    UartHandle.Init.WordLength  = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits    = UART_STOPBITS_1;
    UartHandle.Init.Parity      = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl   = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode        = UART_MODE_TX_RX;
    UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_UART_Receive_IT(&UartHandle, &UART_RxBuffer, 1) != HAL_OK)
    {
        Error_Handler();
    }
}

void UART_ReConfig(uint32_t baudrate_bps)
{
    UartHandle.Init.BaudRate    = baudrate_bps;
    if (HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_UART_Receive_IT(&UartHandle, &UART_RxBuffer, 1) != HAL_OK)
    {
        Error_Handler();
    }
}

static void LCD_Config(void)
{
    /* LCD Initialization */
    BSP_LCD_Init();

    /* LCD Initialization */
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);

    /* Enable the LCD */
    BSP_LCD_DisplayOn();

    /* Select the LCD Foreground Layer  */
    BSP_LCD_SelectLayer(0);

    /* Clear the Foreground Layer */
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    /* Configure the transparency : Increase the transparency */
    BSP_LCD_SetTransparency(0, 0xFF);
}

static void TS_Config(void)
{
    if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) != TS_OK)
        Error_Handler();
    else
    {
        if (BSP_TS_ITConfig() != TS_OK)
            Error_Handler();
    }
}


/* ---------------------------------------------------------------- */
/* -- Start Task                                                 -- */
/* ---------------------------------------------------------------- */
static void StartThread(void const *argument)
{
	// Create Task
	// Priority --- 0:Idle, 1:Low, 2:BelowNormal, 3:Normal(default), 4:AboveNormal, 5:High, 6:Realtime
	xTaskCreate( (TaskFunction_t)uGUIUpdateThread, "uGUIUpdateTask", configMINIMAL_STACK_SIZE + 256, NULL, Priority_RealTime, &xHandle_uGUIUpdateThread);
	xTaskCreate( (TaskFunction_t)SMP_Thread,  "SMPTask",  configMINIMAL_STACK_SIZE + 3000, NULL, Priority_Normal, &xHandle_SMP);
	
	/* Suspend this thread */
	vTaskSuspend(NULL);

	/* Infinite loop */
	while (1)
	{
	    //vTaskDelay(1000);
	}
}

/* ---------------------------------------------------------------- */
/* -- uGUI update Task                                           -- */
/* ---------------------------------------------------------------- */
static void uGUIUpdateThread(void const *argument)
{
	/* Variables initialization ------------------------------------*/
	TickType_t xLastWakeTime;
	TickType_t NowTickCount;

	xLastWakeTime = xTaskGetTickCount();

	/* Infinite loop */
	while (1)
	{
		// periodic sequence
		vTaskDelayUntil(&xLastWakeTime, uGUI_UPDATE_MS);

		NowTickCount = xTaskGetTickCount();

		// Reset touch screen data if specified time has passed
		if( ((NowTickCount - LastTouchedTickCount) > uGUI_UPDATE_MS) || ((NowTickCount - LastTouchedTickCount) < 0) )
			BSP_TS_ResetTouchData(&TS_State);

		// Touch screen data update
		if (TS_State.touchDetected > 0)
			UG_TouchUpdate(TS_State.touchX[0], TS_State.touchY[0], TOUCH_STATE_PRESSED);
		else
			UG_TouchUpdate(-1, -1, TOUCH_STATE_RELEASED);

		// Update display
		UG_Update();
	}
}

/* ---------------------------------------------------------------- */
/* -- Main Menu window                                           -- */
/* ---------------------------------------------------------------- */
void createMainMenuWindow(void)
{
	/* Configure window */
	UG_WindowCreate(&wnd_MainMenu, obj_buff_wnd_MainMenu, uGUI_MAX_OBJECTS, MainMenuWindow_callback);
	UG_WindowSetTitleText(&wnd_MainMenu, "MAIN MENU");
	UG_WindowSetTitleTextFont(&wnd_MainMenu, &FONT_12X20);
	UG_WindowSetTitleInactiveColor(&wnd_MainMenu, C_GRAY);

	/* Configure buttons */
	UG_ButtonCreate(&wnd_MainMenu, &btn_MainMenu_switchSMPWindow, MainMenu_BTN_ID_SwitchSMP,
			5, 5, 300, 50);
	UG_ButtonSetFont(&wnd_MainMenu, MainMenu_BTN_ID_SwitchSMP, &FONT_8X14);
	UG_ButtonSetBackColor(&wnd_MainMenu, MainMenu_BTN_ID_SwitchSMP, C_GRAY);
	UG_ButtonSetText(&wnd_MainMenu, MainMenu_BTN_ID_SwitchSMP, "Serial Monitor & Plotter");

	UG_WindowShow(&wnd_MainMenu);
}

void MainMenuWindow_callback(UG_MESSAGE* msg)
{
	if (msg->type == MSG_TYPE_OBJECT)
	{
		if (msg->id == OBJ_TYPE_BUTTON)
		{
			switch (msg->sub_id)
			{
			case MainMenu_BTN_ID_SwitchSMP:
				/* Switch window */
			    ActivateThread();
			    UG_WindowShow(&wnd_SMP);
				break;

			default:
				Error_Handler();
				break;
			}
		}
	}
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case TS_INT_PIN:	// Touch screen interrupt
		LastTouchedTickCount = xTaskGetTickCount();
		BSP_TS_ITGetStatus();
		BSP_TS_ITClear();
		BSP_TS_GetState(&TS_State);
		break;

	default:
		break;
	}
}

/**
 * @brief  Tx Transfer completed callback
 * @param  UartHandle: UART handle.
 * @note   This example shows a simple way to report end of IT Tx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
}

/**
 * @brief  Rx Transfer completed callback
 * @param  UartHandle: UART handle
 * @note   This example shows a simple way to report end of DMA Rx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandleBuf)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(UartHandleBuf == &UartHandle)
    {
        if ( (gui.active_window == &wnd_SMP) && (isPause == false) )
        {
            /* check if the queue is not full */
            if (xQueueIsQueueFullFromISR(UARTqueue) == pdFALSE)
            {
                /* Push queue */
                xQueueSendFromISR(UARTqueue, &UART_RxBuffer, &xHigherPriorityTaskWoken);

                /* Now the buffer is empty we can switch context if necessary. */
                if (xHigherPriorityTaskWoken != pdFALSE)
                {
                    /* Actual macro used here is port specific. */
                    taskYIELD();
                }
            }
        }

        /* UART Receive */
        if (HAL_UART_Receive_IT(&UartHandle, &UART_RxBuffer, 1) != HAL_OK)
        {
            Error_Handler();
        }
    }
    else
    {
        Error_Handler();
    }
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&UartHandle_COM, (uint8_t *)ptr, (uint16_t)len, 0xFFFFFFFF);
    return len;
}

/* -------------------------------------------------------------------------------- */
/* -- Porting function for uGUI                                                  -- */
/* -------------------------------------------------------------------------------- */
void pset(UG_S16 x, UG_S16 y, UG_COLOR col)
{
	BSP_LCD_DrawPixel(x, y, (0xFF000000 | col) );
}

/* Hardware accelerator */
UG_RESULT _HW_DrawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
	BSP_LCD_SetTextColor( 0xFF000000 | c );
	BSP_LCD_DrawLine(x1, y1, x2, y2);
	return UG_RESULT_OK;
}

UG_RESULT _HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
	BSP_LCD_SetTextColor( 0xFF000000 | c );
	BSP_LCD_FillRect((x1<x2)?x1:x2, (y1<y2)?y1:y2, (x1>=x2)?(x1-x2):(x2-x1), (y1>=y2)?(y1-y2):(y2-y1));
	return UG_RESULT_OK;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
