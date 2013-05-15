/*
 * Copyright (C) 2007-2013 Johannes Reinhardt <jreinhardt@ist-dein-freund.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdint.h>

int freiheit(uint16_t w){
	int sum=0,i;
	for(i=0;i<10;i++) sum += (w>>i) & 0x0001;
	return sum;
}

int mogl(uint16_t* f){
	int i,sum = 0;
	for(i=0;i<81;i++) sum += freiheit(f[i]);
	return sum;
}

void setze(uint16_t* f, int r, int s, uint16_t sw){
	int i;
	for(i=0;i<9;i++){
		f[i*9+s] &= (~sw);
		f[9*r+i] &= (~sw);
		f[9*((r/3)*3+(i%3))+((s/3)*3+(i/3))] &= (~sw);
	}
	f[9*r+s] = sw;
}

void set(uint16_t* f, int r, int s, uint16_t sw){
	int i,mog = 0;
	setze(f,r,s,sw);
	while(mog != mogl(f)){
		mog = mogl(f);
		for(i=0;i<81;i++) if(freiheit(f[i]) == 1) setze(f,i/9,i%9,f[i]);
	}
}

int flag2int(uint16_t f){
	int i;
	for(i=1;((f>>=1)&1)==0;i++);
	return i;
}

int wider(uint16_t* f){
	int i;
	for(i=0;i<81;i++) if(f[i] & 0x03fe == 0) return 0;
	return 1;
}

int rekursiv(uint16_t* f){
	uint16_t i=0,j,k,ret,bak[81];
	if (wider(f) == 0) return 0;
	if(mogl(f) == 81) return 1;
	for(j=0;j<81;j++) bak[j] = f[j];
	while(freiheit(f[i]) == 1) i++;
	for(j=1;j<10;j++){
		 if((f[i]>> j) & 1 == 1){
			set(f,i/9,i%9,(1<<j));
			ret = rekursiv(f);
			if(ret == 0) for(k=0;k<81;k++) f[k] = bak[k];
			if(ret == 1) return 1;
		}
	}
	return 0;
}

int main(void){
	uint16_t i,j,inp[9],feld[81];
	for(i=0;i<81;i++) feld[i] = 0x03fe;
	for(i=0;i<9;i++){
		scanf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",&inp[0],&inp[1],&inp[2],&inp[3],&inp[4],&inp[5],&inp[6],&inp[7],&inp[8]);
		for(j=0;j<9;j++) if(inp[j] != 0) set(feld,i,j,(1<<inp[j]));
	}
	rekursiv(feld);
	printf("\nLösung:\n");
	for(i=0;i<81;i++){
		if(freiheit(feld[i]) == 1) printf("%d\t",flag2int(feld[i]));
		if(freiheit(feld[i]) != 1) printf("*%d\t",freiheit(feld[i]));
		if(i%9 == 8) printf("\n");
	}
	return 0;
}
