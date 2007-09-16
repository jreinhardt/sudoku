
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include "common.h"

// first monte carlo solver, mildly constraint
void oku_mcsol(oku_sod* sod, double temp){
	int i,j,num,size = sod->size;
	int cnt[size+1];
	int fit1, fit2, idx1, idx2,tmp,tmp2;

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

	while(fit1 && num < 200000){
		//choose 2 indices
		idx1 = get_unkidx(sod,rndi() % get_numunk(sod));
		idx2 = get_unkidx(sod,rndi() % get_numunk(sod));

		//swap
		tmp = get_idx(sod,idx1);
		if(tmp == (tmp2 = get_idx(sod,idx2))) continue;
		set_idx(sod,idx1,tmp2);
		set_idx(sod,idx2,tmp);

		fit2 = fitness(sod);

		if(fit2 > fit1 && exp((fit1 - fit2)/temp) < rndf()){
			//swap back
			tmp = get_idx(sod,idx1);
			set_idx(sod,idx1,get_idx(sod,idx2));
			set_idx(sod,idx2,tmp);
		} else fit1 = fit2;
		num++;
			
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
				
// completely unconstrained monte carlo
void oku_mcsol2(oku_sod* sod, double temp){
	int i,j,num,size = sod->size;
	int cnt[size+1];
	int fit1, fit2, idx1, idx2,tmp;

	//refresh list of unknowns in sod
	unk_find(sod);

	//fill sudoku with missing symbols
	for(i=0;i<get_numunk(sod);i++){
		set_idx(sod,get_unkidx(sod,i),(rndi() % sod->size) + 1);
	}

	fit1 = fitness(sod);
	num=0;

	while(fit1 && num < 200000){
		//choose 2 indices
		idx1 = get_unkidx(sod,rndi() % get_numunk(sod));

		tmp = get_idx(sod,idx1);

		set_idx(sod,idx1,(rndi() % sod->size) + 1);

		fit2 = fitness(sod);

		if(fit2 > fit1 && exp((fit1 - fit2)/temp) < rndf()){
			//swap back
			set_idx(sod,idx1,tmp);
		} else fit1 = fit2;
		num++;
			
	}
	printf("Solution found after %d MC Steps\n",num);

};

//Nelder-Meads
void oku_ineldermeads(oku_sod* sod){
	int i,j,dim,idx,tmpfit;
	int* fit, *barc;
	oku_sod** polytope;
	oku_sod* tmp, *tmp2;
	int max,maxidx,max2,max2idx,min,minidx;
	double alpha,beta,gamma;

	alpha = 1.0;
	beta = 2.0;
	gamma = 0.5;

	unk_find(sod);

	dim = get_numunk(sod) + 1;

	fit = (int*) malloc(dim*sizeof(int));
	barc =(int*) malloc(dim*sizeof(int));
	polytope = (oku_sod**) malloc(dim*sizeof(oku_sod*));

	oku_sod_init(&tmp,sod->size);
	oku_sod_copy(tmp,sod);
	oku_sod_init(&tmp2,sod->size);
	oku_sod_copy(tmp2,sod);

	//init polytope
	for(i=0;i<dim;i++){
		oku_sod_init(&polytope[i],sod->size);
		oku_sod_copy(polytope[i],sod);
		for(j=0;j<dim-1;j++)
			set_idx(polytope[i],get_unkidx(sod,j),rndi() % sod->size);
		fit[i] = fitness(polytope[i]);
		printf("Fitness of vertex %d: %d\n",i,fit[i]);
	}

	do{
		for(j=0;j<dim-1;j++)
			barc[j] = 0;
		max = minidx = maxidx = 0;
		min = INT_MAX;

		for(i=0;i<dim;i++){
			fit[i] = fitness(polytope[i]);
			min = fit[i] < min ? fit[minidx = i] : min;
			if(fit[i] > max){
				max2 = max;
				max2idx = maxidx;
				max = fit[i];
				maxidx = i;
			} else {
				max = fit[i] > max2 ? fit[max2idx = i] : max2;
			}

			//calculate barycenter
			for(j=0;j<dim-1;j++)
				barc[j] += get_idx(polytope[i],get_unkidx(sod,j));
		}

		printf("Min: %d Max: %d ",min,max);
		//we work with fixed point arithmetic with accuracy dim
		//reflection
		for(j=0;j<dim-1;j++){
			idx = get_unkidx(sod,j);
			set_idx(tmp,idx,(barc[j] + alpha*(barc[j] - dim*get_idx(polytope[maxidx],idx)))/dim);
		}

		tmpfit = fitness(tmp);

		//accept reflection
		if((fit[minidx] <= tmpfit) && (tmpfit < fit[max2idx]))
			oku_sod_copy(polytope[maxidx],tmp);
			printf("Reflection\n");
			continue;

		//expand
		if(tmpfit < fit[minidx]){
			for(j=0;j<dim-1;j++){
				idx = get_unkidx(sod,j);
				set_idx(tmp2,idx,(barc[j] + beta*(dim*get_idx(tmp,idx) - barc[j]))/dim);
			 }

			if(fitness(tmp2) < tmpfit){
				oku_sod_copy(polytope[maxidx],tmp2);
			} else {
				oku_sod_copy(polytope[maxidx],tmp);
			}
			printf("Expansion\n");
			continue;
		}

		//contraction
		if(tmpfit > fit[max2idx]){
			if(tmpfit > fit[maxidx]){
				for(j=0;j<dim-1;j++){
					idx = get_unkidx(sod,j);
					set_idx(tmp2,idx,(barc[j] + gamma*(dim*get_idx(polytope[maxidx],idx) - barc[j]))/dim);
				}

				if(fitness(tmp2) < fit[maxidx]){
					oku_sod_copy(polytope[maxidx],tmp2);
					printf("Contraction1\n");
					continue;
			} else {
				for(j=0;j<dim-1;j++){
					idx = get_unkidx(sod,j);
					set_idx(tmp2,idx,(barc[j] + gamma*(dim*get_idx(tmp,idx) - barc[j]))/dim);
				}

				if(fitness(tmp2) < tmpfit){
					oku_sod_copy(polytope[maxidx],tmp2);
					printf("Contraction2\n");
					continue;
				}
			}
			
			//shrink polytope
			for(i=0;i<dim;i++)
				for(j=0;j<dim-1;j++){
					idx = get_unkidx(sod,j);
					set_idx(polytope[i],idx,(get_idx(polytope[i],idx) + dim*barc[j])/2/dim);
				}
			printf("Shrinking\n");
			}
		}
	}while(min>0);

	//destroy
	for(i=0;i<dim;i++)
		oku_sod_destroy(polytope[i]);

	oku_sod_destroy(tmp);
	oku_sod_destroy(tmp2);
	
	free(polytope);
	free(fit);

}

