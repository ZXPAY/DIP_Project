clc, clear, close all

fileID = fopen('txtHeaderName.txt', 'r');

while ~feof(fileID)
    tline = fgetl(fileID);
    if ~strcmp(tline, '')
        f = fopen(tline, 'r');
        gray = zeros([256, 1]);
        value = zeros([256, 1]);
        for n=1:256
            data = fscanf(f, '%s,%s');
            raw = strsplit(data, ',');
            gray(n) = str2num(raw{1});
            value(n) = str2num(raw{2});
        end
        fig = figure();
        bar(gray, value);
        grid on;
        title(tline(1:end-4));
        xlabel('Gray Level');
        ylabel('Number of pixels');
        xlim([0, 255]);
        ylim([0, max(value)]);
        saveas(fig, strcat(tline(1:end-4), 'Histogram.bmp'));
        fclose(f);
    end
end

fclose(fileID);



