#include "liboku.c"
#include <stdio.h>


#define SUD_SIZE 9

int main(int argc, char** argv){
	//TODO nicer
	int i,j,in[16];

	oku_sod* sod;
	//init sod
	oku_sod_init(&sod,SUD_SIZE);

	/* read in sudoku*/
	if(SUD_SIZE==9)
	for(i=0;i<9;i++){
		scanf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
		&in[0],&in[1],&in[2],&in[3],&in[4],&in[5],&in[6],&in[7],&in[8]);
		for(j=0;j<9;j++){
			set_xy(sod,j,i,in[j]);
		}
	}
	if(SUD_SIZE==16)
	for(i=0;i<16;i++){
		scanf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
		&in[0],&in[1],&in[2],&in[3],&in[4],&in[5],&in[6],&in[7],
		&in[8],&in[9],&in[10],&in[11],&in[12],&in[13],&in[14],&in[15]);
		for(j=0;j<16;j++){
			set_xy(sod,j,i,in[j]);
		}
	}


	oku_mcsol(sod,0.8);
//	oku_mcblksol(sod,0.5);
//	oku_backtrack(sod);
//	oku_ineldermeads(sod);
	oku_sod_print(sod);

	oku_sod_destroy(sod);
}
			
			

	


