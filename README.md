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



## IDE
System Workbench for STM32(SW4STM32) + STM32CubeF7

## Project file location
"STM32Cube_FW_F7_V1.x.x\Projects\STM32746G-Discovery\Applications\FreeRTOS\STM32F746DISCO_Serial_Monitor_Plotter"

## Configuration (in "Inc/SerialMonitorPlotter.h")
* Line feed code : define LINE_FEED_LF or LINE_FEED_CRLF or LINE_FEED_CR (default : LINE_FEED_LF)
* Font size of serial monitor : MONITOR_FONT_SIZE (default : FONT_6X10)
* X-axis increment of serial plotter : PLOTTER_X_INCREMENT (default : 1)
* Line color of serial plotter : PLOTTER_COLOR_1 ~ PLOTTER_COLOR_8

