

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "liboku.h"

//*********************************************************************
//sod class
//*********************************************************************

//type for a sudoku object sod
struct oku_sod {
	//these fields can be assumed to be always correctly set
	//sudoku dimendion is size*size
	int size;
	int stride;
	int* field;
	int** blkidx;

	//these fields should be reset before use
	int numunk;
	int* unkidx;
};

//private

static void set_xy(oku_sod* sod,int x, int y, int val){
	if(x < sod->size && y < sod->size && val < sod->size+1){
		sod->field[x + sod->stride*y] = val;
	} else printf("Error! Invalid Input in set_xy. x=%d y=%d val=%d\n",x,y,val);
}

static void set_idx(oku_sod* sod, int idx, int val){
	int x,y;
	y = idx / sod->size;
	x = idx % sod->size;
	set_xy(sod,x,y,val);
}

static int get_xy(oku_sod* sod, int x, int y){
	if(x < sod->size && y < sod->size){
		return sod->field[x + sod->stride*y];
	} else printf("Error! Invalid input in get_xy. x=%d y=%d\n",x,y);

	return 0;
}

static int get_idx(oku_sod* sod, int idx){
	int x,y;
	y = idx / sod->size;
	x = idx % sod->size;
	return get_xy(sod,x,y);
}

//gets the posth element of the rowth row
static int get_row(oku_sod* sod, int row, int pos){
	return get_xy(sod,pos,row);
}

static void set_row(oku_sod* sod,int row, int pos, int val){
	set_xy(sod,pos,row,val);
}

//gets the posth element of the colth column
static int get_col(oku_sod* sod, int col, int pos){
	return get_xy(sod,col,pos);
}

static void set_col(oku_sod* sod, int col, int pos, int val){
	set_xy(sod,col,pos,val);
}

//gets the posth element of the blkth block
static int get_blk(oku_sod* sod, int blk, int pos){
	if(blk < sod->size && pos < sod->size){
		return get_idx(sod,sod->blkidx[blk][pos]);
	} else printf("Error! Invalid Input in get_blk. blk=%d, pos=%d\n",blk,pos);

	return 0;
}

static void set_blk(oku_sod* sod, int blk, int pos, int val){
	set_idx(sod,sod->blkidx[blk][pos],val);
}

static void unk_find(oku_sod* sod){
	int i,size = sod->size;

	sod->numunk = 0;

	for(i=0;i<size*size;i++)
		if(get_idx(sod,i) == 0)
			sod->unkidx[sod->numunk++] = i;
}

static int get_numunk(oku_sod* sod){
	return sod->numunk;
}

static int get_unkidx(oku_sod* sod, int idx){
	if(idx > get_numunk(sod)){
		printf("Error! Invalid Input in get_unkidx. idx=%d\n",idx);
		return 0;
	}
	return sod->unkidx[idx];
}

//public

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
	sod->blkidx = (int**) malloc(sizeof(int)*size);
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
static int fitness(oku_sod* sod){
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
static int unsolved(oku_sod* sod){
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
			score += row[i][j] > 1 ? : 0;
			score += col[i][j] > 1 ? printf("%d %d\n",i,j): 0;
			score += blk[i][j] > 1 ? 1 : 0;
		 }
	printf("%d\n",score);
	return score;
}

//*********************************************************************
//solver
//*********************************************************************
//solvers take a sod with hints and 0 for the unknowns and 
//return a complete sod

