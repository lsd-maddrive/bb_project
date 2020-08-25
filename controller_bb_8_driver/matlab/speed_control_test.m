close all
clear, clc

% Init buffer
x_axis = [0];
speed_A_axis = [0];
speed_B_axis = [0];
speed_C_axis = [0];
speed_setting_axis = [0];
update_period = 10;         % How often update plot, points
time_window = 10;           % Time window to be shown on figure, sec
counter = 0;
freq = 1;                   % Frequency of sine input, Hz (if flag is set)
flag_var_speed = 0;         % 1 to sine wave as input

% Figure settings
figure(1);
set(gcf,'CurrentCharacter','@');                            % Set to a dummy character
set(gcf, 'MenuBar', 'none', 'WindowState', 'maximized');    % Set figure to fullscreen and make none interactible
plotHandle = plot(NaN, NaN, '-r',...
                  NaN, NaN, '-b',...
                  NaN, NaN, '-g',...
                  NaN, NaN, '-m');                          % Make initial empty plot
legend('Set', 'Motor A', 'Motor B', 'Motor C');
xlim([0 time_window]);
ylim([-4 4]);
ylabel('Speed, rps');
xlabel('"Time"');
grid on 

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

while true
    counter = counter + 1;
    new_values = fread(port, [4, 1], 'int16');
    
    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end
    
    % Add new points to the plot and forget last one if necessary
    if x_axis(end) >= time_window
        x_axis = [x_axis(2:end) etime(clock, start_time)];
        speed_setting_axis = [speed_setting_axis(2:end) (new_values(1) / 100)];
        speed_A_axis = [speed_A_axis(2:end) (new_values(1) / 100)];
        speed_B_axis = [speed_B_axis(2:end) (new_values(1) / 100)];
        speed_C_axis = [speed_C_axis(2:end) (new_values(1) / 100)];
    else
        x_axis = [x_axis etime(clock, start_time)];
        speed_setting_axis = [speed_setting_axis (new_values(1) / 100)];
        speed_A_axis = [speed_A_axis (new_values(2) / 100)];
        speed_B_axis = [speed_B_axis (new_values(3) / 100)];
        speed_C_axis = [speed_C_axis (new_values(4) / 100)];
    end
    
    % Update plot
    if counter == update_period
        set(plotHandle(1), 'XData', x_axis, 'YData', speed_setting_axis);
        set(plotHandle(2), 'XData', x_axis, 'YData', speed_A_axis);
        set(plotHandle(2), 'XData', x_axis, 'YData', speed_B_axis);
        set(plotHandle(2), 'XData', x_axis, 'YData', speed_C_axis);
        if x_axis(end) >= time_window
            xlim([x_axis(1) x_axis(end)]);
        end
        drawnow;
        counter = 0;
    end
    
    % Stop loop from keyboard
    k = get(gcf,'CurrentCharacter');
    if k ~= '@'
        disp('Stopped by user');
        break;
    end
    
    % Non-constant speed input
    if flag_var_speed == 1
        desired_speed = 1.5 + sin(freq * pi * etime(clock, start_time));
        fwrite(port, desired_speed * 100, 'int16');
    end
end

% Stop motor and close port
fwrite(port, 500, 'int16');
fclose(port);
disp('Connection is closed!');