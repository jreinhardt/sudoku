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
//the good ones
void oku_mcsol(oku_sod* sod, double temp);
void oku_mcsol2(oku_sod* sod, double temp);
void oku_backtrack(oku_sod* sod);

//the bad ones
void oku_mcblksol(oku_sod* sod, double temp);
void oku_ineldermeads(oku_sod* sod);

#endif
