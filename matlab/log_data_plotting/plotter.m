clear
clc
close all
filename = 'log2.csv';
opts = detectImportOptions(filename);
opts = setvaropts(opts, 'time', 'Type', 'datetime');
opts = setvaropts(opts, 'time', 'DatetimeFormat', 'HH:mm:ss:SSS');
log_file = readtable(filename, opts);
time = milliseconds(log_file{:, 1} - log_file{1, 1}) / 1000;
w_set = log_file{:, 6};
phi_set = log_file{:, 2};
phi_set_check = zeros(size(w_set));
instant_val = 0;
for i = 1:length(phi_set_check)
    instant_val = instant_val + w_set(i) * 0.1;
    if (instant_val > 360) || (instant_val < 0)
        instant_val = rem(instant_val, 360);
    end
    phi_set_check(i) = instant_val;
end
plot(time, phi_set)
hold on
plot(time, phi_set_check)
legend('angle calculated by stm', 'angle calculated in matlab', 'Location', 'northwest')
xlabel('time, sec')
grid on
