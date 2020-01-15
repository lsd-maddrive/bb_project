# Использование модулей периферии контроллера

## Tickless mode => GPT5 

## Драйвер управления приводами движения 
Driver | Pins | Input/Output | № Motor
:-----:|:----:|:------------:|:--------:
PWM1   | PE_9 | output (channel 0)| 1 (forward)
PWM1   | PE_11| output (channel 1)| 1 (backward)
PWM1   | PE_13| output (channel 2)| 2 (forward)
PWM1   | PE_14| output (channel 3)| 2 (backward)

## Драйвер обработки энкодера 
Driver | Pins | Input/Output | Base EXT channel
:-----:|:----:|:------------:|:--------:
EXT1   | PD_5 | input | YES
EXT1   | PD_4 | input | NO
