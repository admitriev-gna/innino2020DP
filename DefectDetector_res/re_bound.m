clc; close all; clear all;


% path  = 'd:\hak\bd\';
% list = dir(path);
filename = 'pos.txt';
fid = fopen(filename,'r');

filename2 = 'pos_new_new.txt';
fid2 = fopen(filename2,'a');

i = 1;
while 1
    tline = fgetl(fid);
    if isempty(tline)
        break;
    end
    [filename, tline] = strtok(tline);
    remain = tline;
    fprintf(fid2, '%s ', filename);
    i = i+1;
    while (remain ~= "")
        [token,remain] = strtok(remain, ' ');
        pos = str2num(token);
        if isempty(pos), break; end

        I = imread(['bd\',filename]);
        [h,w,c] = size(I);
        %        figure(1); imshow(I);
        roi = [pos(1),pos(2),pos(3)-pos(1),pos(4)-pos(2)];
        %        roi = drawrectangle('Position',roi); roi = roi.Position;

        big_roi = [roi(1)-roi(3),roi(2)-roi(4),roi(3)*3,roi(4)*3];
        big_roi(big_roi<1)=1;
        Ic = imcrop(I,big_roi);
        hh = figure(2); imshow(Ic,[]); title(num2str(i));
        roic = [roi(1)-big_roi(1), roi(2)-big_roi(2), roi(3),roi(4)];
        roic = drawrectangle('Position',roic,'InteractionsAllowed','all');
        wait(roic);
        roic = roic.Position;
        new_roi = [roic(1)+big_roi(1),roic(2)+big_roi(2), roic(3), roic(4)];
        %        figure(3); imshow(I); drawrectangle('Position',new_roi);
        new_roi = int32(new_roi);
        fprintf(fid2, '0,%d,%d,%d,%d ', new_roi(1),new_roi(2),new_roi(1)+new_roi(3),new_roi(2)+new_roi(4)); 
    end
    fprintf(fid2, '\r\n'); 
end


fclose(fid)
fclose(fid2)


%     