function f = Analytical_disp(m, T, L, A, t)
        f = T*exp((-1*L*t)/(2*m))*(cos(A*t)+(L/(2*m*A)*sin(A*t)));
end
