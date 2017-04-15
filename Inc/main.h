/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_DelayUntil/Inc/main.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "stm32f7xx_hal.h"

/* EVAL includes component */
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

/* FreeRTOS */
#include "cmsis_os.h"

/* uGUI */
#include "ugui.h"

/* User */
#include "ugui_user.h"
#include "SerialMonitorPlotter.h"

/* ---------------------------------------------------------------- */
/* -- Configuration                                              -- */
/* ---------------------------------------------------------------- */
#define uGUI_UPDATE_MS      50      /* uGUI update time in milliseconds */


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- UART                                                       -- */
/* ---------------------------------------------------------------- */
/* Definition for USARTx clock resources */
#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_6
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_PIN                    GPIO_PIN_7
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF8_USART6

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART6_IRQn
#define USARTx_IRQHandler                USART6_IRQHandler

#define DEFAULT_UART_BAUD_RATE  9600
/*
 * Serial Monitor & Ploter buffer size
 * Max = 115200[bps] = 14400[char/s] = 720[char/50ms] -> must be set over this value
 */
#define UART_QUEUE_BUFFER_SIZE 1024

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#define uGUI_MAX_OBJECTS    10

// FreeRTOS Priority
enum FreeRTOSPriority
{
    Priority_Idle = tskIDLE_PRIORITY,
    Priority_Low,
    Priority_BelowNormal,
    Priority_Normal,
    Priority_AboveNormal,
    Priority_High,
    Priority_RealTime
};

/* Exported types ------------------------------------------------------------*/
/* Private enum tag ----------------------------------------------------------*/
enum MainMenu_BTN_ID
{
    MainMenu_BTN_ID_SwitchSMP = BTN_ID_0
};

/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Private struct/union tag --------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* UART */
UART_HandleTypeDef UartHandle;
QueueHandle_t UARTqueue;
uint8_t UART_RxBuffer;

/* Touch screen */
TS_StateTypeDef TS_State;

/* uGUI structure */
UG_GUI gui;

TickType_t LastTouchedTickCount;

// FreeRTOS
TaskHandle_t xHandle_StartThread;
TaskHandle_t xHandle_uGUIUpdateThread;
TaskHandle_t xHandle_GraphDrawThread;


/* uGUI : Main Menu Window */
UG_WINDOW	wnd_MainMenu;
UG_OBJECT 	obj_buff_wnd_MainMenu[uGUI_MAX_OBJECTS];
UG_BUTTON 	btn_MainMenu_switchSMPWindow;

/* uGUI : Console */
UG_S16 console_x_start_default, console_x_end_default, console_y_start_default, console_y_end_default;

/* Exported functions ------------------------------------------------------- */
void UART_ReConfig(uint32_t);

/* uGUI porting function */
void pset(UG_S16, UG_S16, UG_COLOR);

/* uGUI : Main Menu Window */
void createMainMenuWindow(void);
void MainMenuWindow_callback(UG_MESSAGE*);

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

