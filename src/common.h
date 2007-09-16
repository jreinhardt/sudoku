#include "liboku.h"
#ifndef COMMON_H
#define COMMON_H
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

//helper functions

void set_xy(oku_sod* sod,int x, int y, int val);
//void set_idx(oku_sod* sod, int idx, int val);

int get_xy(oku_sod* sod, int x, int y);
//int get_idx(oku_sod* sod, int idx);

//gets the posth element of the rowth row
int get_row(oku_sod* sod, int row, int pos);
void set_row(oku_sod* sod,int row, int pos, int val);

//gets the posth element of the colth column
int get_col(oku_sod* sod, int col, int pos);
void set_col(oku_sod* sod, int col, int pos, int val);

//gets the posth element of the blkth block
int get_blk(oku_sod* sod, int blk, int pos);
void set_blk(oku_sod* sod, int blk, int pos, int val);

//updates internal unknown state
void unk_find(oku_sod* sod);
//returns number of unknowns
int get_numunk(oku_sod* sod);
//returns the index of the idxth unknown cell
int get_unkidx(oku_sod* sod, int idx);


// more helper
unsigned long int rndi();
float rndf();
int fitness(oku_sod* sod);
int unsolved(oku_sod* sod);


#endif

