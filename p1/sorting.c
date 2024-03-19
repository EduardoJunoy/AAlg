/**
 *
 * Descripcion: Implementation of sorting functions
 *
 * Fichero: sorting.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */


#include "sorting.h"
#include "permutations.h"
#include <stdlib.h>

int min(int* array, int ip, int iu);  
int SelectSort(int* array, int ip, int iu);

/**
 * @brief Ordena de menor a mayor un array
 * 
 * @param array el array a ordenar
 * @param ip primer índice del array
 * @param iu último índice del array
 * 
 * @return el número de operaciones básicas (comparaciones de clave) que ha realizado el algortimo
*/
int SelectSort(int* array, int ip, int iu)
{
  int i, count, minimum;

  if(array == NULL || ip > iu) return ERR;

  count = 0;
  i = ip;
  while(i < iu) {
    minimum = min(array, i, iu);
    count += iu-i;
    swap(&array[i], &array[minimum]);
    i++;
  }

  return count;
}

/**
 * @brief Ordena de mayor a menor un array
 * 
 * @param array el array a ordenar
 * @param ip primer índice del array
 * @param iu último índice del array
 * 
 * @return el número de operaciones básicas (comparaciones de clave) que ha realizado el algortimo
*/
int SelectSortInv(int* array, int ip, int iu)
{
  int i, count, minimum;

  if(array == NULL || ip > iu) return ERR;

  for(i=0; i<iu; i++);

  count = 0;
  while(i >= 0) {
    minimum = min(array, ip, i);
    count += i-ip;
    swap(&array[i], &array[minimum]);
    i--;
  }

  return count;
}

/**
 * @brief Localiza el menor elemento de un array
 * 
 * @param array el array en el que busca
 * @param ip primer índice del array
 * @param iu último índice del array
 * 
 * @return el índice del menor elemento del array
*/
int min(int* array, int ip, int iu)
{
  int i;
  int min;
  
  min = ip;
  for(i = ip+1; i<=iu; i++) {
    if(array[i] < array[min]) {
      min = i;
    }
  }

  return min;
}

