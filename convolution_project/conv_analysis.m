%% Convolution Project Analysis
% Written exclusively by William D. Goins V ("Quin")
% 2026-11-22 for CPE 512 at The University of Alabama in Huntsville
% Designed to analyze performance of multi-threaded convolution algorithm

close all;
clearvars;
clc;

cost_scalar = 1;

%% Import 

csvdata = readtable("conv_benchmark_guided.csv");
thread_count = csvdata{:,1};
arrSize = csvdata{:,2};
execution_time = csvdata{:,3};

% list unique thread values
threadValues = unique(thread_count);
Nth = length(threadValues);

% list unique size values
sizeValues = unique(arrSize);
Nsz = length(sizeValues);

%% Compute Serial performance metrics
serialMask = (thread_count == 0);
serialTimes = execution_time(serialMask);

%% Compute OpenMP performance metrics
% Preallocate OpenMP performance metrics
sz = size(serialTimes);
openmpTimes = zeros(sz);
openmpSpeedup = zeros(sz);
openmpEfficiency = zeros(sz);
openmpCost = zeros(sz);

for k = 2:Nth % skip serial case
    threads = threadValues(k);
    threadMask = (thread_count == threads);

    j = k - 1; % index preallocated matrices
    openmpTimes(:,j) = execution_time(threadMask);
    openmpSpeedup(:,j) = serialTimes ./ openmpTimes(:,j);
    openmpEfficiency(:,j) = openmpSpeedup(:,j) ./ threads;
    openmpCost(:,j) = cost_scalar * threads * openmpTimes(:,j);
end

%% Plot Execution Time

figure;
plot(sizeValues, serialTimes, 'DisplayName', 'Serial');
hold on;

for k = 1:(Nth-1)
    threads = threadValues(k+1);
    hold on;
    plot(sizeValues, openmpTimes(:,k), ...
        'DisplayName', sprintf("%d cores", threads));
end

legend('Location', 'northwest');
title("Execution Time for Discrete Convolution vs Array Size");
xlabel("Array Size (elements)");
ylabel("Time (s)");
grid on;
hold off;

%% Plot Parallel Speedup

figure;

for k = 1:(Nth-1)
    threads = threadValues(k+1);
    hold on;
    plot(sizeValues, openmpSpeedup(:,k), ...
        'DisplayName', sprintf("%d cores", threads));
end

legend('Location', 'northwest');
title("Speedup for OpenMP Discrete Convolution vs Array Size");
xlabel("Array Size (elements)");
ylabel("Time (s)");
grid on;
hold off;

%% Plot Parallel Efficiency

figure;

for k = 1:(Nth-1)
    threads = threadValues(k+1);
    hold on;
    plot(sizeValues, openmpEfficiency(:,k), ...
        'DisplayName', sprintf("%d cores", threads));
end

legend('Location', 'best');
title("Efficiency for OpenMP Discrete Convolution vs Array Size");
xlabel("Array Size (elements)");
ylabel("Time (s)");
grid on;
hold off;

%% Plot Parallel Cost Function

figure;

plot(sizeValues, serialTimes, 'DisplayName', "Serial");

for k = 1:(Nth-1)
    threads = threadValues(k+1);
    hold on;
    plot(sizeValues, openmpCost(:,k), ...
        'DisplayName', sprintf("%d cores", threads));
end

legend('Location', 'best');
title("Cost for OpenMP Discrete Convolution vs Array Size");
xlabel("Array Size (elements)");
ylabel("Cost (s)");
grid on;
hold off;

%% Averages

averageSerialTime = mean(serialTimes);
averageOpenmpTimes = mean(openmpTimes, 1);
averageSpeedup = averageSerialTime ./ averageOpenmpTimes;
averageEfficiency = averageSpeedup ./ threadValues(2:end)';

figure;
plot(threadValues(2:end), averageSpeedup);
title("Average Speedup vs Thread Count");
xlabel("Thread Count");
ylabel("Speedup");

figure;
plot(threadValues(2:end), averageEfficiency)
title("Average Effieciency vs Thread Count");
xlabel("Thread Count");
ylabel("Efficiency");