close all
clear, clc

% Init buffer
x_axis = [0];
speed_filtered_axis = [0];
speed_raw_axis = [0];
update_period = 2;          % How often update plot, points
time_window = 10;           % Time window to be shown on figure, sec
counter = 0;

% Figure settings
figure(1);
set(gcf,'CurrentCharacter','@');                            % Set to a dummy character
set(gcf, 'MenuBar', 'none', 'WindowState', 'maximized');    % Set figure to fullscreen and make none interactible
plotHandle = plot(NaN, NaN, '-r',...
                  NaN, NaN, '-b');                          % Make initial empty plot
legend('Filtered', 'Raw');
xlim([1 time_window]);
ylim([0 1])
ylabel('Speed, rps')
xlabel('"Time"')

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
    new_values = fread(port, [2, 1], 'int16');
    
    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end
    
    % Add new points to the plot and forget last one if necessary
    if x_axis(end) >= time_window
        x_axis = [x_axis(2:end) etime(clock, start_time)];
        speed_filtered_axis = [speed_filtered_axis(2:end) (new_values(1) / 100)];
        speed_raw_axis = [speed_raw_axis(2:end) (new_values(2) / 100)];
    else
        x_axis = [x_axis etime(clock, start_time)];
        speed_filtered_axis = [speed_filtered_axis (new_values(1) / 100)];
        speed_raw_axis = [speed_raw_axis (new_values(2) / 100)];
    end
    
    % Update plot
    if counter == update_period
        set(plotHandle(1), 'XData', x_axis, 'YData', speed_filtered_axis);
        set(plotHandle(2), 'XData', x_axis, 'YData', speed_raw_axis);
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
end

% Stop motor and close port
fwrite(port, 0, 'int16');
fclose(dat);
disp('Connection is closed!');