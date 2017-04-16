/* Include system header files -----------------------------------------------*/
/* Include user header files -------------------------------------------------*/
#include "SerialMonitorPlotter.h"

/* Imported variables --------------------------------------------------------*/
extern UG_WINDOW wnd_MainMenu;

/* Private variables ---------------------------------------------------------*/
/* "this" pointer */
static UG_WINDOW* this_wnd = &wnd_SMP;
static TaskHandle_t* this_xHandle = &xHandle_SMP;

/* Mode */
static bool MonitorMode = true;
static uint8_t baudrate_num = 0;
static bool isRecording = false;

/* UART setting */
static uint32_t baudrate[] = {DEFAULT_UART_BAUD_RATE, 38400, 115200};
static char str_baudrate[10];
static char str_UART_Receive[UART_QUEUE_BUFFER_SIZE];

/* Plotter */
const static UG_COLOR Plotter_color[8] = {
    PLOTTER_COLOR_1, PLOTTER_COLOR_2, PLOTTER_COLOR_3, PLOTTER_COLOR_4,
    PLOTTER_COLOR_5, PLOTTER_COLOR_6, PLOTTER_COLOR_7, PLOTTER_COLOR_8};
static char Plotter_str_1line[8*12+3];
static int32_t Plotter_RawVal[8];
static int32_t Plotter_DrawVal[8], Plotter_prevDrawVal[8];
static UG_S16 Plotter_X_position;

