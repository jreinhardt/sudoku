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

/* heavily commented backtracking solver */

int fld[81];
int unkwn[81];
int numunkwn;

int en(){
	int i,j,c=0,s[27][10];
	/*initialize counter*/
	for(j=1;j<10;j++) for(i=0;i<27;i++) s[i][j]=0;
	/*count number of occurances of every symbol in every row/column/block*/
	for(j=1;j<10;j++) for(i=0;i<81;i++) {
		/*column*/
		s[i%9][j]+=fld[i]==j?1:0;
		/*row*/
		s[i/9+9][j]+=fld[i]==j?1:0;
		/*block*/
		s[3*(i/27)+(i/3)%3+18][j]+=fld[i]==j?1:0;
	}
	/*calculate the score*/
	for(j=1;j<10;j++) for(i=0;i<27;i++) c+=s[i][j]>1?1:0;
	
	return c;
}

int solve(int idx){
	int i;
	printf("level: %d unsolved: %d\n",idx,en());	
	/*not the correct solution*/
	if(en() > 0 || idx > numunkwn) return 1;
	/*correct solution*/
	if(idx == numunkwn && en() == 0) return 0;

	for(i=1;i < 10;i++){
		/*reduce problem*/
		fld[unkwn[idx]] = i;
		/*recurse*/
		if(solve(idx + 1) == 0)	return 0;
	}
	/*no valid solution found, restore state*/
	fld[unkwn[idx]] = 0;

	return 1;
}

int main(int argc, char** argv){
	int i;
	/* read in sudoku*/
	for(i=0;i<81;i+=9)
		scanf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",&fld[i],
		&fld[i + 1],&fld[i + 2],&fld[i + 3],&fld[i + 4],
		&fld[i + 5],&fld[i + 6],&fld[i + 7],&fld[i + 8]);

	/*find the unknown fields*/
	numunkwn=0;
	for(i=0;i<81;i++){
		unkwn[numunkwn] = i;
		numunkwn += fld[i] == 0 ? 1 : 0;
	}
	/*solve the puzzle*/
	solve(0);
	/*print the solution*/
	for(i=0;i<81;i+=9)
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",fld[i],
		fld[i + 1],fld[i + 2],fld[i + 3],fld[i + 4],
		fld[i + 5],fld[i + 6],fld[i + 7],fld[i + 8]);
	return 0;
}
