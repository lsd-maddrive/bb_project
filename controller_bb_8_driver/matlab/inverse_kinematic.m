function [w_A, w_B, w_C] = inverse_kinematic(V_x, V_y, w)
    input = [V_x; V_y; w];
    R = 0.05;   % Radius of wheels, m
    L = 0.2;      % Distance between base center and wheel center, m
    A = [0              -1              L;...
        cos(pi / 6)     sin(pi / 6)     L;...
        -cos(pi / 6)    sin(pi / 6)     L];
    output = A / R * input / (2 * pi);
    w_A = output(1);
    w_B = output(2);
    w_C = output(3);
end