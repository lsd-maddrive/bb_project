clear all
global dat
delete(instrfind);
dat = serial('COM20', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat); 
set(dat, 'ByteOrder', 'littleEndian'); 

disp "Connection is ready!"

start = 's';
fwrite(dat, start, 'uint8'); 

pool = []; 
total = []; 

for i = 1:10
    pool = fread(dat, [1000, 1], 'int16');
    total = [total; pool];
end