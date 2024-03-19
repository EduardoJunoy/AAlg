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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sorting.h"
#include "permutations.h"

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

/**
 * @brief Ordena de menor a mayor una tabla
 * 
 * @param tabla la tabla a ordenar
 * @param ip primer elemento de la tabla
 * @param iu último elemento de la tabla
 * 
 * @return el número de operaciones básicas (comparaciones de clave) que ha realizado el algortimo
*/
int mergesort(int* tabla, int ip, int iu) {
  int medio, n_obs, st;

  if(ip > iu) return ERR;

  /* Caso Base */
  if(ip == iu) {
    return OK;
  }

  n_obs = 0;
  medio = (ip+iu)/2;
  st = mergesort(tabla, ip, medio);
  if (st == ERR) return ERR;
  n_obs += st;
  st = mergesort(tabla, medio+1, iu);
  if(st==ERR) return ERR;
  n_obs += st;
  
  n_obs += merge(tabla, ip, iu, medio);
  return n_obs;
}

/**
 * @brief Combina las dos subtablas en una tabla auxiliar y esta la copia en la tabla original
 * 
 * @param tabla la tabla a ordenar
 * @param ip primer elemento de la tabla
 * @param iu último elemento de la tabla
 * @param imedio valor medio del número de elementos
 * 
 * @return la tabla combinada
*/
int merge(int* tabla, int ip, int iu, int imedio) {
  int i, j, k, n_obs;
  int *taux;

  taux = (int*)malloc(sizeof(int) * (iu+1-ip));
  if(taux == NULL) return ERR;
  n_obs = 0;

  for(i = ip, j = imedio+1, k = 0; (i<=imedio) && (j<=iu); k++){
    n_obs++;
    if (tabla[i] < tabla[j]){
      taux[k] = tabla[i];
      i++;
    }
    else {
      taux[k] = tabla[j];
      j++;
    }
  }

  if(i > imedio) {
    while (j <= iu) {
    taux[k] = tabla[j];
    j++;
    k++;
    }
  }
  else if (j > iu){
    while (i <= imedio)
    {
      taux[k]=tabla[i];
      i++;
      k++;
    }
  }
  
  
  /* Copiar */
  for(i=ip, j=0; i<=iu; i++, j++) {
    tabla[i] = taux[j];
  }

  free(taux);
  return n_obs;
}



/**
 * @brief Elige un pivote, que puede ser la media, mediana o el 
 * elemento central y ordena la tabla en función de ese pivote
 * 
 * @param tabla la tabla a ordenar
 * @param ip primer elemento de la tabla
 * @param iu último elemento de la tabla
 * 
 * @return la tabla ordenada
*/
int quicksort(int* tabla, int ip, int iu) {  
  int pos;
  int n_obs, st;

  if(ip>iu) return ERR;
  if (ip==iu) return 0;

 
  n_obs = partition(tabla, ip, iu, &pos);
  if(n_obs == ERR) return ERR;

  if (ip < pos-1)
  {
    st = quicksort(tabla, ip, pos-1);
    if(st == ERR) return ERR;
    n_obs += st;
  }
  if ((pos+1) < iu)
  {
    st = quicksort(tabla, pos+1, iu);
    if(st == ERR) return ERR;
    n_obs += st;
  }

  return n_obs;
}

/**
 * @brief realiza una partición en dos respecto al total de la tabla
 * 
 * @param tabla la tabla a ordenar
 * @param ip primer elemento de la tabla
 * @param iu último elemento de la tabla
 * @param pos pivote divisiorio sobre el cual se va a ordenar la tabla
 * 
 * @return el número de operaciones básicas realizadas
*/
int partition(int* tabla, int ip, int iu, int *pos) {
  int i, k, n_obs;
  n_obs = 0;

  if(median_stat(tabla, ip, iu, pos) == ERR) {
    return ERR;
  }

  k = tabla[*pos];

  swap(&tabla[ip], &tabla[*pos]);
  *pos = ip;

  for (i = ip + 1; i <= iu; i++)
  {
    n_obs++;
    if (tabla[i] < k)
    {
      (*pos)++;
      swap(&tabla[i], &tabla[*pos]);
    }
  }

  swap(&tabla[ip], &tabla[*pos]);

  return n_obs;
}

/**
 * @brief sitúa como pivote el primer elemento de la tabla
 * 
 * @param tabla la tabla a ordenar
 * @param ip primer elemento de la tabla
 * @param iu último elemento de la tabla
 * @param pos pivote divisiorio sobre el cual se va a ordenar la tabla
 * 
 * @return OK si el procedimiento se ha realizado correctamente. 
 * ERROR si existe algún error en el procedimiento
*/
int median(int *tabla, int ip, int iu, int *pos) {
  if(tabla == NULL || ip > iu) return ERR;

  (*pos) = iu;

  return OK;
}

/**
 * @brief sitúa como pivote el valor medio del primer elemento y el último
 * 
 * @param tabla la tabla a ordenar
 * @param ip primer elemento de la tabla
 * @param iu último elemento de la tabla
 * @param pos pivote divisiorio sobre el cual se va a ordenar la tabla
 * 
 * @return OK si el procedimiento se ha realizado correctamente. 
 * ERROR si existe algún error en el procedimiento
*/
int median_avg(int *tabla, int ip, int iu, int *pos){
  if(tabla==NULL || ip>iu) return ERR;

  (*pos) = (iu+ip)/2;

  return OK;
}

/**
 * @brief sitúa como pivote el valor mediano, que es el elemento situado 
 * en la posición de la mitad de una tabla ordenada
 * 
 * @param tabla la tabla a ordenar
 * @param ip primer elemento de la tabla
 * @param iu último elemento de la tabla
 * @param pos pivote divisiorio sobre el cual se va a ordenar la tabla
 * 
 * @return OK si el procedimiento se ha realizado correctamente. 
 * ERROR si existe algún error en el procedimiento
*/
int median_stat(int *tabla, int ip, int iu, int *pos) {
  if(tabla == NULL || ip>iu) return ERR;

  (*pos) = (tabla[ip] + tabla[iu] + tabla[(iu+ip)/2]) / 3;

  return OK;
}