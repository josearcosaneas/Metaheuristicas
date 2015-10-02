#include <math.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


/************************ TYPES ******************************/

// guarda todos los datos iniciales
typedef double **TTABLA;

// tipo de dato del gen
typedef int TipoGen;

// tipo de dato del cromosoma
typedef TipoGen *TipoCromosoma;

// cada miembro de la poblacion tendra esta forma
typedef struct {
	TipoCromosoma Gene; //permutacion
	double objetivo; // evecualcion
	int cambio; // bool , si ha sido evaluaco 1, si no 0
} ESTRUCTURA;

// lo usare para la evaluacion de cada cromosoma
typedef int *TipoConfiguracion;
typedef double **TipoCentros;


/*********** GENERADOR DE NUMEROS PSEUDOALETORIOS ************/

/* used in random number generator below */
#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9
/*******************************************************************/
/*	 Rand computes a psuedo-random				   */
/*	 float value between 0 and 1, excluding 1.  Randint	   */
/*	 gives an integer value between low and high inclusive.	   */
/*  Randfloat genera un flotante entre low y high, incluido low    */
/*  y no incluido high                                             */
/*******************************************************************/

#define Rand()  (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE )

#define Randint(low,high) ( (int) (low + (high-(low)+1) * Rand()))

#define Randfloat(low,high) ( (low + (high-(low))*Rand()))

/** end of file **/