/* Exported functions --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void window_callback( UG_MESSAGE* msg );
static void initialize(void);
static void execute(void);
static void finalize(void);
static void InactivateThread(void);

static void initMonitorMode(void);
static void initPlotterMode(void);

/* ---------------------------------------------------------------- */
/* -- Thread                                                     -- */
/* ---------------------------------------------------------------- */
void SMP_Thread(void const *argument)
{
    /* Variables initialization ------------------------------------*/
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    UG_WINDOW* last_wnd = gui.active_window;

    /* Infinite loop */
    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, SMP_UPDATE_MS);

        /* Initialize */
        if( (last_wnd != this_wnd) && (gui.active_window == this_wnd) )
        {
            initialize();
        }

        /* Execute */
        if( (last_wnd == this_wnd) && (gui.active_window == this_wnd) )
        {
            execute();
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
void createSMPWindow(void)
{
	UG_WindowCreate(this_wnd, obj_SMP, uGUI_SMP_OBJECTS, window_callback);
	sprintf(SMP_Title, "%s ver%d", SERIAL_MONITOR_PLOTTER_TITLE, SERIAL_MONITOR_PLOTTER_VERSION);
	UG_WindowSetTitleText(this_wnd, SMP_Title);
	UG_WindowSetTitleTextFont(this_wnd, &FONT_10X16);
	UG_WindowResize(this_wnd, 0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() );

	/* Create buttons */
    UG_ButtonCreate(this_wnd, &btn_SMP_switchMonitorPlotter, SMP_BTN_ID_switchMonitorPlotter,
            UG_WindowGetInnerWidth(this_wnd) * 0 / 5 + 1,
            UG_WindowGetInnerHeight(this_wnd) - 1 - SMP_BUTTON_HEIGHT,
            UG_WindowGetInnerWidth(this_wnd) * 1 / 5 - 1,
            UG_WindowGetInnerHeight(this_wnd) - 1);
    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_switchMonitorPlotter, C_WHITE_SMOKE);
    UG_ButtonSetFont(this_wnd, SMP_BTN_ID_switchMonitorPlotter, &FONT_8X12);
    UG_ButtonSetText(this_wnd, SMP_BTN_ID_switchMonitorPlotter, "Monitor");

    UG_ButtonCreate(this_wnd, &btn_SMP_baudrate, SMP_BTN_ID_baudrate,
            UG_WindowGetInnerWidth(this_wnd) * 1 / 5 + 1,
            UG_WindowGetInnerHeight(this_wnd) - 1 - SMP_BUTTON_HEIGHT,
            UG_WindowGetInnerWidth(this_wnd) * 2 / 5 - 1,
            UG_WindowGetInnerHeight(this_wnd) - 1);
    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_baudrate, C_WHITE_SMOKE);
    UG_ButtonSetFont(this_wnd, SMP_BTN_ID_baudrate, &FONT_8X12);
    sprintf(str_baudrate, "%dbps", (int)baudrate[0]);
    UG_ButtonSetText(this_wnd, SMP_BTN_ID_baudrate, str_baudrate);
    
    isPause = false;
    UG_ButtonCreate(this_wnd, &btn_SMP_pause, SMP_BTN_ID_pause,
            UG_WindowGetInnerWidth(this_wnd) * 2 / 5 + 1,
            UG_WindowGetInnerHeight(this_wnd) - 1 - SMP_BUTTON_HEIGHT,
            UG_WindowGetInnerWidth(this_wnd) * 3 / 5 - 1,
            UG_WindowGetInnerHeight(this_wnd) - 1);
    if (isPause == false)
    {
        UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_pause, C_WHITE_SMOKE);
    }
    else
    {
        UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_pause, C_LIME);
    }
    UG_ButtonSetFont(this_wnd, SMP_BTN_ID_pause, &FONT_8X12);
    UG_ButtonSetText(this_wnd, SMP_BTN_ID_pause, "Pause");

    UG_ButtonCreate(this_wnd, &btn_SMP_record, SMP_BTN_ID_record,
            UG_WindowGetInnerWidth(this_wnd) * 3 / 5 + 1,
            UG_WindowGetInnerHeight(this_wnd) - 1 - SMP_BUTTON_HEIGHT,
            UG_WindowGetInnerWidth(this_wnd) * 4 / 5 - 1,
            UG_WindowGetInnerHeight(this_wnd) - 1);
    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_record, C_WHITE_SMOKE);
    UG_ButtonSetFont(this_wnd, SMP_BTN_ID_record, &FONT_8X12);
    //UG_ButtonSetText(this_wnd, SMP_BTN_ID_record, "Record");

    UG_ButtonCreate(this_wnd, &btn_SMP_close, SMP_BTN_ID_close,
            UG_WindowGetInnerWidth(this_wnd) * 4 / 5 + 1,
            UG_WindowGetInnerHeight(this_wnd) - 1 - SMP_BUTTON_HEIGHT,
            UG_WindowGetInnerWidth(this_wnd) * 5 / 5 - 1,
            UG_WindowGetInnerHeight(this_wnd) - 1);
    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_close, C_WHITE_SMOKE);
    UG_ButtonSetFont(this_wnd, SMP_BTN_ID_close, &FONT_8X12);
    UG_ButtonSetText(this_wnd, SMP_BTN_ID_close, "Close");

    /* Configure console */
    UG_FontSelect(&(MONITOR_FONT_SIZE));
    UG_ConsoleSetBackcolor(C_BLACK);
    UG_ConsoleSetForecolor(C_WHITE);
}

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
			case SMP_BTN_ID_switchMonitorPlotter:
			    if(MonitorMode == true)
			    {
			        /* Monitor -> Plotter */
			        MonitorMode = false;
			        initPlotterMode();
			        UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_switchMonitorPlotter, C_LIME);
			        UG_ButtonSetText(this_wnd, SMP_BTN_ID_switchMonitorPlotter, "Plotter");
			    }
			    else
			    {
			        /* Plotter -> Monitor */
			        MonitorMode = true;
			        initMonitorMode();
			        UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_switchMonitorPlotter, C_WHITE_SMOKE);
			        UG_ButtonSetText(this_wnd, SMP_BTN_ID_switchMonitorPlotter, "Monitor");
			    }
			    break;

			case SMP_BTN_ID_baudrate:
			    baudrate_num++;
			    if( baudrate_num >= (sizeof(baudrate) / sizeof(baudrate[0])) )
			        baudrate_num = 0;
			    sprintf(str_baudrate, "%dbps", (int)baudrate[baudrate_num]);
			    UG_ButtonSetText(this_wnd, SMP_BTN_ID_baudrate, str_baudrate);

			    // Change UART baud rate
			    UART_ReConfig(baudrate[baudrate_num]);
			    break;

			case SMP_BTN_ID_pause:
                if (isPause == true)
                {
                    /* Pause -> Resume */
                    isPause = false;
                    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_pause, C_WHITE_SMOKE);
                    //UG_ButtonSetText(this_wnd, SMP_BTN_ID_pause, "Pause");
                }
                else
                {
                    /* Resume -> Pause */
                    isPause = true;
                    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_pause, C_LIME);
                    //UG_ButtonSetText(this_wnd, SMP_BTN_ID_pause, "Resume");
                }
			    break;

			case SMP_BTN_ID_record:
			    if (isRecording == true)
                {
                    /* End recording */
			        isRecording = false;
                    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_record, C_WHITE_SMOKE);
                }
                else
                {
                    /* Start recording */
                    isRecording = true;
                    UG_ButtonSetBackColor(this_wnd, SMP_BTN_ID_record, C_TOMATO);
                }
			    break;

			case SMP_BTN_ID_close:
			    UG_ButtonSetPosition(&wnd_MainMenu, MainMenu_BTN_ID_SwitchSMP, 5, 5, 300, 50);
				UG_WindowShow(&wnd_MainMenu);
				break;

			default:
				//printf("error\n\r");
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

    if (MonitorMode == true)
        initMonitorMode();
    else
        initPlotterMode();
}

