
#ifndef LIBOKU_H
#define LIBOKU_H
//type for a sudoku object sod
typedef struct oku_sod oku_sod;

//initialise empty sod
void oku_sod_init(oku_sod** sod,int size);

//deepcopy of sod
void oku_sod_copy(oku_sod* dst,oku_sod* src);
//print sod to stdout
void oku_sod_print(oku_sod* sod);

//destroy sod
void oku_sod_destroy(oku_sod* sod);

//solver

void oku_mcsol(oku_sod* sod, double temp);
void oku_mcsol2(oku_sod* sod, double temp);
void oku_backtrack(oku_sod* sod);

//util

int oku_solcount(oku_sod* sod);

//generators

void oku_mcgen(oku_sod* sod, double temp);

#endif
