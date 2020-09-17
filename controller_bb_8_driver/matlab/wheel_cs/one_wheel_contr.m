clear all
global dat
delete(instrfind);
dat = serial('COM11', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat); 
set(dat, 'ByteOrder', 'littleEndian'); 

disp 'Connection is ready!'

% send start command 
disp 'start!'
fwrite(dat, 's', 'uint8'); 

cur = []; 
total = [];
ref = []; 


iter = 6;
for i = 1:iter
    cur = fread(dat, [100, 1], 'float32');
    total = [total; cur];
end

disp 'Stop machine!'
fwrite(dat, 'x', 'uint8');

fclose(dat);
disp 'Connection is closed!'

time = [];
for i = 0:10:(iter * 1000 / 2 - 1)
   time = [time; i / 1000];
end

real_v = [];
for i = 1:length(total)
    if mod(i, 2) == 0
        ref = [ref; total(i, 1)];
    else
        real_v = [real_v; total(i, 1)];
    end
end



plot(time, real_v, 'DisplayName', 'real-v', 'LineWidth', 2)
grid on 
hold on 
plot(time, ref, 'DisplayName', 'ref-v', 'LineWidth', 1) 
xlabel('t, s')
legend