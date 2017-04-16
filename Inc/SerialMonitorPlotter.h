/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERIALMONITORPLOTTER_H
#define __SERIALMONITORPLOTTER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/* Include user header files -------------------------------------------------*/
#include "ugui.h"
#include "main.h"

/* Exported macro ------------------------------------------------------------*/

/* ---------------------------------------------------------------- */
/* -- User setting Configuration                                 -- */
/* ---------------------------------------------------------------- */
#define SERIAL_MONITOR_PLOTTER_VERSION     "3.0"

/* General setting */
/* Receive line feed code (define only one) */
#define LINE_FEED_LF        /* "\n" : Unix */
//#define LINE_FEED_CRLF    /* "\r\n" : Windows */
//#define LINE_FEED_CR      /* "\r" : Mac(before ver9) */

#define SMP_UPDATE_MS       50      /* Serial Monitor & Plotter thread update time in milliseconds */

/* Monitor */
#define MONITOR_FONT_SIZE   FONT_6X10

/* Plotter : General */
#define PLOTTER_TOKEN           ","
/* Plotter : Value range */
#define PLOTTER_DRAW_MAX_VALUE   100
#define PLOTTER_DRAW_MIN_VALUE   -100
#define PLOTTER_MAX_DIGITS       14      /* include a sign and a decimal point */
#define PLOTTER_MAX_VALUE_1      100.0
#define PLOTTER_MIN_VALUE_1     -100.0
#define PLOTTER_MAX_VALUE_2      100.0
#define PLOTTER_MIN_VALUE_2     -100.0
#define PLOTTER_MAX_VALUE_3      100.0
#define PLOTTER_MIN_VALUE_3     -100.0
#define PLOTTER_MAX_VALUE_4      100.0
#define PLOTTER_MIN_VALUE_4     -100.0
#define PLOTTER_MAX_VALUE_5      100.0
#define PLOTTER_MIN_VALUE_5     -100.0
#define PLOTTER_MAX_VALUE_6      100.0
#define PLOTTER_MIN_VALUE_6     -100.0
#define PLOTTER_MAX_VALUE_7      100.0
#define PLOTTER_MIN_VALUE_7     -100.0
#define PLOTTER_MAX_VALUE_8      100.0
#define PLOTTER_MIN_VALUE_8     -100.0
/* Plotter : Draw style */
#define PLOTTER_X_INCREMENT 1
#define PLOTTER_COLOR_1     C_BLACK
#define PLOTTER_COLOR_2     C_RED
#define PLOTTER_COLOR_3     C_BLUE
#define PLOTTER_COLOR_4     C_LIME_GREEN
#define PLOTTER_COLOR_5     C_GRAY
#define PLOTTER_COLOR_6     C_ORANGE
#define PLOTTER_COLOR_7     C_DEEP_SKY_BLUE
#define PLOTTER_COLOR_8     C_GOLD

/* Private macro -------------------------------------------------------------*/
#define SERIAL_MONITOR_PLOTTER_TITLE    "Serial Monitor & Plotter"
#define uGUI_SMP_OBJECTS    10
#define SMP_BUTTON_HEIGHT   38

/* Exported function macro ---------------------------------------------------*/
/* Private function macro ----------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private enum tag ----------------------------------------------------------*/
enum SMP_BTN_ID
{
    SMP_BTN_ID_switchMonitorPlotter = BTN_ID_0,
    SMP_BTN_ID_baudrate,
    SMP_BTN_ID_pause,
    SMP_BTN_ID_clear,
    SMP_BTN_ID_close
};

/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Private struct/union tag --------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
TaskHandle_t    xHandle_SMP;

UG_WINDOW	 wnd_SMP;
UG_OBJECT	 obj_SMP[uGUI_SMP_OBJECTS];
UG_BUTTON    btn_SMP_switchMonitorPlotter;
UG_BUTTON    btn_SMP_baudrate;
UG_BUTTON    btn_SMP_pause;
UG_BUTTON    btn_SMP_record;
UG_BUTTON    btn_SMP_close;
char SMP_Title[32];

bool isPause;

/* Exported function prototypes ----------------------------------------------*/
void SMP_Thread(void const *argument);
void createSMPWindow(void);
void ActivateThread(void);

/* Private function prototypes -----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SERIALMONITORPLOTTER_H */

/***************************************************************END OF FILE****/


