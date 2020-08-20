# Использование модулей периферии контроллера

## Tickless mode => GPT5 

## Отладка UART

Driver | Pins | Input/Output 
:-----:|:----:|:------------:
SD6    | PG14 | TX
SD6    | PG9  | RX 

## Драйвер управления приводами движения 

### Motor A 
Driver | Pins | Input/Output | Name
:-----:|:----:|:------------:|:--------:
PWM1   | PE_11| output (channel 2) | HIN1  
PWM1   | PE_10| output (channel 2N)| LIN1
PWM1   | PE_13| output (channel 3) | HIN2
PWM1   | PE_12| output (channel 3N)| LIN2

### Motor B 
Driver | Pins | Input/Output | Name
:-----:|:----:|:------------:|:--------:
PWM8   | PC_6 | output (channel 1) | HIN1  
PWM8   | PA_5 | output (channel 1N)| LIN1
PWM8   | PC_7 | output (channel 2) | HIN2
PWM8   | PB_0 | output (channel 2N)| LIN2

### Motor C 
Driver | Pins | Input/Output | Name
:-----:|:----:|:------------:|:--------:
PWM8   | PC_8 | output (channel 3) | HIN1  
PWM8   | PB_1 | output (channel 3N)| LIN1
PWM1   | PE_9 | output (channel 1) | HIN2
PWM1   | PE_8 | output (channel 1N)| LIN2

## Драйвер обработки энкодера 

### Encoder Motor A 
Driver | Pins | Input/Output | Base EXT channel
:-----:|:----:|:------------:|:--------:
EXT1   | PD_5 | input | YES
EXT1   | PD_4 | input | NO

### Encoder Motor B
Driver | Pins | Input/Output | Base EXT channel
:-----:|:----:|:------------:|:--------:
EXT1   | PD_7 | input | YES
EXT1   | PD_6 | input | NO

### Encoder Motor C
Driver | Pins | Input/Output | Base EXT channel
:-----:|:----:|:------------:|:--------:
EXT1   | PD_3 | input | YES
EXT1   | PE_2 | input | NO