//monte carlo solver
void oku_mcsol(oku_sod* sod, double temp){
	int i,j,num,size = sod->size;
	int cnt[size+1];
	int fit1, fit2, idx1, idx2,tmp;

	//refresh list of unknowns in sod
	unk_find(sod);

	//fill sudoku with missing symbols
	for(i=1;i<size+1;i++) cnt[i] = size;

	for(i=0;i<size*size;i++)
		cnt[get_idx(sod,i)]--;

	j=1;
	for(i=0;i<get_numunk(sod);i++){
		while(cnt[j] < 1 && j <= size+1) j++;
		cnt[j]--;
		set_idx(sod,get_unkidx(sod,i),j);
	}

	fit1 = fitness(sod);
	num=0;

	while(fit1){
		//choose 2 indices
		idx1 = get_unkidx(sod,rndi() % get_numunk(sod));
		idx2 = get_unkidx(sod,rndi() % get_numunk(sod));

		//swap
		tmp = get_idx(sod,idx1);
		set_idx(sod,idx1,get_idx(sod,idx2));
		set_idx(sod,idx2,tmp);

		fit2 = fitness(sod);

		if(fit2 > fit1 && exp((fit1 - fit2)/temp) < rndf()){
			//swap back
			tmp = get_idx(sod,idx1);
			set_idx(sod,idx1,get_idx(sod,idx2));
			set_idx(sod,idx2,tmp);
		} else fit1 = fit2;
		num++;
		if(num % 2000 == 0)
			printf("Step:%d Fitness: %d\n",num,fit1);
			
	}
	printf("Solution found after %d MC Steps\n",num);

};

//structured monte carlo
//maintains correct blockstructure doesnt work very good
void oku_mcblksol(oku_sod* sod, double temp){
	int i,j,k,val;
	int fit1, fit2, num,blk,idx1,idx2,tmp;
	int size = sod->size;
	int blkunk[size],cnt[size];
	int blkunkidx[size][size];

	for(i=0;i<size;i++){
		blkunk[i] = 0;
		for(j=0;j<size;j++)
			cnt[j] = 0;

		//find unknowns for every block
		for(j=0;j<size;j++){
			val = get_blk(sod,i,j);
			if(val == 0){
				blkunkidx[i][blkunk[i]++] = j;
			} else {
				if(cnt[val-1] == 0){
					cnt[val-1] = 1;
				} else printf("Fehler in oku_mcblocksol: Invalid hint: %d\n",val);
			}
		}

		//fill them
		k=0;
		for(j=0;j<size;j++)
			if(cnt[j] == 0)
				set_blk(sod,i,blkunkidx[i][k++],j+1);
	}

	oku_sod_print(sod);

	fit1 = fitness(sod);
	num = 0;
	while(fit1){
		//choose block and two indizes inside
		blk = rndi() % size;
		idx1 = rndi() % blkunk[blk];
		idx2 = rndi() % blkunk[blk];

		//swap
		tmp = get_blk(sod,blk,idx1);
		set_blk(sod,blk,idx1,get_blk(sod,blk,idx2));
		set_blk(sod,blk,idx2,tmp);

		fit2 = fitness(sod); 

		if(fit2 > fit1 && exp((fit1 - fit2)/temp) < rndf()){
			//swap back
			tmp = get_blk(sod,blk,idx1);
			set_blk(sod,blk,idx1,get_blk(sod,blk,idx2));
			set_blk(sod,blk,idx2,tmp);
		} else fit1 = fit2;
		num++;
		if(num % 2000 == 0)
			printf("Step:%d Fitness: %d\n",num,fit1);
	}

}
				



//simple backtracking
static int backtrack(oku_sod* sod, int level){
	int i;
	//not a correct solution, abort
	if(unsolved(sod) > 0 || level > get_numunk(sod)) return 1;

	// correct solution
	if(level == get_numunk(sod) && unsolved(sod) == 0) return 0;

	//not a complete solution, try all symbols in this field
	for(i=1;i<sod->size+1;i++){
		set_idx(sod,get_unkidx(sod,level),i);
		if(backtrack(sod,level+1) == 0) return 0;
	}

	// no valid solution found, restore state*/
	set_idx(sod,get_unkidx(sod,level),0);

	return 1;
}

void oku_backtrack(oku_sod* sod){

	//refresh the list of unknown positions in sod
	unk_find(sod);

	if(backtrack(sod,0) == 1)
		printf("Error! No solution found in oku_backtrack\n");

}


//*********************************************************************
//util
//*********************************************************************

int oku_solcount(oku_sod* sod);

//*********************************************************************
//generators
//*********************************************************************

void oku_mcgen(oku_sod* sod, double temp);


