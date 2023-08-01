function [T, V] = RK(func, h, T0, V0, t)
%     DT1 = h * V;
%     DV1 = h*(-L*V/m - g*sin(T/r));
%     DT2 = h*(V + DV1/2);
%     DV2 = h*(-L*V/m - g*sin(T/r));

     DT1 = h * V0;
     DV1 = h*func(t, T0, V0);
     DT2 = h*(V0 + DV1/2);
     DV2 = h*func(t+h/2, T0+DT1/2, V0+DV1/2);
     DT3 = h*(V0 + DV2/2);
     DV3 = h*func(t+h/2, T0+DT2/2, V0+DV2/2);
     DT4 = h*(V0 + DV3/2);
     DV4 = h*func(t+h, T0+DT3, V0+DV3);
     
     T = T0 + (1/6)*(DT1+2*DT2+2*DT3+DT4);
     V = V0 + (1/6)*(DV1+2*DV2+2*DV3+DV4);

end
