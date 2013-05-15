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
#include "liboku.h"
#include <stdio.h>


#define SUD_SIZE 9

int main(int argc, char** argv){
	//TODO nicer
	int i,j,in[16];

	oku_sod* sod,*sod2;
	//init sod
	oku_sod_init(&sod,SUD_SIZE);
	oku_sod_init(&sod2,SUD_SIZE);

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

	oku_sod_copy(sod2,sod);


	oku_mcsol(sod,0.7);
	oku_mcsol2(sod2,0.7);
//	oku_mcblksol(sod,0.5);
//	oku_backtrack(sod);
//	oku_ineldermeads(sod);
	oku_sod_print(sod);

	oku_sod_destroy(sod);
}
			
			

	


