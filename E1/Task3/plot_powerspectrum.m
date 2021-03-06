% plot the powerspectrum
close all;
clear all;

pos = 0;
rows = 3; cols = 2;
figure(1);

n = 250; plot_1_0_n;
n = 258; plot_1_0_n;
n = 260; plot_1_0_n;

% f = 2
pos = 0;
rows = 3; cols = 2;
figure(2);

n = 250; plot_2_0_n;
n = 258; plot_2_0_n;
n = 260; plot_2_0_n;

% f = 2, phi = pi/2
pos = 0;
rows = 3; cols = 2;
figure(3);

n = 250; plot_1_PiHalf_n;
n = 258; plot_1_PiHalf_n;
n = 260; plot_1_PiHalf_n;

% % N = 250
% pos = 0; n = 250;
% rows = 4; cols = 2;
% figure;
% 
% plot_1_0_n;
% plot_2_0_n;
% plot_1_PiHalf_n;
% 
% % N = 258
% pos = 0; n = 258;
% rows = 4; cols = 2;
% figure;
% 
% plot_1_0_n;
% plot_2_0_n;
% plot_1_PiHalf_n;
% 
% % N = 260
% pos = 0; n = 260;
% rows = 4; cols = 2;
% figure;
% 
% plot_1_0_n;
% plot_2_0_n;
% plot_1_PiHalf_n;