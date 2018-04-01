/**
 * @file 
 * @author John David Gonzalez - Mateo Llano, johnd.gonzalez@udea.edu.co - mateo.llano@udea.edu.co
 * @version 1.7
 * 
 */

/** Importación de librerias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Definción de estructuras */

/**
 * Estructura creada con alias para idenfiticar algunos de los elementos de un proceso
 *
 */
typedef struct
{
    char nombre[25];
    char estado[3];
    char memoria[25];
    char data[25];
    char stack[25];
    char text[25];
    char cv[25];  /** Representa los cambios de contexto voluntarios */
    char cnv[25]; /** Representa los cambios de contexto realizados no voluntarios */
} proceso;

/** Definición de funciones */
int obtenerInformacion(proceso *, char[10]);
void imprimirInformacion(proceso *);
void obtenerInformacionProceso(char cadena[1000], proceso *pr, char nroProceso[10]);

/**
 * 
 * @param arg -> número de parametros que se le ingresa al ejecutable
 * @param args -> contiene los paramatros ingresados
 */
int main(int arg, char **args)
{
    //Con el argumento -l pasado como parametro se muestra la información de los porcesos ingresados.
    if (strcmp(args[1], "-l") == 0)
    {
        proceso *proce;
        printf("-- Información recolectada!!!\n\n");

        for (int i = 2; i < arg; i++)
        {
            printf("Pid: %s\n", args[i]);
            proce = (proceso *)malloc(sizeof(proceso));
            if(obtenerInformacion(proce, args[i])==0){
                printf("El proceso no existe.\n");
            }            
            imprimirInformacion(proce);
            printf("\n\n");
            free(proce);
        }
    }
    else if (strcmp(args[1], "-r") == 0)
    {

        char ruta[100] = "psinfo-report-";
        for (int i = 2; i < arg; i++)
        {
            strcat(ruta, args[i]);
            strcat(ruta, "-");
        }
        strcat(ruta, ".info");

        char cadena[1000];
        proceso *proce;

        FILE *fl;
        fl = fopen(ruta, "wt");

        for (int i = 2; i < arg; i++)
        {
            proce = (proceso *)malloc(sizeof(proceso));
            if(obtenerInformacion(proce, args[i])==0){
                strcat(cadena,"El proceso ");
                strcat(cadena,args[i]);
                strcat(cadena, " no existe.\n\n");
                fputs(cadena,fl);
                strcpy(cadena,"");
                continue;
            }
            obtenerInformacionProceso(cadena, proce, args[i]);

            fputs(cadena, fl);
            strcpy(cadena, "");
            free(proce);
        }
        fclose(fl);
        printf("Archivo de salida generado: %s\n", ruta);
    }
    else if (arg > 1)
    {
        proceso pr;
        if(obtenerInformacion(&pr, args[1])==0){
            printf("El proceso no existe.\n");
            return 0;            
        }
        imprimirInformacion(&pr);
    }

    return 0;
}

/** Implementacion de funciones */

/**
 * Obtiene información referente a un proceso
 * @param pr -> atributo correspondiente a una estructura tipo proceso
 * @param ruta -> contiene el número del proceso
 * @return -> 0: el proceso no existe, 1: el proceso existe
 * 
 */
int obtenerInformacion(proceso *pr, char ruta[10])
{
    char c[25] = "/proc/";
    strcat(c, ruta);
    strcat(c, "/status");
    FILE *file;
    char a[25];
    char b[25];
    file = fopen(c, "r");
    strcpy(pr->memoria, "-1");
    if (file == NULL){        
        return 0;
    }
    while (fscanf(file, " %[^:]:%s", a, b) > 0)
    {

        if (strcmp(a, "Name") == 0)
        {
            strcpy(pr->nombre, b);
        }
        else if (strcmp(a, "VmPeak") == 0)
        {
            strcpy(pr->memoria, b);
        }
        else if (strcmp(a, "State") == 0)
        {
            strcpy(pr->estado, b);
        }
        else if (strcmp(a, "kB\nVmData") == 0)
        {
            strcpy(pr->data, b);
        }
        else if (strcmp(a, "kB\nVmStk") == 0)
        {
            strcpy(pr->stack, b);
        }
        else if (strcmp(a, "kB\nVmExe") == 0)
        {
            strcpy(pr->text, b);
        }
        else if (strcmp(a, "voluntary_ctxt_switches") == 0)
        {
            strcpy(pr->cv, b);
        }
        else if (strcmp(a, "nonvoluntary_ctxt_switches") == 0)
        {
            strcpy(pr->cnv, b);
        }
    }
    fclose(file);
    return 1;
}

/**
 * Imprime la información referente a un proceso
 * @param pr -> contiene la información del proceso que se va imprimir (mostrar en consola)
 * 
 */
void imprimirInformacion(proceso *pr)
{
    printf("Nombre del proceso: %s\n", pr->nombre);
    printf("Estado del proceso: %s\n", pr->estado);
    if (strcmp(pr->memoria, "-1") == 0)
    {
        printf("La información de memoria no se encuentra disponible en este proceso.\n");
    }
    else
    {
        printf("Tamaño total de la imagen de memoria: %s kB\n", pr->memoria);
        printf("\tTamaño de la memoria en la región TEXT: %s kB\n", pr->text);
        printf("\tTamaño de la memoria en la región DATA: %s kB\n", pr->data);
        printf("\tTamaño de la memoria en la región STACK: %s kB\n", pr->stack);
    }
    printf("Número de cambios de contexto realizados (voluntarios - no voluntarios): %s - %s\n", pr->cv, pr->cnv);
}
/**
 * Concatena toda la información referente a un proceso.
 * @param cadena -> Variable a la que se le almacenará la información de un proceso.
 * @param pr -> contiene la información del proceso que se va a concatenar
 * @param nroProceso -> Contiene el número de proceso al cual se está accediendo a su información
 * 
 */
void obtenerInformacionProceso(char cadena[1000], proceso *pr, char nroProceso[10])
{
    strcat(cadena, "Pid: ");
    strcat(cadena, nroProceso);
    strcat(cadena, "\nNombre del proceso: ");
    strcat(cadena, pr->nombre);
    strcat(cadena, "\nEstado del proceso: ");
    strcat(cadena, pr->estado);
    if (strcmp(pr->memoria, "-1") == 0)
    {
        strcat(cadena, "\nLa información de memoria no se encuentra disponible en este proceso.");
    }
    else
    {
        strcat(cadena, "\nTamaño total de la imagen de memoria: ");
        strcat(cadena, pr->memoria);
        strcat(cadena, " kB");
        strcat(cadena, "\n\tTamaño de la memoria en la región TEXT: ");
        strcat(cadena, pr->text);
        strcat(cadena, " kB");
        strcat(cadena, "\n\tTamaño de la memoria en la región DATA: ");
        strcat(cadena, pr->data);
        strcat(cadena, " kB");
        strcat(cadena, "\n\tTamaño de la memoria en la región STACK: ");
        strcat(cadena, pr->stack);
        strcat(cadena, " kB");
    }
    strcat(cadena, "\nNúmero de cambios de contexto realizados (voluntarios - no voluntarios): ");
    strcat(cadena, pr->cv);
    strcat(cadena, " - ");
    strcat(cadena, pr->cnv);
    strcat(cadena, "\n\n");
}
