# Serial Monitor and Plotter using STM32F7 Discovery
[](
<http://y2kblog.seesaa.net/article/SerialMonitorPlotter_STM32F7-Discovery.html>
)

## Serial Monitor
![](https://github.com/y2kblog/STM32F746DISCO_Serial_Monitor_Plotter/blob/master/images/SerialMonitor.jpg)

## Serial Plotter
![](https://github.com/y2kblog/STM32F746DISCO_Serial_Monitor_Plotter/blob/master/images/SerialPlotter.jpg)


## Let's just try
Drag & Drop "/STM32746G_DISCOVERY.bin" into the STM32F7 Discovery drive.
Connect pin of STM32F7 discovery is arduino D0(RX) pin.

If you have another mbed microcontroller device, you can use "/CommunicationTestCode_mbed_NUCLEO-F411RE/main.cpp" to test communication.

## IDE
System Workbench for STM32 (SW4STM32) + STM32CubeF7

## Project file location
"STM32Cube_FW_F7_V1.x.x\Projects\STM32746G-Discovery\Applications\FreeRTOS\STM32F746DISCO_Serial_Monitor_Plotter"

## Specification & Configuration
### Communication (UART)
* Data : 8bit
* Parity : None
* Stop bit : 1bit
* Flow Control : None
* Baudrate : You can change in operation
* Line feed code : define LINE_FEED_LF or LINE_FEED_CRLF or LINE_FEED_CR in "Inc/SerialMonitorPlotter.h" (default : LINE_FEED_LF)

### Serial Monitor
* Font size : config MONITOR_FONT_SIZE in "Inc/SerialMonitorPlotter.h" (default : FONT_6X10)

### Serial Plotter
* Max number of display points : 8
* Numeric format : Decimal (ex. "-10.0") or Integer (ex. "100")
* Max value : config PLOTTER_MAX_VALUE_1 ~ PLOTTER_MAX_VALUE_8 in "Inc/SerialMonitorPlotter.h"
* Min value : config PLOTTER_MIN_VALUE_1 ~ PLOTTER_MIN_VALUE_8 in "Inc/SerialMonitorPlotter.h"
* Token : config PLOTTER_TOKEN in "Inc/SerialMonitorPlotter.h" (default : ",")
* X-axis increment : config PLOTTER_X_INCREMENT in "Inc/SerialMonitorPlotter.h" (default : 1)
* Line color : config PLOTTER_COLOR_1 ~ PLOTTER_COLOR_8 in "Inc/SerialMonitorPlotter.h"

