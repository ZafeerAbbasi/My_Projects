clear all;
clc;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Define Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%SET1
% m1 = 290;       %[kg]
% m2 = 15;        %[kg]
% k1 = 16200;     %[N/m]
% k2 = 191000;    %[N/m]
% b1 = 1000;      %[Ns/m]
% b2 = 2500;      %[Ns/m]
% t = 0:0.001:20;  %[s]
% u = 0.1;        %[m]

%SET2
% m1 = 404;       %[kg]
% m2 = 42;        %[kg]
% k1 = 21562;     %[N/m]
% k2 = 186220;    %[N/m]
% b1 = 2940;      %[Ns/m]
% b2 = 1400;      %[Ns/m]
% t = 0:0.001:7;  %[s]
% u = 0.1;        %[m]

%SET3 -> Main SET
m1 = 208;       %[kg]
m2 = 28;        %[kg]
k1 = 18709;     %[N/m]
k2 = 127200;    %[N/m]
b1 = 1300;      %[Ns/m]
b2 = 10;        %[Ns/m]
t = 0:0.001:5;  %[s]
u = 0.1;        %[m]

b3 = 3000;  % Varying damping coefficent of suspension system
m3 = 500;   % Increased vehicle mass
m4 = 50;    % Decreased vehicle mass


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Define transfer function
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Transfer function for Y1(t) as output
% Y1(s) -> num1
% U(s)  -> den1
num1 = [b1*b2 (b2*k1 + b1*k2) k1*k2];
den1 = [m1*m2 (m1*b1 + m1*b2 + m2*b1) (m1*k1 + m1*k2 + m2*k1 + b1*b2) (b1*k2 + b2*k1) k1*k2];
G1 = tf(num1,den1);
s = tf('s');
G1_deriv = G1*s;

% Transfer function for Y2(t) as output
% Y2(s) -> num2
% U(s)  -> den2
num2 = [m1*b2 (b1*b2 + m1*k2) (b2*k1 + b1*k2) k1*k2];
den2 = [m1*m2 (m1*b1 + m1*b2 + m2*b1) (m1*k1 + m1*k2 + m2*k1) (b1*b2 + b1*k2 + b2*k1) k1*k2];
G2 = tf(num2,den2);

% Transfer function for Y1(t) as output WITH NEW DAMPING COEFFICENT 
% Y1(s) -> num3
% U(s)  -> den3
num3 = [b3*b2 (b2*k1 + b3*k2) k1*k2];
den3 = [m1*m2 (m1*b3 + m1*b2 + m2*b3) (m1*k1 + m1*k2 + m2*k1 + b3*b2) (b3*k2 + b2*k1) k1*k2];
G3 = tf(num3,den3);
s1 = tf('s');
G3_deriv = G3*s1;

% Transfer function for Y1(t) as output WITH GREATER VEHICLE WEIGHT
% Y1(s) -> num4
% U(s)  -> den4
num4 = [b1*b2 (b2*k1 + b1*k2) k1*k2];
den4 = [m3*m2 (m3*b1 + m3*b2 + m3*b1) (m3*k1 + m3*k2 + m2*k1 + b1*b2) (b1*k2 + b2*k1) k1*k2];
G4 = tf(num4,den4);
s2 = tf('s');
G4_deriv = G4*s2;

