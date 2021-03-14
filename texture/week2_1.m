%12181769 ³²Èñ¿ø 2ÁÖÂ÷ ½Ç½À 1¹ø

N=10000;
R=10;

U=rand(1,N)*(3-2)+2;

[M,X]=hist(U,R);

resol=X(2)-X(1);
PDF=M/N/resol;
CDF=cumsum(PDF*resol);

E=mean(U,'all');

figure; hold on;
bar(X,PDF);
plot(X,CDF);
