cl;

path = 'd:\hak\bd_resize\';
filename = 'pos_final.txt';
fid = fopen(filename,'r');
imageFilename = cell(0);
vehicle = cell(0);
i=1;
W = 7296; H = 5472;
Wn = 416; Hn=416;
while 1
    tline = fgetl(fid);
    if isempty(tline) || length(tline)==1
        break;
    end
    [filename, tline] = strtok(tline);
    remain = tline;
    imageFilename{i} = [path filename];
%     fprintf(fid2, '%s ', filename);
    vehicle{i} = [];
    while ~isempty(remain) && (remain ~= "") 
        [token,remain] = strtok(remain, ' ');
        pos = str2num(token);
        if isempty(pos), break; end
        pos = pos(2:end);
        pos = [pos(1)/W*Wn,pos(2)/H*Hn,(pos(3)-pos(1))/W*Wn,(pos(4)-pos(2))/H*Hn];
        pos = round(pos);
        vehicle{i} = [vehicle{i};pos];
    end
    i = i+1;
end
img = insertObjectAnnotation(imread(imageFilename{2}),'rectangle',vehicle{2},1);
figure; imshow(img);

trainingData = table(imageFilename',vehicle','VariableNames',{'imageFilename','vehicle'});
data.vehicleTrainingData = trainingData;
% data = load('vehicleTrainingData.mat');
% trainingData = data.vehicleTrainingData;
%% Specify the directory in which training samples are stored. Add full path to the file names in training data.

% dataDir = fullfile(toolboxdir('vision'),'visiondata');
% trainingData.imageFilename = fullfile(dataDir,trainingData.imageFilename);

%% Randomly shuffle data for training.
rng(0);
shuffledIdx = randperm(height(trainingData));
trainingData = trainingData(shuffledIdx,:);

%% Create an imageDatastore using the files from the table.
imds = imageDatastore(trainingData.imageFilename);
%% Create a boxLabelDatastore using the label columns from the table.

blds = boxLabelDatastore(trainingData(:,2:end));
%% Combine the datastores.

ds = combine(imds, blds);
%% Load a preinitialized YOLO v2 object detection network.

net = load('yolov2VehicleDetector.mat');
lgraph = net.lgraph
load('lgraph2.mat'); lgraph = replaceLayer(lgraph,'input',lgraph2.Layers(1));

%% Inspect the layers in the YOLO v2 network and their properties. You can also create the YOLO v2 network by following the steps given in Create YOLO v2 Object Detection Network.
% lgraph2.Layers
lgraph.Layers
%% Configure the network training options.

options = trainingOptions('sgdm',...
          'InitialLearnRate',0.001,...
          'Verbose',true,...
          'MiniBatchSize',16,...
          'MaxEpochs',100,...
          'Shuffle','never',...
          'VerboseFrequency',30,...
          'CheckpointPath',tempdir);
%% Train the YOLO v2 network.

% [detector,info] = trainYOLOv2ObjectDetector(ds,lgraph,options); % Обучение заново
load('detector.mat','detector'); [detector,info] = trainYOLOv2ObjectDetector(ds,detector,options); % Дообучение
detector
save('detector.mat','detector');
%% You can verify the training accuracy by inspecting the training loss for each iteration.

figure
plot(info.TrainingLoss)
grid on
xlabel('Number of Iterations')
ylabel('Training Loss for Each Iteration')


%% Read a test image into the workspace.

img = imread(imageFilename{1});
%% Run the trained YOLO v2 object detector on the test image for vehicle detection.

[bboxes,scores] = detect(detector,img);
%% Display the detection results.

if(~isempty(bboxes))
    img = insertObjectAnnotation(img,'rectangle',bboxes,scores);
end
figure
imshow(img)

