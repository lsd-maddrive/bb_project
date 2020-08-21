close all
clear, clc

% Init buffer
plot_buffer_size = 1000;   % Number of points to be shown simultaneously
x_axis = ones(1,plot_buffer_size)*NaN;
speed_filtered_axis = ones(1,plot_buffer_size)*NaN;
speed_raw_axis = ones(1,plot_buffer_size)*NaN;
update_period = 10;
counter = 0;

% Figure settings
figure(1);
set(gcf,'CurrentCharacter','@');                    % Set to a dummy character
set(gcf, 'Position', get(0, 'Screensize'));         % Set figure to fullscreen
plotHandle = plot(x_axis, speed_filtered_axis, '-r',...
                  x_axis, speed_raw_axis, '-b');    % Make initial empty plot
xlim([1 plot_buffer_size]);
ylim([-2 3])
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
    x_axis = [x_axis(2:plot_buffer_size) etime(clock, start_time)];
    speed_filtered_axis = [speed_filtered_axis(2:plot_buffer_size) (new_values(1) / 100)];
    speed_raw_axis = [speed_raw_axis(2:plot_buffer_size) (new_values(2) / 100)];
    
    % Adjust xscale
    if ~isnan(x_axis(end))
        xlim([x_axis(1) x_axis(end)]);
    end
    
    % Update plot
    if counter == update_period
        set(plotHandle(1), 'XData', x_axis, 'YData', speed_filtered_axis);
        set(plotHandle(2), 'XData', x_axis, 'YData', speed_raw_axis);
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