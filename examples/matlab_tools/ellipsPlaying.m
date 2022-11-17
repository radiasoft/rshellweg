a=15.0;
b=5.0;
t0=1.5;
s=25.0;

N=101;
x=zeros(2*N,3);
y=zeros(2*N,3);

for i=1:N
   x(i,1)=a*(-1+2/(N-1)*(i-1));
   x(N+i,1)=-a*(-1+2/(N-1)*(i-1));
   y(i,1)=b*sqrt(1-(x(i,1)/a)^2);
   y(N+i,1)=-b*sqrt(1-(x(N+i,1)/a)^2);
   x(i,2)=sqrt(a^2+s)*(-1+2/(N-1)*(i-1));
   x(N+i,2)=-sqrt(a^2+s)*(-1+2/(N-1)*(i-1));
   y(i,2)=sqrt((b^2+s)*(1-x(i,2)^2/(a^2+s)));
   y(N+i,2)=-sqrt((b^2+s)*(1-x(N+i,2)^2/(a^2+s)));
   x(i,3)=sqrt(a^2*t0)*(-1+2/(N-1)*(i-1));
   x(N+i,3)=-sqrt(a^2*t0)*(-1+2/(N-1)*(i-1));
   y(i,3)=sqrt((b^2*t0)*(1-x(i,3)^2/(a^2*t0)));
   y(N+i,3)=-sqrt((b^2*t0)*(1-x(N+i,3)^2/(a^2*t0)));
end

figure (10)
plot (x(:,1),y(:,1),'r',x(:,2),y(:,2),'b',x(:,3),y(:,3),'g','LineWidth',2)
xlabel('x','FontSize',16,'Color','m')
ylabel('y','FontSize',16,'Color','m')
grid on


pi=3.14159265358;
qEl_CGSE=4.8032045e-10;        % CGSE
qEl_C=1.602176565e-19;         % C
mEl_g=9.1098291e-28;           % g
mEl_MeV=0.510998928;           % MeV
cLight=2.99792458e10;          % cm/s
MeV2erg=1.602176565e-6;
A2SGSE=qEl_CGSE/qEl_C;

Ekin=0.03;                % MeV
Bfield=500.0;             % Gs

gamma=1+Ekin/mEl_MeV;
beta=sqrt(1-1/gamma^2);
vel=beta*cLight;          % cm/s
p=MeV2erg/cLight*sqrt(Ekin*(Ekin+2*mEl_MeV));   % g*cm/s

ro_Larm=p*cLight/(q_e*Bfield);                   % cm

k_Br=1/sqrt(2)/ro_Larm;                          % 1/cm
lambda_Br=2*pi/k_Br;

current=0.1;                                     % A
J=qEl_CGSE*A2SGSE*current/(beta^2*gamma^3*mEl_g*cLight^3);


a0=5.0;                                          % cm
ro_Br=a0/2;                                      % cm
B_Br=p*cLight/(q_e*ro_Br);                       % Gs

k_kuts=q_e*Bfield/(2*mEl_g*cLight*beta*gamma)



