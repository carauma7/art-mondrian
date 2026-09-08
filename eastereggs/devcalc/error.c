/***********************************************************************
 * 
 * error.c | "Reportagem de erros e funções de depuração."
 * 
 * Copyright 2015 J. G. Silva (django) <dparicarana@Gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 ***********************************************************************
 *  
 * UNIVERSIDADE FEDERAL DE RORAIMA
 * CENTRO DE CIÊNCIAS E TECNOLOGIA - CCT
 * DEPARTAMENTO DE CIÊNCIAS DA COMPUTAÇÃO - DCC
 * PROGRAMAÇÃO ESTRUTURADA
 * 
 * LISTA DE EXERCÍCIOS I
 * 
 * DOSCENTE:	Dion Ribeiro
 * DISCENTE: 	Janderson Gomes da Silva
 * MATRÍCULA: 	2201514716
 * DATA: 		10/12/2015
 * 
 **********************************************************************/

#include <stdio.h>
#include "error.h"

void debug_list(list *ls) {
    while (ls != NULL) {
        printf("%s\n", ls->entry);
        ls = ls->next;
    }
}

void debug_term(term *tm) {
    printf("--- term segm ---\n");
    debug_list(tm->segm);
}

void debug_comp(comp *cp) {
    printf("--- comp elem ---\n");
    debug_list(cp->elem);
}

void debug_block(block *bl) {
    printf("--- block mult ---\n");
    debug_list(bl->mult);
    printf("--- block divi ---\n");
    debug_list(bl->divi);
}
