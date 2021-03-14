%12181769 남희원 2주차 실습 2번

N=1000;%1000개의 난수
avg=5;%평균이 5
stdev=3^(1/2);%분산이 3

R=50;

T=randn(1,N)*stdev+avg;%가우시안 난수 생성

%평균과 분산
E=mean(T);
V=var(T);

%pdf, cdf
[M,X]=hist(T,R);

resol=X(2)-X(1);
PDF=M/N/resol;

CDF=cumsum(PDF*resol);

%가우시안 분포
x=0:10;
y=1/(stdev*(2*pi).^(1/2))*exp(1).^(-(1/2)*((x-avg)/stdev).^2);

figure; hold on;
bar(X,PDF);
plot(X,CDF);
plot(x,y);
