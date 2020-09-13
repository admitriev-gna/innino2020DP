function ang = calc_angle(Icc)

% Icc = imread('1.jpg');
Icc2 = rgb2gray(Icc);
L = imsegkmeans(Icc2, 2, 'NumAttempts', 2);
ind = find(L == 1);
Icc2(ind) = 0;
Icc2(Icc2 > 0) = 1;
BW = logical(Icc2);
% figure(5), imshow(BW);

[H,T,R] = hough(BW);
% HH = medfilt2(H, [10, 10]);
H = medfilt2(H,[5,5]);

P = houghpeaks(H, 1);
% figure(6), imshow(H,[],'XData',T,'YData',R,'InitialMagnification','fit');
% figure(6), surf(H,'XData',T,'YData',R);
% xlabel('\theta'), ylabel('\rho');
% axis on, axis normal, hold on;
% plot(T(P(:,2)),R(P(:,1)),'s','color','white');
% hold off;
ang = T(P(2));
% Icc_rot = imrotate(Icc, ang+90);
% figure(7), imshow(Icc_rot);

% %% other metod
% if 0
% [BW, maskedImage] = segmentImage(Icc);
% figure(51), imshow(BW);
% figure(52), imshow(maskedImage);
% [H,T,R] = hough(BW);
% P = houghpeaks(H ,1);
% figure(61), imshow(H,[],'XData',T,'YData',R,...
%             'InitialMagnification','fit');
% xlabel('\theta'), ylabel('\rho');
% axis on, axis normal, hold on;
% plot(T(P(:,2)),R(P(:,1)),'s','color','white');
% hold off;
% ang = T(P(2))
% Icc_rot = imrotate(Icc, ang+90);
% figure(71), imshow(Icc_rot);
% end
