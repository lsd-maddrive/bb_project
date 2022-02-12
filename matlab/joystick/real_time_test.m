close all
clear, clc

% Init gamepad (need to install simulink 3d animation add-on)
joy = vrjoystick(1);
stop_check = 0;

% Init variables
x_axis = [0];
speed_A_axis = [0];
speed_B_axis = [0];
speed_C_axis = [0];
update_period = 10;         % How often to update plot, points
time_window = 10;           % Time window to be shown on figure, sec
counter = 0;

% Figure settings
figure(1);
set(gcf,'CurrentCharacter','@');                            % Set to a dummy character
set(gcf, 'MenuBar', 'none', 'WindowState', 'maximized');    % Set figure to fullscreen and make none interactible
plotHandle = plot(NaN, NaN, '-b',...
                  NaN, NaN, '-g',...
                  NaN, NaN, '-m');                          % Make initial empty plot
legend('Motor A', 'Motor B', 'Motor C');
xlim([0 time_window]);
ylim([-4 4]);
ylabel('Speed, rps');
xlabel('Time');
grid on
pause(0.5);
% Usart settings
port = serial('COM11', 'BaudRate', 115200);
port.InputBufferSize = 4096;
fopen(port);
set(port, 'ByteOrder', 'littleEndian');
disp('Connection is ready!');

new_values = [];
start_time = clock;

fwrite(port, cast(([0 0 0] * 100), 'int16'), 'int16');

while stop_check < 0.5
    counter = counter + 1;
    new_values = fread(port, [3, 1], 'int16');

    % Break from the loop if timeout occured
    if (strcmp(lastwarn,'Unsuccessful read: A timeout occurred before the Terminator was reached or SIZE values were available..'))
        disp('Timeout');
        break;
    end

    % Add new points to the plot and forget last one if necessary
    if x_axis(end) >= time_window
        x_axis = [x_axis(2:end) etime(clock, start_time)];
        speed_A_axis = [speed_A_axis(2:end) (new_values(1) / 100)];
        speed_B_axis = [speed_B_axis(2:end) (new_values(2) / 100)];
        speed_C_axis = [speed_C_axis(2:end) (new_values(3) / 100)];
    else
        x_axis = [x_axis etime(clock, start_time)];
        speed_A_axis = [speed_A_axis (new_values(1) / 100)];
        speed_B_axis = [speed_B_axis (new_values(2) / 100)];
        speed_C_axis = [speed_C_axis (new_values(3) / 100)];
    end

    % Update plot
    if counter == update_period
        set(plotHandle(1), 'XData', x_axis, 'YData', speed_A_axis);
        set(plotHandle(2), 'XData', x_axis, 'YData', speed_B_axis);
        set(plotHandle(3), 'XData', x_axis, 'YData', speed_C_axis);
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

    gamepad_vals = axis(joy, [1 2 3 4]);    % get data from gamepad
    V_xy_set = gamepad_vals(1:2);
    if abs(V_xy_set(1)) < 0.2 && abs(V_xy_set(2)) < 0.2
        V_xy_set = [0 0];
    end
    V_xy_set = V_xy_set * 0.5;
    w_set = gamepad_vals(4);
    if abs(w_set) < 0.2
        w_set = 0;
    end
    stop_check = gamepad_vals(3);
    % Calculate speed for each motor
    [w_A, w_B, w_C] = inverse_kinematic(V_xy_set(2),...
                        V_xy_set(1), w_set);
    [w_A w_B w_C]
    fwrite(port, cast(([w_A w_B w_C] * 100), 'int16'), 'int16');

end

% Stop motor and close port
fwrite(port, 500, 'int16');
fclose(port);
disp('Connection is closed!');
