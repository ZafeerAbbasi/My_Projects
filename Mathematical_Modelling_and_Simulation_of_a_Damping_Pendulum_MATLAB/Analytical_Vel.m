function f = Analytical_Vel(m, T, L, A, t)
    f = (-T*((4*(A^2)*(m^2)+(L^2))*exp(-L*t/2*m)*sin(t*A))/(4*(m^2)*A));
end

    