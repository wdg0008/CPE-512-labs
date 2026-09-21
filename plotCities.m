%% Homework #3 Traveling Salesman Problem
% MATLAB file for results analysis
% 2026-09-19

clearvars;
close all;
clc;

csvdata = readtable("time_serial.csv", "CommentStyle", "#");

n = csvdata.Cities;
T = csvdata.Time_s;

% Ignore measurements below 1 second
idx = T >= 1;

n_fit = n(idx);
T_fit = T(idx);

% Fit linearized log(T) = log(C) + log(n!)
% (polyfit(x,y,n) n-degree polynomial
p = polyfit(gammaln(n_fit + 1), log(T_fit), 1);

slope = p(1);
C = exp(p(2)); % intercept is log(C)

fprintf("T(n) = %.4e * n! seconds\n", C);
fprintf("Linear regression slope is %g.\n", p(1));
disp("A slope near 1 means this is a good fit.")

figure;
plot(n, T);
xlabel("Number of Cities");
ylabel("Execution Time (s)");
title("Serial TSP Execution Time");
grid on;

figure;
plot(n, log10(T));
xlabel("Number of Cities");
ylabel(" log_{10}(T)");
title("Serial TSP Execution Time");
grid on;