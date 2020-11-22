close all
time = 0.01:0.01:69.71;
K = polyfit(time, data_to_save(:,2), 1);
appr = K(1) * time + K(2);
plot(time, appr, 'LineWidth', 2)
hold on
plot(time, data_to_save(:,2), 'LineWidth', 2)
grid on
legend('Actual wheel speed', 'Approximated wheel speed', 'location', 'northwest', 'FontSize', 24)
xlabel('Time, sec', 'FontSize', 24)
ylabel('Speed, rps', 'FontSize', 24)
set(gca, 'FontSize', 24)