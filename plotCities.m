%% Homework #3 Traveling Salesman Problem
% MATLAB file for results analysis
% 2026-09-19

clearvars;
close all;
clc;

cases = ["Serial", "PTH1", "PTH2"]'; % column-major in memory
dataFiles = ["time_serial.csv","time_PTH1.csv","time_PTH2.csv"];

% Pre-allocate space for city count and time required
% cells must be used because PTH1 only has 14 samples, not 15 like others
N = cell(3,1);
T = cell(3,1);
for i = 1:3 % read data in through a loop
    tbl = readtable(dataFiles(i), "CommentStyle", "#");
    N{i} = tbl.Cities;
    T{i} = tbl.Time_s;
end

% i = 0 is serial
% i = 1 for PTH1
% i = 2 for PTH2

%% Plotting and Estimating Execution Time

% Pre-allocate estimation variables
C = zeros(3,1); % estimate of constant multiple of O(n!)
s = zeros(3,1); % estimate of slope of ln, which would be a linear power

figure;
for i = 1:3
    [C(i), s(i)] = fitFactorial(N{i}, T{i});

    fprintf("\nBeginning of %s case\n", cases(i));
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

%% Efficiency

eff = cell(2,1);

figure;
for i = 1:length(eff)
    j = i+1; % ignore the serial case
    % index serial time to ignore extra samples at the end
    eff{i} = T{1}(1:length(T{j})) ./ (N{j} .* T{j});

    subplot(1,2,i);
    plot(N{1}(1:length(N{j})), eff{i});
end

%% Cost

C_cost = zeros(2,1);
s_cost = zeros(2,1);

cost = cell(2,1);
for i = 1:length(cost)
    j = i + 1; % adjust for element 1 being serial
    L = length(N{j}); % how many points to operate on
    cost{i} = N{j} .* T{j};
    [C_cost(i), s_cost(i)] = fitFactorial(N{1}(1:L), cost{i});

    fprintf("%s cost fit: %.4e * n! city-seconds (slope %.3f)\n", ...
        cases(j), C_cost(i), s_cost(i));
    
    subplot(1,2,i);
    plot(N{j}, cost{i});
    xlabel("Number of Cities");
    ylabel("Cost (city-seconds)");
    title(sprintf("%s TSP Cost", cases(j)));
    grid on;
end

%% Functions

% Perform linear regression to estimate coefficient
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