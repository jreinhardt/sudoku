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

#define BL_SIZE 3

#define SU_SIZE BL_SIZE*BL_SIZE

int bidx[SU_SIZE];
int boff[SU_SIZE];

int field[SU_SIZE][SU_SIZE][SU_SIZE];
int sol[SU_SIZE][SU_SIZE];


/*check whether solved or not*/
int unsolved(void){
	int i,j,k;
	int uamb;

	for(i=0;i<SU_SIZE;i++)
		for(j=0;j<SU_SIZE;j++){
			uamb = 1;
			for(k=0;k<SU_SIZE;k++){
				if(field[i][j][k] && uamb){
					uamb = 0;
					sol[i][j] = k + 1;
				} else if(field[i][j][k] && uamb == 0)
					//sol[i][j] = 0;
					return 0;
				}
			}

	return 0;
}

/*eliminate obvious possibilities*/
void set(int x, int y, int val){
	int i;
	if(val != 0){
		for(i=0;i<SU_SIZE;i++){
			field[y][i][val - 1] = 0;
			field[i][x][val - 1] = 0;
			field[y][x][i]   = 0;
			
		}
		field[y][x][val - 1] = 1;
	}
}

int main(void){
int i,j,k;
int hint;

/* FIXME Generate bidx*/
for(i=0;i<BL_SIZE;i++)
	for(j=0;j<BL_SIZE;j++){
		boff[BL_SIZE*i+j] = j + SU_SIZE*i;
		bidx[BL_SIZE*i+j] = BL_SIZE*(j + SU_SIZE*i);
	}


/*initialize field - everything is possible*/
for(i=0;i<SU_SIZE;i++)
	for(j=0;j<SU_SIZE;j++)
		for(k=0;k<SU_SIZE;k++)
			field[i][j][k] = 1;

/*initialize sol*/
for(i=0;i<SU_SIZE;i++)
	for(j=0;j<SU_SIZE;j++)
		sol[i][j] = 0;

/*read in hints row wise, one at once*/
for(i=0;i<SU_SIZE;i++){
	for(j=0;j<SU_SIZE;j++){
		scanf("%d\n",&hint);
		printf("%d ",hint);
		set(j,i,hint);
	}
	printf("\n");
}

/*solve*/
while(unsolved()){

	/*check for obvious symbols*/
	int pos;
	for(i=0;i<SU_SIZE;i++)
		for(j=0;j<SU_SIZE;j++){
			pos = -1;
			for(k=0;k<SU_SIZE;k++){
				if(field[i][j][k] == 1 && pos == -1){
					pos = k;
				} else if(pos != -1){
					pos = -2;
				}
			}
			if(pos >= 0)
				set(j,i,pos);
			}


}


/*print out solution*/
for(i=0;i<SU_SIZE;i++){
	for(j=0;j<SU_SIZE;j++)
		printf("%d ",sol[i][j]);
	printf("\n");
}

}
