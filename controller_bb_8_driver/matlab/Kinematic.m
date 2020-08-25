Vx = 1;
Vy = 0;
w = 0;
x = [Vx; Vy; w];
R = 10;
L = 20;
A = [0              -1              L;...
    cos(pi / 6)     sin(pi / 6)     L;...
    -cos(pi / 6)    sin(pi / 6)     L];
wheels = 1 / R * A * x;
% https://github.com/Daveorr/Omni-directional-Robot-Model