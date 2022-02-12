clear all
path = 'D:\RTOS_info\Distrib_STM32\Distrib\ChibiStudio_v0\ChibiStudio\workspace176\bb-8_project\controller_bb_8_driver\matlab\files\';
if ~isfolder(path)
    disp 'Folder is not found!'
    return;
else
    disp 'Folder is found!'
end

filePattern = fullfile(path, '*mat');
files = dir(filePattern);

if isempty(files)
    disp 'No files found'
    return;
end

for i = 1:length(files)
    name = files(i).name;
    fullFileName = fullfile(files(i).folder, name);

    fileData = load(fullFileName, '-mat');
    data = fileData.total;

    plot(data, 'DisplayName', name)
    hold on
    grid on
    legend
end

saveas(figure(1), [pwd '/files/freq.fig']);
