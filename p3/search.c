/**
 *
 * Description: Implementation of functions for search
 *
 * File: search.c
 * Author: Carlos Aguirre and Javier Sanz-Cruzado
 * Version: 1.0
 * Date: 14-11-2016
 *
 */


#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "search.h"
#include "times.h"

/**
 *  Key generation functions
 *
 *  Description: Receives the number of keys to generate in the n_keys
 *               parameter. The generated keys go from 1 to max. The
 * 				 keys are returned in the keys parameter which must be 
 *				 allocated externally to the function.
 */
  
/**
 *  Function: uniform_key_generator
 *               This function generates all keys from 1 to max in a sequential
 *               manner. If n_keys == max, each key will just be generated once.
 */
void uniform_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for(i = 0; i < n_keys; i++) keys[i] = 1 + (i % max);

  return;
}

/**
 *  Function: potential_key_generator
 *               This function generates keys following an approximately
 *               potential distribution. The smaller values are much more 
 *               likely than the bigger ones. Value 1 has a 50%
 *               probability, value 2 a 17%, value 3 the 9%, etc.
 */
void potential_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for(i = 0; i < n_keys; i++) 
  {
    keys[i] = .5+max/(1 + max*((double)rand()/(RAND_MAX)));
  }

  return;
}

/**
 * @brief Inicializa un diccionario
 * 
 * @param size el tamaño del diccionario
 * @param order el diccionario está ordenado o no
 * 
 * @returns Un diccionario inicializado
*/
PDICT init_dictionary (int size, char order)
{
  PDICT dic_new;

  assert(size >= 0 && (order == SORTED || order == NOT_SORTED));

  dic_new = (PDICT)malloc(sizeof(DICT));
  if(dic_new == NULL) return NULL;

  dic_new->size = size;
  dic_new->n_data = 0;
  dic_new->order = order;
  dic_new->table = (int*)malloc(sizeof(int) * dic_new->size);
  if(dic_new->table == NULL) {
    free(dic_new);
    return NULL;
  }

  return dic_new;
}

/**
 * @brief Libera la memoria de un diccionario
 * 
 * @param pdict el diccionario a liberar
 * 
*/
void free_dictionary(PDICT pdict)
{
  assert(pdict != NULL);
  
  free(pdict->table);
	free(pdict);
}


/**
 * @brief Inserta un elemento en el diccionario
 * 
 * @param pdict el diccionario en el que se inserta el elemento
 * @param order el diccionario está ordenado o no
 * 
 * @returns El número de OBs realizadas
*/
int insert_dictionary(PDICT pdict, int key)
{
  int j, nobs = 0, aux, i;

  assert(pdict != NULL);
  
  pdict->n_data++;
  pdict->table[pdict->n_data-1] = key;
    
  if (pdict->order == SORTED)
  {
    for(i=pdict->n_data-2, j=pdict->n_data-1; i >= 0 && pdict->table[i] > pdict->table[j]; i--, j--) {
      nobs++;
      aux = pdict->table[i];
      pdict->table[i] = key;
      pdict->table[j] = aux;
    }
  }

  return nobs;
}

/**
 * @brief Inserta en el diccionario una cadena de elementos
 * 
 * @param pdict el diccionario en el que se insertan los elementos
 * @param keys puntero a los elementos a insertar
 * @param n_keys número de elementos a insertar
 * 
 * @returns El número de OBs realizadas
*/
int massive_insertion_dictionary (PDICT pdict,int *keys, int n_keys)
{
  int i, obs=0, st;

  assert(pdict != NULL && keys != NULL);

  for(i=0; i<n_keys; i++) {
    st = insert_dictionary(pdict, keys[i]);
    if(st==ERR) return ERR;
    obs += st;
  }
  
  return obs;
}

/**
 * @brief Busca un elemento en el diccionario
 * 
 * @param pdict el diccionario en el que se busca el elemento
 * @param key elemento que se busca
 * @param ppos puntero a la posición dentro del diccionario
 * 
 * @returns El número de OBs realizadas
*/
int search_dictionary(PDICT pdict, int key, int *ppos, pfunc_search method)
{
  int obs;

  assert(pdict != NULL && method != NULL);

  obs = method(pdict->table, 0, pdict->n_data-1, key, ppos);

  return obs;
}


/**
 * @brief Busca un elemento en el diccionario con búsqueda binaria
 * 
 * @param table tabla del diccionario en el que se busca el elemento
 * @param F el primer elemento de la tabla
 * @param L el último elemento de la tabla
 * @param key elemento que se busca
 * @param ppos puntero a la posición dentro del diccionario
 * 
 * @returns El número de OBs realizadas
*/
int bin_search(int *table,int F,int L,int key, int *ppos)
{
  int mid, obs = 0;

  assert(table != NULL);
  
  while(F <= L) {
    mid = (F+L)/2;
    obs++;
    if(key == table[mid]) {
      (*ppos) = mid;
      return obs;
    }
    if(key < table[mid]) {
      L = mid-1;
    }
    else {
      F = mid+1;
    }
  }

  return NOT_FOUND;
}

/**
 * @brief Busca un elemento en el diccionario con búsqueda lineal
 * 
 * @param table tabla del diccionario en el que se busca el elemento
 * @param F el primer elemento de la tabla
 * @param L el último elemento de la tabla
 * @param key elemento que se busca
 * @param ppos puntero a la posición dentro del diccionario
 * 
 * @returns El número de OBs realizadas
*/
int lin_search(int *table,int F,int L,int key, int *ppos)
{
  int i, obs = 0;

  assert(table != NULL);

  for(i=F; i<=L; i++) {
    obs++;
    if(key == table[i]) {
      *ppos = i;
      return obs;
    }
  }

  return NOT_FOUND;
}
/**
 * @brief Busca un elemento en el diccionario con búsqueda lineal autoorganizada
 * 
 * @param table tabla del diccionario en el que se busca el elemento
 * @param F el primer elemento de la tabla
 * @param L el último elemento de la tabla
 * @param key elemento que se busca
 * @param ppos puntero a la posición dentro del diccionario
 * 
 * @returns El número de OBs realizadas
*/
int lin_auto_search(int *table,int F,int L,int key, int *ppos)
{
  int obs, aux;

  assert(table != NULL);

  obs = lin_search(table, F, L, key, ppos);
  if(obs == NOT_FOUND) return obs;

  if((*ppos) != 0) {
    aux = table[(*ppos)-1];
    table[(*ppos)-1] = table[*ppos];
    table[*ppos] = aux;
  }

  return obs;
}