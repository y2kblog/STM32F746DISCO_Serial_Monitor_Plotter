/* Include system header files -----------------------------------------------*/
/* Include user header files -------------------------------------------------*/
#include "ugui_user.h"

/* Imported variables --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void UG_ButtonGetPosition( UG_WINDOW* wnd, UG_U8 id, UG_S16 *xs, UG_S16 *ys, UG_S16 *xe, UG_S16 *ye)
{
   UG_OBJECT* obj=NULL;
   
   obj = _UG_SearchObject( wnd, (UG_U8)OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      *xs = obj->a_rel.xs;
      *ys = obj->a_rel.ys;
      *xe = obj->a_rel.xe;
      *ye = obj->a_rel.ye;
   }
}

UG_RESULT UG_ButtonSetPosition( UG_WINDOW* wnd, UG_U8 id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye)
{
   UG_OBJECT* obj=NULL;
   
   obj = _UG_SearchObject( wnd, (UG_U8)OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   obj->a_rel.xs = xs;
   obj->a_rel.ys = ys;
   obj->a_rel.xe = xe;
   obj->a_rel.ye = ye;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

/* Private functions ---------------------------------------------------------*/

/***************************************************************END OF FILE****/

