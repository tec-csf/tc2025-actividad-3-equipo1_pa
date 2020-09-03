#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    int tam;
    int prioridad;
    int usado;
    int finalizado;
}tarea;

tarea *comparacionRR(tarea *procesos, int np);
int terminado(tarea *procesos, int np);
void correrProcesos( tarea ** cores, int ncpu, int quantum);
void printProcesos(tarea *A);

int main(int argc, const char * argv[]){
	printf("-----Round Robin-----\n");
  tarea * procesos;
  tarea * aux;
  int terminar = 0;
	int np = 0, ncpu = 0, nq = 0, quantum = 3;
	
	//Se pide el numero de cpu's
	printf("\nNumero de cpu's' :");
	scanf("%d", &ncpu);
  tarea ** cores =(tarea**) malloc(sizeof(tarea*) * ncpu);
	
	//Se pide el numero de procesos
  printf("\nNumero de procesos : ");
  scanf("%d", &np);
  procesos =(tarea*) malloc(sizeof(tarea) * np);

	//se pide el tama�o de cada proceso
  aux = procesos;
	for(int i = 0; i<np; i++){
		printf("Tama�o de quantum del tarea %d: ",i+1);
		scanf("%d", &aux->tam);
    printf("Prioridad del tarea %d: ",i+1);
    scanf("%d", &aux->prioridad);
    aux->finalizado = 0;     
    
    if(i != np-1){ 
      aux ++;
    }
	}
    
	//Algoritmo de RR
  tarea ** asignador = cores;
	while(terminar != 1){
		for(int i=0; i<ncpu; i++){
      *asignador = comparacionRR(procesos,np);
      asignador ++;
    }    
    asignador = cores;
    //remover sleep en caso de querer ejecutar en tiempo real
    sleep(1);
    correrProcesos(cores, ncpu, quantum);
    terminar = terminado(procesos, np);
	}
  free(cores);
  free(procesos);
	return 0;
}
//Funcion que utiliza RoundRobin para obtener el mejor candidato para ingresar al cpu
tarea * comparacionRR(tarea * procesos, int np){
    tarea * aux;
    tarea * copia;
    aux = procesos;
    copia = procesos;
    int term = terminado(procesos,np);
    int desplazamiento = 0;
    if(term == 1){
        printf("Ya no hay procesos disponibles\n");
        return NULL;
    }
    //seleccioón del primer proceso que no haya concluido
    for(int i=0; i<np; i++){
        if(aux->finalizado == 0){
            copia = aux;
            desplazamiento += i;
            break;
        }
        aux++;
    }
    //mueve aux hasta el mejor candidato para ingresar a cores/CPU
    for(int i = (1 + desplazamiento); i<np; i++){
        copia ++;
        if(copia->finalizado == 0){
            if(copia->prioridad < aux->prioridad){
                aux = copia;
            }else if(copia->prioridad == aux->prioridad){
                if(copia->usado < aux->usado){
                   aux = copia;
                }
            }
        }
    }
    aux->usado ++;
    return aux;
}
//condicion que revisa si existen procesos que no hayan finalizado (todos finalizaron = 1) (quedan sin finalizar = 0)
int terminado(tarea *procesos, int np){
    tarea * aux = procesos;
    for(int i = 0; i< np; i++){
        if(aux->finalizado == 0){
            return 0;
        }
        aux++;
    }
    return 1;
}
//simula el procesamiento de los procesos
void correrProcesos(tarea ** cores, int ncpu, int quantum){
        tarea ** asignador = cores;
        tarea * aux = *asignador;
        asignador = cores;
        aux = *asignador;
        for(int i = 0; i<ncpu; i++){
            aux->tam = aux->tam - quantum;
            if(aux->tam < 1){
                aux->finalizado = 1;
            }
           asignador += i + 1;
           aux= *asignador;
        }
}
//Herramienta para imprimir los contenidos de un proceso
void printProcesos(tarea *A){
    printf("Tam: %d, priori: %d, usado: %d, final: %d \n", A->tam, A->prioridad, A->usado, A->finalizado);
}
