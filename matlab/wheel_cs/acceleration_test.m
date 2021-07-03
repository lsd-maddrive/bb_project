close all
clear, clc

% TODO: clear all nasty if-s 
% TODO: create common interface for comfortable usage

% Init buffer
x_axis = [0];
speed_A_axis = [0];
speed_B_axis = [0];
speed_C_axis = [0];
speed_setting_axis = [0];
time_length = 2;           % Time window to be shown on figure, sec

% Usart settings
port = serial('COM4', 'BaudRate', 115200);
port.InputBufferSize = 4096;
fopen(port);
set(port, 'ByteOrder', 'littleEndian');
disp('Connection is ready!');

% TODO: change to float 
desired_speed = 1; % Speed setting ([-3 3], rps)
fwrite(port, cast(desired_speed * 100, 'int16'), 'int16');   % Send command to mcu

new_values = [];
start_time = clock;

while x_axis < time_length
    new_values = fread(port, [4, 1], 'int16');
    
    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end
    
    % Add new points to the plot
    x_axis = [x_axis etime(clock, start_time)];
    speed_setting_axis = [speed_setting_axis (new_values(1) / 100)];
    speed_A_axis = [speed_A_axis (new_values(2) / 100)];
    speed_B_axis = [speed_B_axis (new_values(3) / 100)];
    speed_C_axis = [speed_C_axis (new_values(4) / 100)];
    
end

desired_speed = 0; % Speed setting ([-3 3], rps)
fwrite(port, cast(desired_speed * 100, 'int16'), 'int16');   % Send command to mcu

while x_axis < time_length + 0.5
    new_values = fread(port, [4, 1], 'int16');
    
    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end
    
    % Add new points to the plot
    x_axis = [x_axis etime(clock, start_time)];
    speed_setting_axis = [speed_setting_axis (new_values(1) / 100)];
    speed_A_axis = [speed_A_axis (new_values(2) / 100)];
    speed_B_axis = [speed_B_axis (new_values(3) / 100)];
    speed_C_axis = [speed_C_axis (new_values(4) / 100)];
    
end

desired_speed = -1; % Speed setting ([-3 3], rps)
fwrite(port, cast(desired_speed * 100, 'int16'), 'int16');   % Send command to mcu

while x_axis < (time_length * 2 + 0.5)
    new_values = fread(port, [4, 1], 'int16');
    
    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end
    
    % Add new points to the plot
    x_axis = [x_axis etime(clock, start_time)];
    speed_setting_axis = [speed_setting_axis (new_values(1) / 100)];
    speed_A_axis = [speed_A_axis (new_values(2) / 100)];
    speed_B_axis = [speed_B_axis (new_values(3) / 100)];
    speed_C_axis = [speed_C_axis (new_values(4) / 100)];
    
end



% Stop motor and close port
fwrite(port, 500, 'int16');
fclose(port);
disp('Connection is closed!');

ref = 'ref3.mat';
speed_a = 'speed_A3.mat';
speed_b = 'speed_B3.mat';
speed_c = 'speed_C3.mat';
path = 'D:\RTOS_info\Distrib_STM32\Distrib\ChibiStudio_v0\ChibiStudio\workspace176\bb-8_project\controller_bb_8_driver\matlab\files\wheel_cs';
save(fullfile(path, ref), 'speed_setting_axis', '-mat')
save(fullfile(path, speed_a), 'speed_A_axis', '-mat')
save(fullfile(path, speed_b), 'speed_B_axis', '-mat')
save(fullfile(path, speed_c), 'speed_C_axis', '-mat')

disp 'Saved successfully!'

% Figure settings
figure(1);
plot(x_axis(2:end), speed_setting_axis(2:end), '-r',...
     x_axis(2:end), speed_A_axis(2:end), '-b',...
     x_axis(2:end), speed_B_axis(2:end), '-g',...
     x_axis(2:end), speed_C_axis(2:end), '-m');
legend('Set', 'Motor A', 'Motor B', 'Motor C');
ylabel('Speed, rps');
xlabel('"Time"');
grid on