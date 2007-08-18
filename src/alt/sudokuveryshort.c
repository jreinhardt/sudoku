#include<stdio.h>

int main(int j,char**V){
char*R=V[1],i=0,k=48;
for(;*R>k;*++R||puts(R-i))++i;
for(;++k<58;*R&&main(*R=k,V),*R=1)
	for(j=81;j--;)
		*R*=R[j-i]-k||i/9^j/9&&i%9^j%9&&i/27^j/27|i%9/3^j%9/3;
}