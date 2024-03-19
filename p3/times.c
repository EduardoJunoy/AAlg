/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "times.h"
#include "sorting.h"
#include "permutations.h"
#include "search.h"

/**
 * @brief Calcula los tiempo de ejecución de un algoritmo de ordenación
 * 
 * @param metodo la función de ordenación
 * @param n_perms el número de permutaciones
 * @param N el tamaño de las permutaciones
 * @param ptime un puntero a la estructura time_aa
 * 
 * @return ERR en caso de error y OK en caso contrario
*/
short average_sorting_time(pfunc_sort metodo, 
                              int n_perms,
                              int N, 
                              PTIME_AA ptime)
{
  int **permutaciones;
  int ini, fin, i, ob;
  ptime->N = N;
  ptime->n_elems = n_perms;
  ptime->min_ob = ptime->max_ob = ptime->average_ob = 0;

  permutaciones = generate_permutations(n_perms, N);
  if(permutaciones == NULL){
    return ERR;
  }

  ini = clock();
  for (i = 0; i < n_perms; i++)
  {
    ob = metodo(permutaciones[i], 0, N-1);
    if(ob == ERR) {
      for(i=0; i<n_perms; i++) {
        free(permutaciones[i]);
      }
      free(permutaciones);
      return ERR;
    }
    
    ptime->average_ob += ob;
    
    if(ptime->min_ob > ob || ptime->min_ob == 0){
      ptime->min_ob = ob;
    }
    else if(ptime->max_ob < ob || ptime->max_ob == 0){
      ptime->max_ob = ob;
    }
  }
  fin = clock();

  ptime->time = (double)(fin-ini)/CLOCKS_PER_SEC/n_perms;
  ptime->average_ob /= n_perms;

  for(i=0; i<n_perms; i++)
  {
    free(permutaciones[i]);
  }
  free(permutaciones);
  return OK;
}

/**
 * @brief utiliza average_sorting_time y save_time_table para calcular los tiempo de ejecución de un algoritmo y guardarlos en un archivo
 *
 * @param method un algoritmo de ordenación
 * @param file el nombre del archivo donde se guardarán los tiempos de ejecución
 * @param num_min el ínfimo del conjunto de tamaños que tomarán las permutaciones
 * @param num_max el supremo del conjunto de tamaños que tomarán las permutaciones
 * @param incr incremento del tamaño de la permutación en cada iteración
 * @param n_perms número de permutaciones a ordenar en cada iteración
 * 
 * @return ERR en caso de error y OK en caso contrario
*/
short generate_sorting_times(pfunc_sort method, char* file, 
                                int num_min, int num_max, 
                                int incr, int n_perms)
{
  int n_times = ((num_max-num_min)/incr)+1;
  int tamanyo, i;
  PTIME_AA p_time, times;

  p_time = (PTIME_AA)malloc(sizeof(p_time[0]));
  if(p_time == NULL)
  {
    return ERR;
  }
  times = (PTIME_AA)malloc(sizeof(times[0]) * n_times);
  if(times == NULL)
  {
    free(p_time);
    return ERR;
  }
  for (i=0, tamanyo = num_min; tamanyo <= num_max; tamanyo += incr, i++)
  {
    if(average_sorting_time(method, n_perms, tamanyo, p_time) == ERR)
    {
      free(p_time);
      free(times);
      return ERR;
    }
    times[i].N = p_time->N;
    times[i].n_elems = p_time->n_elems;
    times[i].time = p_time->time;
    times[i].average_ob = p_time->average_ob;
    times[i].max_ob = p_time->max_ob;
    times[i].min_ob = p_time->min_ob;
  }
  if(save_time_table(file, times, n_times) == ERR) {
    free(p_time);
    free(times);
    return ERR;
  }

  free(p_time);
  free(times);
  return OK;
}

