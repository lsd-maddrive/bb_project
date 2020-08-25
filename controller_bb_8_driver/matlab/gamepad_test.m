joy = vrjoystick(1);
plothandle = plot(NaN, NaN);
xlim([-0.8 0.8]);
ylim([-0.8 0.8]);
loop = 0;
while loop < 0.5
    pause(0.2);
    new_val = axis(joy, [1 2 3 4]);
    x = new_val(1:2);
    w = new_val(4);
    loop = new_val(3);
    if abs(x(1)) < 0.2 && abs(x(2)) < 0.2
        x = [0 0];
    end
    if abs(w) < 0.2
        w = 0;
    end
    
    x = x * 0.5;
    set(plothandle, 'XData', [0 x(1)], 'YData', [0 -x(2)])
    drawnow;
    [w_A, w_B, w_C] = inverse_kinematic(x(2), x(1), w)
end