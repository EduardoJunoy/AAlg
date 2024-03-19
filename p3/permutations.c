/**
 *
 * Descripcion: Implementation of function that generate permutations
 *
 * File: permutations.c
 * Autor: Carlos Aguirre
 * Version: 1.1
 * Fecha: 21-09-2019
 *
 */

#include "permutations.h"
#include "sorting.h"
#include <stdlib.h>
#include <assert.h>

/**
 * @brief Intercambia dos elementos
 * 
 * @param a puntero al primer elemento
 * @param b puntero al segundo elemento
*/
void swap(int *a, int *b) {
  int aux = *a;
  *a = *b;
  *b = aux;
}

/**
 * @brief genera un número aleatorio entre inf y sup
 * 
 * @param inf el límite inferior
 * @param sup el límite superior
 * 
 * @return el número aleatorio generado
*/
int random_num(int inf, int sup)
{
  int aleatorio;

  assert(sup>=inf);  

  aleatorio = rand() % (sup-inf+1) + inf;
  
  return aleatorio;
}


/**
 * @brief Genera una permutación
 * 
 * @param N tamaño de la permutación
 * 
 * @returns puntero a el array que contiene la permutación o NULL en caso de error
*/
int* generate_perm(int N)
{
  int i;
  int *perm;

  if(N <= 0) return NULL;

  perm = (int*)malloc(sizeof(perm[0]) * N);
  if(perm == NULL) return NULL;

  for(i = 0; i < N; i++) {
    perm[i] = (i+1);
  }

  for(i = 0; i < N; i++) {
    swap(&perm[i], &perm[random_num(i, N-1)]);
  }

  return perm;
}

/**
 * @brief Genera n_perms permutaciones aleatorias de N elementos
 * 
 * @param n_perms número de permutaciones
 * @param N tamaño de las permutaciones
 * 
 * @returns array de punteros a las permutaciones o NULL en caso de error
*/
int** generate_permutations(int n_perms, int N)
{
  int i;
  int **permutations;

  permutations = (int**)malloc(sizeof(int*) * n_perms);
  if(permutations == NULL) return NULL;

  for(i=0; i<n_perms; i++) {
    permutations[i] = generate_perm(N);
  }

  return permutations;
}