# Использование модулей периферии контроллера

## Tickless mode => GPT5 

## Драйвер управления приводами движения 

### Motor 1 
Driver | Pins | Input/Output | Name
:-----:|:----:|:------------:|:--------:
PWM4   | PD_15| output (channel 4)| HIN1  
PWM4   | PD_14| output (channel 3)| LIN1
PWM1   | PE_11| output (channel 2)| HIN2
PWM1   | PE_13| output (channel 3)| LIN2

### Motor 2 
Driver | Pins | Input/Output | Name
:-----:|:----:|:------------:|:--------:
PWM2   | PB_10| output (channel 3)| HIN1  
PWM2   | PA_0 | output (channel 1)| LIN1
PWM2   | PB_11| output (channel 4)| HIN2
PWM1   | PE_14| output (channel 4)| LIN2

### Motor 3 
Driver | Pins | Input/Output | Name
:-----:|:----:|:------------:|:--------:
PWM3   | PC_7 | output (channel 2)| HIN1  
PWM4   | PB_6 | output (channel 1)| LIN1
PWM3   | PC_6 | output (channel 1)| HIN2
PWM2   | PB_3 | output (channel 2)| LIN2

## Драйвер обработки энкодера 
Driver | Pins | Input/Output | Base EXT channel
:-----:|:----:|:------------:|:--------:
EXT1   | PD_5 | input | YES
EXT1   | PD_4 | input | NO
