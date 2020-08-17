clear all
global dat
delete(instrfind);
dat = serial('COM11', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat); 
set(dat, 'ByteOrder', 'littleEndian'); 

disp 'Connection is ready!'

start = 's';
fwrite(dat, start, 'uint8'); 

pool = []; 
total = []; 

for i = 1:10
    pool = fread(dat, [1000, 1], 'int16');
    total = [total; pool];
    % increase dutycycle 
    fwrite(dat, 'f', 'uint8');
    disp 'Increase speed!'
end

fclose(dat);
disp 'Connection is closed!'

name = 'freq100.mat';
path = 'D:\RTOS_info\Distrib_STM32\Distrib\ChibiStudio_v0\ChibiStudio\workspace176\bb-8_project\controller_bb_8_driver\matlab\files\';
save(fullfile(path, name), 'total', '-mat')