/* ---------------------------------------------------------------- */
/* -- Execute                                                    -- */
/* ---------------------------------------------------------------- */
static void execute(void)
{
    UBaseType_t QueueNum;
    char rcv_char_buf;
    uint32_t str_len;

    //static char buf_send = 'a';
    //xQueueSend(UARTqueue, &buf_send, (TickType_t)0);

    /* Check queue num */
    QueueNum = uxQueueMessagesWaiting(UARTqueue);
    if(QueueNum > 0)
    {
        /* Receive string from queue */
        str_len = 0;
        for(uint32_t i = 0; i < QueueNum; i++)
        {
            //xQueueReceive(UARTqueue, str_UART_Receive + i, (TickType_t)0);
            xQueueReceive(UARTqueue, &rcv_char_buf, (TickType_t)0);

            /* --- Format string (line feed code) --- */
#if   defined(LINE_FEED_LF)
            /* "\n" */
            /* Do nothing */
            str_UART_Receive[str_len] = rcv_char_buf;
            str_len++;
#elif defined(LINE_FEED_CRLF)
            /* "\r\n" */
            if(rcv_char_buf == '\r')
            {
                /* Skip */
            }
            else
            {
                /* Copy */
                str_UART_Receive[str_len] = rcv_char_buf;
                str_len++;
            }
#elif defined(LINE_FEED_CR)
            /* "\r" */
            if(rcv_char_buf == '\r')
            {
                /* Replace '\r' -> '\n' */
                str_UART_Receive[str_len] = '\n';
                str_len++;
            }
            else
            {
                /* Copy */
                str_UART_Receive[str_len] = rcv_char_buf;
                str_len++;
            }
#else
#error define line feed code
#endif
        }

        /* set null-terminated string */
        str_UART_Receive[str_len] = '\0';

        
        if (MonitorMode == true)
        {
            /* Serial Monitor */
            /* --- Draw --- */
            UG_ConsolePutString(str_UART_Receive);
        }
        else
        {
            /* Serial Plotter */
            /* --- Format string for Plotter --- */
            for (uint32_t i = 0; i < str_len; i++)
            {
                /* check if the string is valid for plotter */
                if( ( isdigit(str_UART_Receive[i]) ) || (str_UART_Receive[i] == '-') || (str_UART_Receive[i] == ',') || (str_UART_Receive[i] == '\n') )
                {
                    /* check line feed */
                    if(str_UART_Receive[i] != '\n')
                    {
                        /* Copy and add Plotter_str_1line */
                        strncat(Plotter_str_1line, &str_UART_Receive[i], 1);
                    }
                    else
                    {
                        /* Line feed */
                        
                        /* check Value number */
                        uint8_t ValNum;
                        char *pstr[8];
                        
                        for (ValNum = 0; ValNum < 8; ValNum++)
                        {
                            if(ValNum == 0)
                                /* 1st call */
                                pstr[0] = strtok(Plotter_str_1line, PLOTTER_TOKEN);
                            else
                                pstr[ValNum] = strtok(NULL, PLOTTER_TOKEN);
                            
                            if(pstr[ValNum] == NULL)
                            {
                                break;
                            }
                        }
                        
                        /* Convert string -> Raw value (integer) */
                        for (uint8_t j = 0; j < ValNum; j++)
                        {
                            Plotter_RawVal[j] = atoi(pstr[j]);
                        }
                        
                        /* Raw value range limitation */
                        for (uint8_t j = 0; j < ValNum; j++)
                        {
                            if(Plotter_RawVal[j] >= PLOTTER_MAX_VALUE)
                                Plotter_RawVal[j] = PLOTTER_MAX_VALUE;
                            if(Plotter_RawVal[j] <= PLOTTER_MIN_VALUE)
                                Plotter_RawVal[j] = PLOTTER_MIN_VALUE;
                        }
                        
                        /* Convert Raw value -> Draw value (ex. scaling) */
                        for (uint8_t j = 0; j < ValNum; j++)
                        {
                            Plotter_DrawVal[j] = -Plotter_RawVal[j] * 1;
                        }
                        
                        /* --- Draw --- */
                        UG_S16 y_center = gui.console.y_start + (gui.console.y_end - gui.console.y_start) / 2;
                        for (uint8_t j = 0; j < ValNum; j++)
                        {
                            if(Plotter_X_position == 0)
                                UG_DrawPixel(gui.console.x_start + Plotter_X_position, y_center + Plotter_DrawVal[j], Plotter_color[j]);
                            else if(Plotter_X_position < (gui.console.x_end - gui.console.x_start) )
                            {
                                UG_DrawLine(
                                    gui.console.x_start + Plotter_X_position - PLOTTER_X_INCREMENT, y_center + Plotter_prevDrawVal[j],
                                    gui.console.x_start + Plotter_X_position, y_center + Plotter_DrawVal[j], Plotter_color[j]);
                            }
                            Plotter_prevDrawVal[j] = Plotter_DrawVal[j];
                        }
                        Plotter_X_position += PLOTTER_X_INCREMENT;
                        
                        /* Reach right end of the plot area -> Clear plot area & Reset plot position(Init) */
                        if(Plotter_X_position > (gui.console.x_end - gui.console.x_start) )
                            initPlotterMode();
                        
                        /* Clear string */
                        for (uint32_t j = 0; j < (8*12+3); j++)
                            Plotter_str_1line[j] = '\0';
                    }
                }
            }
        }

        /* Clear string */
        for (uint32_t i = 0; i < str_len; i++)
            str_UART_Receive[i] = '\0';
    }
}

