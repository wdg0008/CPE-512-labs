%% Convolution Project Testing
% Written exclusively by William D. Goins V ("Quin")
% 2026-11-22 for CPE 512 at The University of Alabama in Huntsville

close all;
clearvars;
clc;

len = 2^4;
x1 = ones(len, 1);
y1 = conv(x1,x1);

figure;
stem(0:numel(y1)-1, y1, 'filled');
xlabel('Sample Index');
ylabel('Amplitude');
title('Convolution of Two Unit Sequences');
grid on;