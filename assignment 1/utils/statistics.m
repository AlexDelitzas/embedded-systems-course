clear;
close all;


%% INITIALIZATION

filePath_v1 = '../logs/timestamps_v1.txt';
filePath_v2 = '../logs/timestamps_v2.txt';

dt = 0.1;

%% Version 1 - Calculate statistics

timestamps1 = csvread(filePath_v1);
ideal1 = timestamps1(:, 1) * dt;
real1 = timestamps1(:, 2);

temp = [0 ; real1(1:end-1)];

distance1 = real1 - temp;

error1 = real1 - ideal1;


figure
histogram(distance1, 'Normalization', 'probability')
xlim([0.0995 0.1005])
ylabel('Frequency', 'FontSize', 13)
xlabel('Time distance (sec)', 'FontSize', 13)
print -depsc hist1

figure 
plot(error1)
ylabel('Time error (sec)', 'FontSize', 13)
xlabel('n: n-th timestamp', 'FontSize', 13)
print -depsc error1

max1 = max(distance1);
min1 = min(distance1);
mean1 = mean(distance1);
median1 = median(distance1);
std1 = std(distance1);

disp(' ')
disp('==== Version 1 ====')
fprintf('Max: %.6f\n', max1)
fprintf('Min: %.6f\n', min1)
fprintf('Mean: %.6f\n', mean1)
fprintf('Median: %.6f\n', median1)
fprintf('Standard deviation: %.6f\n', std1)

%% Version 2 - Calculate statistics

timestamps2 = csvread(filePath_v2);
ideal2 = timestamps2(:, 1) * dt;
real2 = timestamps2(:, 2);

temp = [0 ; real2(1:end-1)];

distance2 = real2 - temp;

error2 = real2 - ideal2;

figure
histogram(distance2, 'Normalization', 'probability')
xlim([0.0995 0.1005])
ylabel('Frequency', 'FontSize', 13)
xlabel('Time distance (sec)', 'FontSize', 13)
print -depsc hist2

figure 
plot(error2)
ylabel('Time error (sec)', 'FontSize', 13)
xlabel('n: n-th timestamp', 'FontSize', 13)
print -depsc error2

max2 = max(distance2);
min2 = min(distance2);
mean2 = mean(distance2);
median2 = median(distance2);
std2 = std(distance2);

disp(' ')
disp('==== Version 2 ====')
fprintf('Max: %.6f\n', max2)
fprintf('Min: %.6f\n', min2)
fprintf('Mean: %.6f\n', mean2)
fprintf('Median: %.6f\n', median2)
fprintf('Standard deviation: %.6f\n', std2)

