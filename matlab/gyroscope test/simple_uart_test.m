port = serial('COM4', 'BaudRate', 115200);
if port.Status == "open"
    fclose(port);
end
fopen(port);
set(port, 'ByteOrder', 'littleEndian');

while(1)
    if fread(port, 1) ~= 15
        fread(port);
        continue
    end
    buf = fread(port, 12)
    buf = typecast(uint8(buf), 'single')
end
