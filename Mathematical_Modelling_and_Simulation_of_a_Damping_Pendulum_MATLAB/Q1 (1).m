clc;
clear;

h = 0.1;
m = 0.3;
g = 9.81;
r = 1;
k = sqrt(m*g/r);
L = [0.03 0.05 0.07 0.09]; %Lambda values
T0 = [10 15 19]; %initial Theta values in deg
T0 = deg2rad(T0);%converting from deg to rad
A = sqrt(((k^2)/m) - ((L.^2)/(4*(m^2))));
t = 0:0.1:8;
A_disp = zeros(3,4,length(t));
A_vel = zeros(3,4,length(t));
RK_disp_eq4 = zeros(3,4,length(t));
RK_disp_eq6 = zeros(3,4,length(t));
RK_vel_eq4 = zeros(3,4,length(t));
RK_vel_eq6 = zeros(3,4,length(t));

for i = 1:3
    for j = 1:4
        for x = 1:length(t)
            A_vel(i,j,x) = Analytical_Vel(m, T0(i), L(j), A(j), t(x)); %creating a 3d matrix to store all valueas of analytical velocities.
        end
    end
end
for i = 1:3
    for j = 1:4
        for x = 1:length(t)
            A_disp(i,j,x) = Analytical_disp(m, T0(i), L(j), A(j), t(x));%creating a 3d matrix to store all valueas of analytical displacements.
        end
    end
end
for i = 1:3
    for j = 1:4
        V = 0;
        T = T0(i);
        for x = 1:length(t)
            [T, V] = RK(@(t, T, V) eq4(m, g, r, T, V, L(j)), h, T, V, t(x));%creating a 3d matrix to store all valueas of RK velocities And displacements for eq4
            RK_disp_eq4(i,j,x) = T;
            RK_vel_eq4(i,j,x) = V;
        end
    end
end

for i = 1:3
    for j = 1:4
        V = 0;
        T = T0(i);
        for x = 1:length(t)
            [T, V] = RK(@(t, T, V) eq6(m, T, V, L(j), k), h, T, V, t(x));%creating a 3d matrix to store all valueas of RK velocities And displacements for eq6.
            RK_disp_eq6(i,j,x) = T;
            RK_vel_eq6(i,j,x) = V;
        end
    end
end
%plotting displacemnt plots
for i = 1:3
    for j = 1:4
        hold on;
        hold on;
        plot(squeeze(A_disp(i,j,:)),Color='r');
        plot(squeeze(RK_disp_eq4(i,j,:)),Color='G');
        plot(squeeze(RK_disp_eq6(i,j,:)),Color='B');
        figure;
    end
end
%plotting velocity plots
for i = 1:3
    for j = 1:4
        hold on;
        hold on;
        plot(squeeze(A_vel(i,j,:)),Color='r');
        plot(squeeze(RK_vel_eq4(i,j,:)),Color='G');
        plot(squeeze(RK_vel_eq6(i,j,:)),Color='B');
        figure;
    end
end

