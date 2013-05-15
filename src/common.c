#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include "common.h"

//private (from common.h)

void set_xy(oku_sod* sod,int x, int y, int val){
	if(x < sod->size && y < sod->size && val < sod->size+1){
		sod->field[x + sod->stride*y] = val;
	} else printf("Error! Invalid Input in set_xy. x=%d y=%d val=%d\n",x,y,val);
}

void set_idx(oku_sod* sod, int idx, int val){
	int x,y;
	y = idx / sod->size;
	x = idx % sod->size;
	set_xy(sod,x,y,val);
}

int get_xy(oku_sod* sod, int x, int y){
	if(x < sod->size && y < sod->size){
		return sod->field[x + sod->stride*y];
	} else printf("Error! Invalid input in get_xy. x=%d y=%d\n",x,y);

	return 0;
}

int get_idx(oku_sod* sod, int idx){
	int x,y;
	y = idx / sod->size;
	x = idx % sod->size;
	return get_xy(sod,x,y);
}

//gets the posth element of the rowth row
int get_row(oku_sod* sod, int row, int pos){
	return get_xy(sod,pos,row);
}

void set_row(oku_sod* sod,int row, int pos, int val){
	set_xy(sod,pos,row,val);
}

//gets the posth element of the colth column
int get_col(oku_sod* sod, int col, int pos){
	return get_xy(sod,col,pos);
}

void set_col(oku_sod* sod, int col, int pos, int val){
	set_xy(sod,col,pos,val);
}

//gets the posth element of the blkth block
int get_blk(oku_sod* sod, int blk, int pos){
	if(blk < sod->size && pos < sod->size){
		return get_idx(sod,sod->blkidx[blk][pos]);
	} else printf("Error! Invalid Input in get_blk. blk=%d, pos=%d\n",blk,pos);

	return 0;
}

void set_blk(oku_sod* sod, int blk, int pos, int val){
	set_idx(sod,sod->blkidx[blk][pos],val);
}

void unk_find(oku_sod* sod){
	int i,size = sod->size;

	sod->numunk = 0;

	for(i=0;i<size*size;i++)
		if(get_idx(sod,i) == 0)
			sod->unkidx[sod->numunk++] = i;
}

int get_numunk(oku_sod* sod){
	return sod->numunk;
}

int get_unkidx(oku_sod* sod, int idx){
	if(idx > get_numunk(sod)){
		printf("Error! Invalid Input in get_unkidx. idx=%d\n",idx);
		return 0;
	}
	return sod->unkidx[idx];
}

//public (from liboku.h)

//initialise empty sod
void oku_sod_init(oku_sod** psod,int size){
	//TODO memerror checken
	oku_sod* sod;
	int i,j,bs,str;

	//blocksize
	bs = sqrt(size);
	if(bs*bs != size){
		printf("Error! Unsupported blockdimension in oku_sod_init!");
		return;
	}

	sod = (oku_sod*) malloc(sizeof(oku_sod));

	//sudoku dimendion is size*size
	sod->size = size;
	str = sod->stride = size;
	sod->field = (int*) malloc(sizeof(int)*str*size);
	sod->blkidx = (int**) malloc(sizeof(int*)*size);
	for(i=0;i<size;i++)
		sod->blkidx[i] = (int*) malloc(sizeof(int)*size);

	//regular blocks for quadratic sizes
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			sod->blkidx[i][j]=(i/bs*str + i%bs)*bs + (j/bs*str + j%bs);

	sod->unkidx = (int*) malloc(sizeof(int)*size*size);
	*psod = sod;

};

//deepcopy of sod
void oku_sod_copy(oku_sod* dst,oku_sod* src){
	int i,size = src->size;
	if(dst->size != src->size){
		oku_sod_destroy(dst);
		oku_sod_init(&dst,src->size);
	}

	//sudoku dimendion is size*size
	for(i=0;i<size*size;i++)
		set_idx(dst,i,get_idx(src,i));
};

//print sod to stdout
void oku_sod_print(oku_sod* sod){
	int i,size=sod->size;
	for(i=0;i<size*size;i++)
		printf("%4d%s",get_idx(sod,i),i%size == size - 1 ? "\n" : "");
}

//destroy sod
void oku_sod_destroy(oku_sod* sod){
	int i;
	for(i=0;i<sod->size;i++)
		free(sod->blkidx[i]);
	free(sod->blkidx);
	free(sod->unkidx);
	free(sod->field);
	free(sod);
}

//*********************************************************************
//common helper functions
//*********************************************************************

unsigned long int rndi(){
	return rand();
}

float rndf(){
	return ((float) rndi())/((float) RAND_MAX);
}

//calculates a fitness value for mc
int fitness(oku_sod* sod){
	int i,j,size = sod->size, score=0;
	//occurance counters
	int row[size+1][size+1],col[size+1][size+1],blk[size+1][size+1];
	//initialize counters
	for(i=0;i<size+1;i++)
		for(j=0;j<size+1;j++){
			row[i][j] = 0;
			col[i][j] = 0;
			blk[i][j] = 0;
		}
	// count occurances
	for(i=0;i<size;i++)
		for(j=0;j<size;j++){
			row[i][get_row(sod,i,j)]++;
			col[i][get_col(sod,i,j)]++;
			blk[i][get_blk(sod,i,j)]++;
		}
	for(i=0;i<size;i++)
		for(j=1;j<size+1;j++){
			score += row[i][j] == 0 ? 1 : row[i][j] - 1;
			score += col[i][j] == 0 ? 1 : col[i][j] - 1;
			score += blk[i][j] == 0 ? 1 : blk[i][j] - 1;
		 }
	return score;
}			

// returns 0 if solved > 0 otherwise
int unsolved(oku_sod* sod){
	int i,j,size = sod->size, score=0;
	//occurance counters
	int row[size+1][size+1],col[size+1][size+1],blk[size+1][size+1];
	//initialize counters
	for(i=0;i<size+1;i++)
		for(j=1;j<size+1;j++){
			row[i][j] = 0;
			col[i][j] = 0;
			blk[i][j] = 0;
		}
	// count occurances
	for(i=0;i<size;i++)
		for(j=0;j<size;j++){
			row[i][get_row(sod,i,j)]++;
			col[i][get_col(sod,i,j)]++;
			blk[i][get_blk(sod,i,j)]++;
		}
	for(i=0;i<size;i++)
		for(j=1;j<size+1;j++){
			score += row[i][j] > 1 ? 1 : 0;
			score += col[i][j] > 1 ? 1 : 0;
			score += blk[i][j] > 1 ? 1 : 0;
		 }
	return score;
}