/**
 * @brief Escribe los tiempos de ejecución y otros parámetros del rendimiento del algoritmo en un archivo
 * 
 * @param file el nombre del archivo
 * @param ptime puntero a la información del rendimiento del algoritmo
 * @param n_times número de tiempos a los que apunta ptime
 * 
 * @return ERR en caso de error y OK en caso contrario
*/
short save_time_table(char* file, PTIME_AA ptime, int n_times)
{  
  int i;

  FILE *pf = fopen(file, "w");
  if(pf == NULL) return ERR;

  for(i=0; n_times > 0; i++, n_times--)
  {
    fprintf(pf,"%d  %.10f  %.2f  %d  %d\n", ptime[i].N, ptime[i].time, ptime[i].average_ob, ptime[i].max_ob, ptime[i].min_ob);
  }
  
  fclose(pf);
  return OK;
}

/**
 * @brief Calcula los tiempos de ejecución y OBs de un algoritmo de búsqueda
 * 
 * @param metodo metodo de búsqueda a usar
 * @param generator función generadora de claves
 * @param order indica si la lista está ordenada
 * @param N tamaño de la lista
 * @param n_times número de veces que se busca cada elemento de la lista
 * @param ptime puntero a la estructura TIME_AA
 * 
 * @returns OK o ERR
*/
short average_search_time(pfunc_search metodo, pfunc_key_generator generator, int order, int N, int n_times, PTIME_AA ptime) {
  PDICT dic = NULL;
  int *perm = NULL;
  int *keys_to_search = NULL;
  int i, pos, obs;
  long ini, fin;


  dic = init_dictionary(N, order);
  if(dic == NULL) return ERR;

  perm = generate_perm(N);
  if(perm == NULL) {
    free_dictionary(dic);
    return ERR;
  }

  keys_to_search = (int*)malloc(sizeof(keys_to_search[0]) * N * n_times);
  if(keys_to_search == NULL) {
    free_dictionary(dic);
    free(perm);
    return ERR;
  }

  if(massive_insertion_dictionary(dic, perm, N) == ERR) {
    free_dictionary(dic);
    free(perm);
    free(keys_to_search);
    return ERR;
  }

  generator(keys_to_search, N * n_times, N);


  ptime->max_ob = 0;
  ptime->min_ob = INT_MAX;
  ptime->average_ob = 0;
  obs = 0;
  ini = clock();
  for(i = 0; i < (N * n_times); i++) {
    obs = metodo(dic->table, 0, dic->n_data-1, keys_to_search[i], &pos);
    if(obs == NOT_FOUND) {
      free_dictionary(dic);
      free(perm);
      free(keys_to_search);
      return ERR;
    }
    if(obs > ptime->max_ob) {
      ptime->max_ob = obs;
    }
    if(obs < ptime->min_ob) {
      ptime->min_ob = obs;
    }
    ptime->average_ob += obs;
  }
  fin = clock();

  ptime->N = N;
  ptime->n_elems = N * n_times;
  ptime->time = (double)(fin-ini)/CLOCKS_PER_SEC/ptime->n_elems;
  ptime->average_ob /= ptime->n_elems;

  free_dictionary(dic);
  free(perm);
  free(keys_to_search);
  return OK;
}

/**
 * @brief hace uso de average_search_time y de save_time_table para calcular los tiempo de ejecución de un algoritmo de búsqueda y guardarlos en un archivo
 * 
 * @param method método de búsqueda a usar
 * @param generator función generadora de claves
 * @param order indica si la lista está ordenada
 * @param file nombre del archivo donde se guardarán los datos
 * @param num_min número mínimo de elemento de la lista
 * @param num_max número máximo de elementos de la lista
 * @param incr incremento en el que crece el número de elementos de la lista
 * @param n_times número de veces que se busca cada clave
 * 
 * @returns OK o ERR
*/
short generate_search_times(pfunc_search method, pfunc_key_generator generator, int order, char* file, int num_min, int num_max, int incr, int n_times) {

  int N, i;
  PTIME_AA ptime = (PTIME_AA)malloc(sizeof(ptime[0]) * ((num_max - num_min + 1)/incr));
  

  for(N = num_min, i = 0; N <= num_max; N += incr, i++) {
    if(average_search_time(method, generator, order, N, n_times, &ptime[i]) == ERR) {
      free(ptime);
      return ERR;
    }
  }

  if(save_time_table(file, ptime, (num_max - num_min + 1)/incr) == ERR) {
    free(ptime);
    return ERR;
  }

  free(ptime);
  return OK;
}