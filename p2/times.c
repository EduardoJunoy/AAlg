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
#include "times.h"
#include "sorting.h"
#include "permutations.h"

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