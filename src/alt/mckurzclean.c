#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define Z for(i=0;i<81;i++)
#define W for(i=0;i<27;i++)
#define X for(j=1;j<10;j++)
#define Y j=f[i2];f[i2]=f[i1];f[i1]=j;
#define V ][j]+=f[i]==j?1:0;
int i,j,f[82],c[10],m[81],s[27][10],i1,i2,e1,e2;int en(){X W s[i][j]=0;X Z{
s[i%9 V s[i/9+9 V s[3*(i/27)+(i/3)%3+18 V} c[0]=0;X W c[0]+=s[i][j]==0?1
:s[i][j]-1;return c[0];}int main(){X c[i]=0;c[0]=0;Z scanf("%d ",&f[i]);Z c[f[i
]]++;m[80]=j=0;Z m[f[i]==0?j++:80]=i;c[0]=0;X c[j]=9-c[j]+c[j-1];X for(i=0;i<c[
10-j];i++)f[m[i]]=10-j;e1=en();while(e1!=0){i1=rand()%m[80];i2=rand()%m[80];Y
e2=en();if(e2>e1&&exp((e1-e2)*2)<(rand()/(float)RAND_MAX)){Y}else{e1=e2;}}Z
printf("%d%s",f[i],(i+1)%9?" ":"\n");return 0;}