/* ---------------------------------------------------------------- */
/* -- Finalize                                                   -- */
/* ---------------------------------------------------------------- */
static void finalize(void)
{
    xQueueReset(UARTqueue);
    InactivateThread();
}

/* ---------------------------------------------------------------- */
/* -- Activate Thread                                            -- */
/* ---------------------------------------------------------------- */
void ActivateThread(void)
{
	vTaskPrioritySet(this_xHandle, Priority_High);
	UG_ConsoleSetArea(5, 25, UG_WindowGetInnerWidth(this_wnd) - 5, 225);
	UG_WindowShow(this_wnd);
}

/* ---------------------------------------------------------------- */
/* -- Inactivate Thread                                          -- */
/* ---------------------------------------------------------------- */
static void InactivateThread(void)
{
	vTaskPrioritySet(this_xHandle, Priority_Normal);
}

/* Private functions ---------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- Init Monitor mode                                          -- */
/* ---------------------------------------------------------------- */
static void initMonitorMode(void)
{
    /* Reset cursor */
    gui.console.x_pos = gui.console.x_end;
    gui.console.y_pos = gui.console.y_end;

    UG_FillFrame(gui.console.x_start, gui.console.y_start, gui.console.x_end, gui.console.y_end,
            gui.console.back_color);
}

/* ---------------------------------------------------------------- */
/* -- Init Plotter mode                                          -- */
/* ---------------------------------------------------------------- */
static void initPlotterMode(void)
{
    /* Variables Initialization */
    Plotter_X_position = 0;

    UG_FillFrame(gui.console.x_start, gui.console.y_start, gui.console.x_end, gui.console.y_end, C_WHITE);
}

/***************************************************************END OF FILE****/
