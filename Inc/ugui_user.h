/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UGUI_USER_H
#define __UGUI_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <stdint.h>

/* Include user header files -------------------------------------------------*/
#include "ugui.h"

/* Exported macro ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Exported function macro ---------------------------------------------------*/
/* Private function macro ----------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private enum tag ----------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Private struct/union tag --------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
void UG_ButtonGetPosition(UG_WINDOW*, UG_U8, UG_S16 *, UG_S16 *, UG_S16 *, UG_S16 *);
UG_RESULT UG_ButtonSetPosition( UG_WINDOW*, UG_U8, UG_S16, UG_S16, UG_S16, UG_S16);

/* Private function prototypes -----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __UGUI_USER_H */

/***************************************************************END OF FILE****/


