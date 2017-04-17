/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOW_TEMPLETE_H
#define __WINDOW_TEMPLETE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <stdio.h>

/* Include user header files -------------------------------------------------*/
#include "UserCommon.h"
#include "ugui.h"

/* Exported macro ------------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- User setting Configuration                                 -- */
/* ---------------------------------------------------------------- */

/* Private macro -------------------------------------------------------------*/
#define TEMPELETE_TITLE             "Tempelte title name"
#define uGUI_TEMPELETE_OBJECTS_NUM  10
#define TEMPELTE_UPDATE_MS          50

/* Exported function macro ---------------------------------------------------*/
/* Private function macro ----------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// FreeRTOS
TaskHandle_t xHandle_Tempelete;

// uGUI
UG_WINDOW	 wnd_Tempelete;

/* Exported function prototypes ----------------------------------------------*/
void TempleteThread(void const *argument);
void createTempleteWindow(void);
void activateTempleteThread(void);

#ifdef __cplusplus
}
#endif

#endif /* __WINDOW_TEMPLETE_H */
/***************************************************************END OF FILE****/
