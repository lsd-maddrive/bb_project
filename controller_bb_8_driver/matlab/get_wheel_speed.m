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

iter = 3;
for i = 1:iter
    % increase dutycycle 
    fwrite(dat, 'f', 'uint8');
    disp 'Increase speed!'
    pool = fread(dat, [200, 1], 'int16');
    total = [total; pool];
end

for i = 1:iter
    fwrite(dat, 'd', 'uint8');
    disp 'Decrease speed!'
    pause(1);
end

disp 'Stop machine!'
fwrite(dat, ' ', 'uint8');

fclose(dat);
disp 'Connection is closed!'

raw_speed = []; 
lpf_speed = [];

for i = 1:lenght(total)
    if mod(i, 2) == 0
        raw_speed = [raw_speed; total(i, 1)]; 
    else
        lpf_speed = [lpf_speed; total(i, 1)];
    end 
end

disp 'Ready to save!'

name_raw = 'raw_speed_wheel_A.mat';
name_lpf = 'lpf_speed_wheel_A.mat';
path = 'D:\RTOS_info\Distrib_STM32\Distrib\ChibiStudio_v0\ChibiStudio\workspace176\bb-8_project\controller_bb_8_driver\matlab\files\wheel_speed';
save(fullfile(path, name_raw), 'total', '-mat')
save(fullfile(path, name_lpf), 'total', '-mat')

disp 'Saved successfully!'

plot(raw_speed)
hold on 
grid on 
plot(lpf_speed)
legend