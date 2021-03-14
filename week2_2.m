%12181769 ����� 2���� �ǽ� 2��

N=1000;%1000���� ����
avg=5;%����� 5
stdev=3^(1/2);%�л��� 3

R=50;

T=randn(1,N)*stdev+avg;%����þ� ���� ����

%��հ� �л�
E=mean(T);
V=var(T);

%pdf, cdf
[M,X]=hist(T,R);

resol=X(2)-X(1);
PDF=M/N/resol;

CDF=cumsum(PDF*resol);

%����þ� ����
x=0:10;
y=1/(stdev*(2*pi).^(1/2))*exp(1).^(-(1/2)*((x-avg)/stdev).^2);

figure; hold on;
bar(X,PDF);
plot(X,CDF);
plot(x,y);
