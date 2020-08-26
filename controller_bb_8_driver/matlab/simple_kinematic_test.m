close all
clear, clc

% Init buffer
x_axis = [0];
speed_A_axis = [0];
speed_B_axis = [0];
speed_C_axis = [0];
time_length = 2;           % Time window to be shown on figure, sec


V_x = 0.5;  % Linear velocity, m/s
V_y = 0.1; 
w = 0;      % angular velocity, rad/s
[w_A, w_B, w_C] = inverse_kinematic(V_x, V_y, w);   % Get wheels speed

% Usart settings
port = serial('COM11', 'BaudRate', 115200);
port.InputBufferSize = 4096;
fopen(port);
set(port, 'ByteOrder', 'littleEndian');
disp('Connection is ready!');

fwrite(port, cast([w_A w_B w_C] * 100, 'int16'), 'int16');   % Send command to mcu

new_values = [];
start_time = clock;

while x_axis < time_length
    new_values = fread(port, [3, 1], 'int16');
    
    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end
    
    % Add new points to the plot
    x_axis = [x_axis etime(clock, start_time)];
    speed_A_axis = [speed_A_axis (new_values(1) / 100)];
    speed_B_axis = [speed_B_axis (new_values(2) / 100)];
    speed_C_axis = [speed_C_axis (new_values(3) / 100)];
    
end

% Stop motor and close port
fwrite(port, 500, 'int16');
fclose(port);
disp('Connection is closed!');

% Figure settings
figure(1);
plot(x_axis(2:end), speed_A_axis(2:end), '-b',...
     x_axis(2:end), speed_B_axis(2:end), '-g',...
     x_axis(2:end), speed_C_axis(2:end), '-m');
legend('Motor A', 'Motor B', 'Motor C');
ylabel('Speed, rps');
xlabel('"Time"');
grid on