% Transfer function for Y1(t) as output WITH SMALLER VEHICLE WEIGHT
% Y1(s) -> num5
% U(s)  -> den5
num5 = [b1*b2 (b2*k1 + b1*k2) k1*k2];
den5 = [m4*m2 (m4*b1 + m4*b2 + m2*b1) (m4*k1 + m4*k2 + m2*k1 + b1*b2) (b1*k2 + b2*k1) k1*k2];
G5 = tf(num5,den5);
s3 = tf('s');
G5_deriv = G5*s3;



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Defining State-Space model
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
A = [0, 1, 0, 0; -k1/m1, -b1/m1, k1/m1, b1/m1; 0, 0, 0, 1; k1/m2, b1/m2, -(k1 + k2)/m2, -(b1 + b2)/m2];
B = [0, 0; 0, 0; 0, 0; k2/m2, b2/m2];
% C = [1, 0, 0, 0; 0, 0, 1, 0; 0, 1, 0, 0];
C = [1, 0, 0, 0; 0, 0, 1, 0];
% D = [0, 0; 0, 0;0, 0];
D = [0, 0; 0, 0];
state_space = ss(A,B,C,D);
% % [n1,d1] = ss2tf(A,B,C,D,2);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Defining State-Space model from Linear Graph
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
A1 = [-b1/m1, b1/m1, 1/m1, 0; b1/m2, -(b1 + b2)/m2, -1/m2, 1/m2; -k1, k1, 0, 0; 0, -k2, 0, 0];
B1 = [0; b2/m2; 0; k2];
% C = [1, 0, 0, 0; 0, 0, 1, 0; 0, 1, 0, 0];
C1 = [0, 0, 1, 0; 0, 0, 0, 1;];
% D = [0, 0; 0, 0;0, 0];
D1 = [0; 0];
state_space1 = ss(A1,B1,C1,D1);
% % [n1,d1] = ss2tf(A,B,C,D,2);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running step response No derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% z1 = u*step(G1,t);
% z2 = u*step(G2,t);
% plot(t,z1,'b')
% grid on
% hold on
% plot(t,z2,'r')
% hold off
% ylabel('Displacement [m]')
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]")
% [t1, t2] = title({'\color{black}Fig 8.1 Step Response';'\color{blue}u = 0.1 [m]'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running step response with derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% z1 = u*step(G1,t);
% z2 = u*step(G2,t);
% dydx = gradient(z1(:)) ./ gradient(t(:));
% plot(t,z1,'b')
% grid on
% hold on
% plot(t,z2,'r')
% hold on
% plot(t,dydx,'black')
% hold off
% ylabel('Amplitude')
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]","d/dt Y1(t) [m/s]")
% [t1, t2] = title({'\color{black}Fig 8.2 Step Response with d/dt Y1(t)';'\color{blue}u = 0.1 [m]'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running square wave response No derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% [u1,t] = gensig("square",5,15);
% h = lsimplot(G1,G2,u*u1,t);
% %setoptions(h, 'Grid', 'on')
% ylabel('Displacement [m]')
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]")
% [t1, t2] = title({'\color{black}Fig 8.3 Square Wave Response';'\color{blue}u = 0.1 [m]'});
% grid on



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running square wave response with derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% [u1,t] = gensig("square",5,15);
% h = lsimplot(G1,'b',G2,'r',G1_deriv,'black',u*u1,t)
% setoptions(h, 'Grid', 'on')
% grid on
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]","d/dt Y1(t) [m/s]")
% [t1, t2] = title({'\color{black}Fig 8.4 Square Wave Response with d/dt Y1(t)';'\color{blue}u = 0.1 [m]'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Ideal highway road profile response No derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% h0 = 0.015;              % Road irregularity amplitude
% v = 80*0.44704;          % Vehicle speed
% L = 13;                  % Road irregularity length
% omega = (2*3.14*v)/L;    % Angular frequency
% epsilon = 0;             % Phase offset
% t = 0:0.001:3;  %[s]
% 
% h = h0*sin(omega*t + epsilon);
% y = lsimplot(G1,'b',G2,'r',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% ylabel('Displacement [m]')
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]")
% [t1, t2] = title({'\color{black}Fig 8.5 Ideal Highway Road Profile Response';'\color{blue}Road irregularity amplitude = 0.015[m], Vehicle speed = 80 [mph]'});
% 


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Ideal highway road profile response with derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% h0 = 0.015;              % Road irregularity amplitude [m]
% v = 80*0.44704;          % Vehicle speed [m/s]
% L = 13;                  % Road irregularity length [m]
% omega = (2*3.14*v)/L;    % Angular frequency
% epsilon = 0;             % Phase offset
% t = 0:0.001:3;           %[s]
% 
% h = h0*sin(omega*t + epsilon);
% y = lsimplot(G1,'b',G2,'r',G1_deriv,'k',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]","d/dt Y1(t) [m/s]")
% [t1, t2] = title({'\color{black}Fig 8.6 Ideal Highway Road Profile Response with d/dt Y1(t)';'\color{blue}Road irregularity amplitude = 0.015[m], Vehicle speed = 80 [mph]'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Realistic road profile response using Fourier Series No derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% In this case we are creating an arbitrary road profile input function.
% Using the fourier series, we will create a 6th order harmonic fucntion
% to represent the road conditions of Urban roads made of asphalt concrete.
% For simplicity's sake we will make b0 = 0, all phase offsets = 0, and
% omega = 1.

% b0 = 0;
% omega = 1;
% t = 0:0.001:10;           
% b0 = 0;
% b1 = 0.01;
% b2 = 0.01;
% b3 = 0.002;
% b4 = 0.001;
% b5 = 0.001;
% b6 = 0.002;
% h = b0 + b1*sin(omega*t) + b2*sin(2*omega*t) + b3*sin(3*omega*t) + b4*sin(4*omega*t) + b5*sin(5*omega*t) + b6*sin(6*omega*t);
% 
% plot(t,h)
% grid on
% ylabel('Displacement [m]')
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]")
% [t1, t2] = title({'\color{black}Fig 8.7 Realistic Road Profile Input using Fourier Series';'\color{blue}Omega = 1'});

% 
% y = lsimplot(G1,'b',G2,'r',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% ylabel('Displacement [m]')
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]")
% [t1, t2] = title({'\color{black}Fig 8.8 Realistic Road Profile Response using Fourier Series';'\color{blue}Omega = 1'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Realistic road profile response using Fourier Series with derivative
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% In this case we are creating an arbitrary road profile input function.
% Using the fourier series, we will create a 6th order harmonic fucntion
% to represent the road conditions of Urban roads made of asphalt concrete.
% For simplicity's sake we will make b0 = 0, all phase offsets = 0, and
% omega = 1.

% b0 = 0;
% omega = 1;
% t = 0:0.001:10;           
% b0 = 0;
% b1 = 0.01;
% b2 = 0.01;
% b3 = 0.002;
% b4 = 0.001;
% b5 = 0.001;
% b6 = 0.002;
% h = b0 + b1*sin(omega*t) + b2*sin(2*omega*t) + b3*sin(3*omega*t) + b4*sin(4*omega*t) + b5*sin(5*omega*t) + b6*sin(6*omega*t);
% 
% y = lsimplot(G1,'b',G2,'r',G1_deriv,'k',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% xlabel('Time [s]')
% legend("Y1(t) [m]","Y2(t) [m]","d/dt Y1(t) [m/s]")
% [t1, t2] = title({'\color{black}Fig 8.9 Realistic Road Profile Response using Fourier Series with d/dt Y1(t)';'\color{blue}Omega = 1'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Realistic road profile response using Fourier Series No
% derivative, varying the damping coefficent
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% In this case we are creating an arbitrary road profile input function.
% Using the fourier series, we will create a 6th order harmonic fucntion
% to represent the road conditions of Urban roads made of asphalt concrete.
% For simplicity's sake we will make b0 = 0, all phase offsets = 0, and
% omega = 1.

% b0 = 0;
% omega = 1;
% t = 0:0.001:10;           
% b0 = 0;
% b1 = 0.01;
% b2 = 0.01;
% b3 = 0.002;
% b4 = 0.001;
% b5 = 0.001;
% b6 = 0.002;
% h = b0 + b1*sin(omega*t) + b2*sin(2*omega*t) + b3*sin(3*omega*t) + b4*sin(4*omega*t) + b5*sin(5*omega*t) + b6*sin(6*omega*t);
% 
% y = lsimplot(G1,'b',G3,'r',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% ylabel('Displacement [m]')
% xlabel('Time [s]')
% legend("Y1(t)[m] ~b = 1300[Ns/m]","Y1(t)[m] ~b = 3000[Ns/m]")
% [t1, t2] = title({'\color{black}Fig 8.10 Realistic Road Profile Response with Varying Damping';'\color{blue}Omega = 1'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Realistic road profile response using Fourier Series with
% derivative, varying the damping coefficent
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% In this case we are creating an arbitrary road profile input function.
% Using the fourier series, we will create a 6th order harmonic fucntion
% to represent the road conditions of Urban roads made of asphalt concrete.
% For simplicity's sake we will make b0 = 0, all phase offsets = 0, and
% omega = 1.

% b0 = 0;
% omega = 1;
% t = 0:0.001:10;           
% b0 = 0;
% b1 = 0.01;
% b2 = 0.01;
% b3 = 0.002;
% b4 = 0.001;
% b5 = 0.001;
% b6 = 0.002;
% h = b0 + b1*sin(omega*t) + b2*sin(2*omega*t) + b3*sin(3*omega*t) + b4*sin(4*omega*t) + b5*sin(5*omega*t) + b6*sin(6*omega*t);
% 
% y = lsimplot(G1,'b',G3,'r',G1_deriv,'k',G3_deriv,'y',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% xlabel('Time [s]')
% legend("Y1(t)[m] ~b = 1300[Ns/m]","Y1(t)[m] ~b = 3000[Ns/m]","d/dt Y1(t)[m/s] ~b = 1300[Ns/m]","d/dt Y1(t)[m/s] ~b = 3000[Ns/m]")
% [t1, t2] = title({'\color{black}Fig 8.11 Realistic Road Profile Response with Varying Damping with d/dt Y1(t)';'\color{blue}Omega = 1'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Realistic road profile response using Fourier Series No
% derivative, varying the vehicle weight
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% In this case we are creating an arbitrary road profile input function.
% Using the fourier series, we will create a 6th order harmonic fucntion
% to represent the road conditions of Urban roads made of asphalt concrete.
% For simplicity's sake we will make b0 = 0, all phase offsets = 0, and
% omega = 1.

% b0 = 0;
% omega = 1;
% t = 0:0.001:10;           
% b0 = 0;
% b1 = 0.01;
% b2 = 0.01;
% b3 = 0.002;
% b4 = 0.001;
% b5 = 0.001;
% b6 = 0.002;
% h = b0 + b1*sin(omega*t) + b2*sin(2*omega*t) + b3*sin(3*omega*t) + b4*sin(4*omega*t) + b5*sin(5*omega*t) + b6*sin(6*omega*t);
% 
% y = lsimplot(G1,'b',G4,'r',G5,'y',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% ylabel('Displacement [m]')
% xlabel('Time [s]')
% legend("Y1(t)[m] ~m1 = 208[kg]","Y1(t)[m] ~m1 = 500[kg]","Y1(t)[m] ~m1 = 50[kg]" )
% [t1, t2] = title({'\color{black}Fig 8.12 Realistic Road Profile Response with Varying Vehicle Weight';'\color{blue}Omega = 1'});



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Running Realistic road profile response using Fourier Series with
% derivative, varying the vehicle weight
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% In this case we are creating an arbitrary road profile input function.
% Using the fourier series, we will create a 6th order harmonic fucntion
% to represent the road conditions of Urban roads made of asphalt concrete.
% For simplicity's sake we will make b0 = 0, all phase offsets = 0, and
% omega = 1.

% b0 = 0;
% omega = 1;
% t = 0:0.001:10;           
% b0 = 0;
% b1 = 0.01;
% b2 = 0.01;
% b3 = 0.002;
% b4 = 0.001;
% b5 = 0.001;
% b6 = 0.002;
% h = b0 + b1*sin(omega*t) + b2*sin(2*omega*t) + b3*sin(3*omega*t) + b4*sin(4*omega*t) + b5*sin(5*omega*t) + b6*sin(6*omega*t);
% 
% y = lsimplot(G1_deriv,'b',G4_deriv,'r',G5_deriv,'y',h,t);
% setoptions(y, 'Grid', 'on')
% grid on
% xlabel('Time [s]')
% legend("d/dt Y1(t)[m] ~m1 = 208[kg]","d/dt Y1(t)[m] ~m1 = 500[kg]","d/dt Y1(t)[m] ~m1 = 50[kg]" )
% [t1, t2] = title({'\color{black}Fig 8.13 Realistic Road Profile Response with Varying Vehicle Weight with d/dt Y1(t)';'\color{blue}Omega = 1'});

