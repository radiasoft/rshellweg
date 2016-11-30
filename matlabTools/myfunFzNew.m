function FzNew = myfunFzNew(x)
    Rb=0.00287883;
    Lb=0.493179;
    r=0.0013119;
    z=0.428186;

    d=(Rb*Rb*Lb)^(1/3);
    d2=d*d;

    Rb2=2*Rb*Rb/d2;
    Lb2=2*Lb*Lb/d2;


    r2=r*r;
    z2=z*z;

    Fr=1./((1-Rb2)*x+Rb2);
    Fz=1./((1-Lb2)*x+Lb2);
    argExp=(1-x).*(r2*Fr+z2*Fz)/d2;
    FzNew=z*sqrt((1-x).*Fz).*exp(-argExp).*Fr.*Fz;
end
