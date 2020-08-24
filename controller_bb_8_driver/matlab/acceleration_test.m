close all
clear, clc

% Init buffer
x_axis = [0];
speed_filtered_axis = [0];
speed_setting_axis = [0];
time_length = 2;           % Time window to be shown on figure, sec

% Usart settings
port = serial('COM11', 'BaudRate', 115200);
port.InputBufferSize = 4096;
fopen(port);
set(port, 'ByteOrder', 'littleEndian');
disp('Connection is ready!');

desired_speed = 1; % Speed setting ([-3 3], rps)
fwrite(port, cast(desired_speed * 100, 'int16'), 'int16');   % Send command to mcu

new_values = [];
start_time = clock;

while x_axis < time_length
    new_values = fread(port, [2, 1], 'int16');
    
    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end
    
    % Add new points to the plot
    x_axis = [x_axis etime(clock, start_time)];
    speed_filtered_axis = [speed_filtered_axis (new_values(1) / 100)];
    speed_setting_axis = [speed_setting_axis (new_values(2) / 100)];
end

% Stop motor and close port
fwrite(port, 500, 'int16');
fclose(port);
disp('Connection is closed!');

% Figure settings
figure(1);
plot(x_axis, speed_filtered_axis, '-r',...
     x_axis, speed_setting_axis, '-b');
legend('Filtered', 'Set');
ylabel('Speed, rps');
xlabel('"Time"');
grid on