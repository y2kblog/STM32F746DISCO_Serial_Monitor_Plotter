/* Include system header files -----------------------------------------------*/
/* Include user header files -------------------------------------------------*/
#include "Window_Templete.h"

/* Imported variables --------------------------------------------------------*/
extern UG_WINDOW wnd_MainMenu;

/* Private enum tag ----------------------------------------------------------*/
enum Tempelete_BTN_ID
{
    Tempelete_BTN_ID_func1 = BTN_ID_0
};

enum Tempelete_TXB_ID
{
    Tempelete_TXB_ID_func1 = TXB_ID_0
};

/* Private variables ---------------------------------------------------------*/
/* "this" pointer */
static TaskHandle_t* pthis_xHandle = &xHandle_Tempelete;
static UG_WINDOW* pthis_wnd = &wnd_Tempelete;

/* Objects of uGUI */
static UG_OBJECT obj_this_wnd[uGUI_SMP_OBJECTS];
static UG_BUTTON btn_SMP_func1;

/* Private function prototypes -----------------------------------------------*/
static void window_callback( UG_MESSAGE* msg );
static void initialize(void);
static void execute(void);
static void draw(void);
static void finalize(void);
static void inactivateThread(void);

/* Exported functions --------------------------------------------------------*/

/* ---------------------------------------------------------------- */
/* -- Thread                                                     -- */
/* ---------------------------------------------------------------- */
void TempleteThread(void const *argument)
{
    /* Variables initialization ------------------------------------*/
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    UG_WINDOW* last_wnd = gui.active_window;
    
#ifdef PRINTF_DEBUG_MDOE
    printf("%s thread start\r\n", pcTaskGetName(*pthis_xHandle) );
#endif

    /* Infinite loop */
    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, TEMPELTE_UPDATE_MS);

        /* Initialize */
        if( (last_wnd != this_wnd) && (gui.active_window == this_wnd) )
        {
            initialize();
        }

        /* Execute */
        if( (last_wnd == this_wnd) && (gui.active_window == this_wnd) )
        {
            execute();
            draw();
        }

        /* Finalize */
        if( (last_wnd == this_wnd) && (gui.active_window != this_wnd) )
        {
            finalize();
        }

        last_wnd = gui.active_window;
    }
}

/* ---------------------------------------------------------------- */
/* -- Create Window                                              -- */
/* ---------------------------------------------------------------- */
void createTempleteWindow(void)
{
    UG_U8 id_buf;
    
	UG_WindowCreate(pthis_wnd, obj_this_wnd, COUNTOF(obj_this_wnd), window_callback);
	UG_WindowSetTitleText(pthis_wnd, TEMPELETE_TITLE);
	UG_WindowSetTitleTextFont(pthis_wnd, &FONT_10X16);
	UG_WindowResize(pthis_wnd, 0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() );

	// Create buttons
	id_buf = Tempelete_BTN_ID_func1;
    UG_ButtonCreate(pthis_wnd, &btn_SMP_func1, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd) * 0 / 5 + 1,
            UG_WindowGetInnerHeight(pthis_wnd) - 1 - 30,
            UG_WindowGetInnerWidth(pthis_wnd) * 1 / 5 - 1,
            UG_WindowGetInnerHeight(pthis_wnd) - 1);
    UG_ButtonSetBackColor(pthis_wnd, id_buf, C_WHITE_SMOKE);
    UG_ButtonSetFont(pthis_wnd, id_buf, &FONT_8X12);
    UG_ButtonSetText(pthis_wnd, id_buf, "Monitor");
}

/* ---------------------------------------------------------------- */
/* -- Activate Thread                                            -- */
/* ---------------------------------------------------------------- */
void activateTempleteThread(void)
{
    vTaskPrioritySet(*pthis_xHandle, Priority_High);
    UG_WindowShow(pthis_wnd);
}



/* Private functions ---------------------------------------------------------*/

/* ---------------------------------------------------------------- */
/* -- Callback function                                          -- */
/* ---------------------------------------------------------------- */
static void window_callback(UG_MESSAGE* msg)
{
	if (msg->type == MSG_TYPE_OBJECT)
	{
		if (msg->id == OBJ_TYPE_BUTTON)
		{
			switch (msg->sub_id)
			{
			case Tempelete_BTN_ID_func1:
			    break;

			default:
#ifdef PRINTF_DEBUG_MDOE
				printf("error\r\n");
#endif
				break;
			}
		}
	}
}

/* ---------------------------------------------------------------- */
/* -- Initialize                                                 -- */
/* ---------------------------------------------------------------- */
static void initialize(void)
{
	/* Variables Initialization */
}

/* ---------------------------------------------------------------- */
/* -- Execute                                                    -- */
/* ---------------------------------------------------------------- */
static void execute(void)
{
    
}

/* ---------------------------------------------------------------- */
/* -- Draw                                                       -- */
/* ---------------------------------------------------------------- */
static void draw(void)
{
    
}

/* ---------------------------------------------------------------- */
/* -- Finalize                                                   -- */
/* ---------------------------------------------------------------- */
static void finalize(void)
{
    /* Variables Finalization */
    
    
    inactivateThread();
}

/* ---------------------------------------------------------------- */
/* -- Inactivate Thread                                          -- */
/* ---------------------------------------------------------------- */
static void inactivateThread(void)
{
	vTaskPrioritySet(*pthis_xHandle, Priority_Low);
}


/***************************************************************END OF FILE****/
