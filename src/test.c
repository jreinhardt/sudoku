#include "liboku.c"
#include <stdio.h>

int main(int argc, char** argv){
	//TODO nicer
	int i,j,in[9];

	oku_sod* sod;
	//init sod
	oku_sod_init(&sod,9);

	/* read in sudoku*/
	for(i=0;i<81;i+=9){
		scanf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",&in[0],
		&in[1],&in[2],&in[3],&in[4],
		&in[5],&in[6],&in[7],&in[8]);
		for(j=0;j<9;j++){
			set_idx(sod,i+j,in[j]);
		}
	}

//	oku_mcsol(sod,0.6);
	oku_backtrack(sod);
	oku_sod_print(sod);

	oku_sod_destroy(sod);
}
			
			

	


