#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    char nombre[25];
    char estado[3];
    char memoria[25];
    char data[25];
    char stack[25];
    char text[25];
    char cv[25];
    char cnv[25];
}proceso;

void obtenerInformacion(proceso*,char[10]);
void imprimirInformacion(proceso*);

int main(int arg, char **args){
    if(arg > 1){        
        proceso pr;
        
        obtenerInformacion(&pr,args[1]);        
        imprimirInformacion(&pr);        
    }
    
    return 0;
}

void obtenerInformacion(proceso* pr,char ruta[10]){
    char c[25] = "/proc/";
        strcat(c,ruta);
        strcat(c,"/status");
     FILE *file;
     char a[25];
     char b[25];
     file = fopen(c,"r");
        if(file == NULL){
            printf("El proceso ingresado no existe.\n");
            exit(1);
        }

        strcpy(pr->memoria,"-1");

        while(fscanf(file," %[^:]:%s",a,b)>0){

            if(strcmp(a, "Name") == 0){
                strcpy(pr->nombre,b); 
            }
            else if(strcmp(a, "VmPeak") == 0){
                strcpy(pr->memoria,b); 
            }
            else if(strcmp(a,"State")==0){
                strcpy(pr->estado,b);
            }
            else if(strcmp(a,"kB\nVmData")==0){                
                strcpy(pr->data,b);
            }
            else if(strcmp(a,"kB\nVmStk")==0){
                strcpy(pr->stack,b);
            }
            else if(strcmp(a,"kB\nVmExe")==0){
                strcpy(pr->text,b);
            }
            else if(strcmp(a,"voluntary_ctxt_switches")==0){
                strcpy(pr->cv,b);
            }
            else if(strcmp(a,"nonvoluntary_ctxt_switches")==0){
                strcpy(pr->cnv,b);
                
            }
        }        
        fclose(file);
}

void imprimirInformacion(proceso *pr){
    printf("Nombre del proceso: %s\n",pr->nombre);
    printf("Estado del proceso: %s\n",pr->estado);
    if(strcmp(pr->memoria,"-1")==0){
        printf("La información de memoria no se encuentra disponible en este proceso.\n");
    }
    else{
        printf("Tamaño total de la imagen de memoria: %s kB\n",pr->memoria);
        printf("\tTamaño de la memoria en la región TEXT: %s kB\n",pr->text);
        printf("\tTamaño de la memoria en la región DATA: %s kB\n",pr->data);
        printf("\tTamaño de la memoria en la región STACK: %s kB\n",pr->stack);
    }
    printf("Número de cambios de contexto realizados (voluntarios - no voluntarios): %s - %s\n",pr->cv,pr->cnv);

}