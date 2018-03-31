/** Importación de librerias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Definición de estructuras */

/**
 * Estructura creada con alias para idenficar algunos de los elementos de un proceso
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
void obtenerInformacion(proceso *, char[10]);
void imprimirInformacion(proceso *);
void obtenerInformacionProceso(char cadena[1000], proceso *pr, char nroProceso[10]);

int main(int arg, char **args)
{
    /** Caso en el que el argumento ingresado sea -l */
    if (strcmp(args[1], "-l") == 0)
    {
        proceso *proce;
        printf("-- Información recolectada!!!\n\n");

        for (int i = 2; i < arg; i++)
        {
            printf("Pid: %s\n", args[i]);
            proce = (proceso *)malloc(sizeof(proceso));
            obtenerInformacion(proce, args[i]);
            imprimirInformacion(proce);
            printf("\n\n");
            free(proce);
        }
    }
    /** Caso en el que el argumento ingresado sea -r */
    else if (strcmp(args[1], "-r") == 0)
    {

        char ruta[100] = "psinfo-report-";
        for (int i = 2; i < arg; i++)   /** Se genera el nombre del archivo .info acorde a los procesos ingresados*/
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
            obtenerInformacion(proce, args[i]);
            obtenerInformacionProceso(cadena, proce, args[i]);

            fputs(cadena, fl);
            strcpy(cadena, "");
            free(proce);
        }
        fclose(fl);
        printf("Archivo de salida generado: %s\n", ruta);
    }
    /** Caso en el que el argumento ingresado sean unicamente procesos */
    else if (arg > 1)
    {
        proceso pr;
        obtenerInformacion(&pr, args[1]);
        imprimirInformacion(&pr);
    }

    return 0;
}

/** Implementacion de funciones */

/**
 * Obtiene informacion referente a un proceso
 * @param pr -> atributo correspondiente a una estructura tipo proceso
 * @param ruta -> contiene el número del proceso
 * 
 */
void obtenerInformacion(proceso *pr, char ruta[10])
{
    char c[25] = "/proc/";
    strcat(c, ruta);
    strcat(c, "/status");
    strcpy(pr->memoria, "-1");
    FILE *file;
    char a[25];
    char b[25];
    file = fopen(c, "r");
    if (file == NULL)
    {
        printf("El proceso no existe.\n");
        return;
    }

    while (fscanf(file, " %[^:]:%s", a, b) > 0)
    {

        if (strcmp(a, "Name") == 0)
        {
            strcpy(pr->nombre, b);
        }
        else if (strcmp(a, "VmSize") == 0)
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
    { /** En caso de que la imagen de memoria no se encuentre disponible, ninguna otra información de memoria lo estará */
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
        strcat(cadena, pr->memoria);
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
