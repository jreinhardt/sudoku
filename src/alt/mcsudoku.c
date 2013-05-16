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

/* Sudoku Solver using the Metropolis Algorithm */

#include <stdio.h>
#include <limits.h>
#include <math.h>

/*nasty global vars*/
int tbl[81];
int swap_idxs[81];
unsigned long int seed;

/*LUTs for easy block access*/
static const int bl_start[9]  = {0, 3, 6, 27, 30, 33, 54, 57, 60};
static const int bl_offset[9] = {0, 1, 2, 9,  10, 11, 18, 19, 20};

/*generate random integer number with a shift register PRNG*/
unsigned long int rndi(){
	seed = seed ^ (seed >> 15);
	return seed = seed ^ (seed << 17);
}

float rndf(){
	return ((float) rndi())/((float) ULONG_MAX);
}

void swap(int idx1,int idx2){
	int tmp = tbl[idx1];
	tbl[idx1] = tbl[idx2];
	tbl[idx2] = tmp;
}

int energy(void){
	int i,j;
	int en = 0;
	int row_count[10];
	int col_count[10];
	int bl_count[10];

	for(i=0;i<9;i++){
		for(j=0;j<10;j++) row_count[j] = col_count[j] = bl_count[j] = 0;
		/*count number of symbol in row/col/block i*/
		for(j=0;j<9;j++){
			row_count[tbl[9*i + j]]++;
			col_count[tbl[i + 9*j]]++;
			bl_count[tbl[bl_start[i] + bl_offset[j]]]++;
		}
		/*calculate energy as number of symbols that occure not 
		exactly one time in col/row/block*/
		for(j=1;j<10;j++){
			en += (row_count[j] == 0) ? 1 : row_count[j] - 1;
			en += (col_count[j] == 0) ? 1 : col_count[j] - 1;
			en += (bl_count[j] == 0)  ? 1 :  bl_count[j] - 1;
		}
	}
	return en;
}

int main(void){
	/*Parameters*/
	float temp = 0.6;
	seed = 2147483647;

	int i,j,k,c;
	int count[10];

	/*read in hints, > 0 for fixed symbols, 0 for unknown symbols*/
	for(i=0;i<81;i+=9)
		scanf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",&tbl[i],
		&tbl[i + 1],&tbl[i + 2],&tbl[i + 3],&tbl[i + 4],
		&tbl[i + 5],&tbl[i + 6],&tbl[i + 7],&tbl[i + 8]);

	/*fill in the missing symbols*/
	for(i=0;i<10;i++) count[i] = 9;
	for(i=0;i<81;i++) count[tbl[i]]-- ;	

	i = 1;
	k = 0;

	for(j=0;j<81;j++){
		if(tbl[j] == 0){
			swap_idxs[k++] = j;
			while(count[i] == 0 && i <10) i++;
			tbl[j] = i;
			count[i]--;
		}
	}

	c = 0;

	int en1 = energy();

	/*do MC steps until sudoku is solved*/
	while(en1){
		/*choose 2 indices to swap*/
		int idx1 = swap_idxs[(rndi() % k)];
		int idx2 = swap_idxs[(rndi() % k)];

		swap(idx1,idx2);

		int en2 = energy();

		/*reject*/
		if(en2>en1 && exp((en1 - en2)/temp) < rndf()){
			swap(idx1,idx2);
		} else {
			en1 = en2;
		}
		c++;
		printf("c:%d en:%d\n",c,en1);
	}

	printf("Solution found after %d Steps:\n",c);
	for(i=0;i<81;i+=9)
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",tbl[i],
		tbl[i + 1],tbl[i + 2],tbl[i + 3],tbl[i + 4],
		tbl[i + 5],tbl[i + 6],tbl[i + 7],tbl[i + 8]);
}
