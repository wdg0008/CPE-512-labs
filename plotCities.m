%% Homework #3 Traveling Salesman Problem
% MATLAB file for results analysis
% 2026-09-19

clearvars;
close all;
clc;

cases = ["Serial", "PTH1", "PTH2"]'; % column-major in memory
serialData = readtable("time_serial.csv", "CommentStyle", "#");
PTH1Data = readtable("time_PTH1.csv", "CommentStyle", "#");
PTH2Data = readtable("time_PTH2.csv", "CommentStyle", "#");

%% Analyze serial case

n = serialData.Cities;
t = serialData.Time_s;

N = {serialData.Cities, PTH1Data.Cities, PTH2Data.Cities};
T = {serialData.Time_s, PTH1Data.Time_s, PTH2Data.Time_s};

C = zeros(3,1); % estimate of constant multiple of O(n!)
s = zeros(3,1); % estimate of slope of ln, which would be a linear power

figure;
for i = 1:3
    [C(i), s(i)] = fitFactorial(N{i}, T{i});

    fprintf("\nBeginning of %s case\n\n", cases(i));
    fprintf("T(n) = %.4e * n! seconds\n", C(i));
    fprintf("Linear regression slope is %g.\n", s(i));
    disp("A slope near 1 means this is a good fit.")

    subplot(3,2,(2*i-1))
    plot(N{i}, T{i});
    xlabel("Number of Cities");
    ylabel("Execution Time (s)");
    title(sprintf("%s TSP Execution Time", cases(i)));
    grid on;

    subplot(3,2,(2*i))
    plot(N{i}, log10(T{i}));
    xlabel("Number of Cities");
    ylabel(" log_{10}(T)");
    title(sprintf("%s TSP Execution Time", cases(i)));
    grid on;
end

function [C, slope] = fitFactorial(n, t)
    assert(length(n) == length(t));

    threshold_s = 1; % ignore times less than this
    idx = (t >= threshold_s);

    n_fit = n(idx);
    T_fit = t(idx);

    % Fit linearized log(T) = log(C) + log(n!)
    % (polyfit(x,y,n) n-degree polynomial
    p = polyfit(gammaln(n_fit + 1), log(T_fit), 1);

    slope = p(1);
    C = exp(p(2)); % intercept is log(C)
end