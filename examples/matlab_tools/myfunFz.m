function Fz = myfunFz(x)
    Rb=0.00287883;
    Lb=0.493179;
    r=0.0013119;
    z=0.428186;

    Rb2=Rb*Rb;
    Lb2=Lb*Lb;

   d=(Rb2*Lb)^(1/3);
   d2=d*d;

    s=d2*(1./x-1);
    t=r.*r./(Rb2+s)+z.*z./(Lb2+s);
    if (t <= 5) 
	    func=sqrt(x);
		pX=1./sqrt((Rb2/d2-1)*x+1);
	    pY=pX;
		pZ=1./sqrt((Lb2/d2-1)*x+1);
		func=pX.*pY.*pZ.*func;
		Fz=z.*pZ.*pZ.*func;
    end

end
