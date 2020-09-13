figure(10), imshow(Icrot);

BW = imbinarize(Icrot, 'adaptive');
figure(11), imshow(BW);

SE = strel(ones(2,2));

% BW = imclose(BW,SE);
BW = imerode(BW,SE);
BW = imdilate(BW,SE);
BW = bwareaopen(BW, 25);
figure(12), imshow(BW);

sBW = sum(BW);
[~, loc] = findpeaks(sBW, 'MinPeakDistance', 8);
figure(13), plot(sBW);
findpeaks(sBW, 'MinPeakDistance', 8);
diff(loc)
