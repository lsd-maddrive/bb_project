close all; clear; clc

deg2rad = pi / 180;

% read stl file
fv = stlread('platform.stl');

% place and orient obbject in initial position
x = fv.vertices(:, 1);
y = fv.vertices(:, 2);
z = fv.vertices(:, 3);
y = cos(pi) * y - sin(pi) * z;
z = sin(pi) * y + cos(pi) * z;
x = x - (min(x) + (max(x) - min(x)) / 2);
y = y - (min(y) + (max(y) - min(y)) / 2);
z = z - (min(z) + (max(z) - min(z)) / 2);
fv.vertices = [x, y ,z];

% plot initial state of 3d object and horizon level
h = figure('units','normalized','outerposition',[0 0 1 1]);
p = patch(fv,'FaceColor',       [0.8 0.8 1.0], ...
     'EdgeColor',       'none',        ...
     'FaceLighting',    'gouraud',     ...
     'AmbientStrength', 0.15);
hold on
grid on
axis([min(x)*3 max(x)*3 min(y)*3 max(y)*3 min(z)*3 max(z)*3]);
camlight('headlight');
material('dull');
view([0 0]);
surf([0 min(x)*3 max(x)*3], [0 min(y)*4 max(y)*4], [0 0 0; 0 0 0; 0 0 0], 'FaceAlpha', 0.1, 'FaceColor', 'r')

% set frequency of plot update
r = rateControl(10);
reset(r)

% Configure and open tcp server
serv = tcpip('0.0.0.0', 8081, 'NetworkRole', 'Server');
fopen(serv)
disp 'Someone connected'

while(1)
    % Get angles values from raspberry
    buf = fread(serv, 12);
    buf = typecast(uint8(buf), 'single');
    angX = buf(1);
    angY = buf(2);
    angZ = buf(3);
    
    % delete object before redraw
    delete(p);
    
    % rotate object
    % around X
    y1 = cos(angX * deg2rad) * y - sin(angX * deg2rad) * z;
    z1 = sin(angX * deg2rad) * y + cos(angX * deg2rad) * z;
    
    % around Y
    x1 = cos(angY * deg2rad) * x - sin(angY * deg2rad) * z1;
    z2 = +sin(angY * deg2rad) * x + cos(angY * deg2rad) * z1;
    
    % around Z
    x2 = cos(angZ * deg2rad) * x1 + sin(angZ * deg2rad) * y1;
    y2 = -sin(angZ * deg2rad) * x1 + cos(angZ * deg2rad) * y1;
    
    % redraw plot
    fv.vertices = [x2, y2, z2];
    p = patch(fv,'FaceColor',       [0.8 0.8 1.0], ...
         'EdgeColor',       'none',        ...
         'FaceLighting',    'gouraud',     ...
         'AmbientStrength', 0.3);
    
    % pause to keep desired frequency
    waitfor(r);
end