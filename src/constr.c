
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include "common.h"

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

