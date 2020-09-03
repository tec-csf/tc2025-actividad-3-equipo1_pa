#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define tamTitulo 255
#define noUrnas 10
#define tamUrnas 1024

void procesaDir(DIR *dptr, int *urnas);
int uNecesarias();

int main(int argc, char *argv[]){
   if(argc != 2){
      printf("Falta dirección de directorio deseado\n");
      return 1;
   }

   int *urnas = (int*) malloc(noUrnas * sizeof(int));
   int *aux  = urnas;
   for(int i = 0; i<noUrnas ; i++){
      *aux = 0;
      aux++;
   }
   
   DIR *dirptr = NULL;

   char *directorioOg =(char*) malloc(strlen(argv[1])*sizeof(char));
   strcpy(directorioOg, argv[1]);

   dirptr = opendir(argv[1]);
   if(dirptr == NULL){
      printf("No se pudo abrir el directorio deseado\n");
      return 1;
   }
   else{
      //llama la función que procesa directorios
      printf("Procesando directorio e imprimiendo tabla...\n");
      procesaDir(dirptr,urnas);
   }
   printf("Urna\t\t N° archivos\t\t Histograma\n");
   int mayor = 0;
   aux = urnas;
   for(int i = 0; i< noUrnas;i++){
      if(*aux > 100){
         mayor = 1;
         break;
      }
   }
   aux  = urnas;
   for(int i = 0; i<noUrnas ; i++){
      printf("%d - %d:\t\t %d\t\t ", i * tamUrnas, i*tamUrnas + tamUrnas -1, *aux);
      if(mayor == 1){
         for(int j=0; j<(*aux * .01); j++){
            printf("*");
         }
      }
      else{
         for(int j=0; j<*aux; j++){
            printf("*");
         }
      }
      printf("\n");
      aux++;
   }
   
   free(urnas);
   free(directorioOg);

   return 0;
}

void procesaDir(DIR *dirptr, int *urnas){
   struct stat statBuff;
   struct dirent *dirStruct = NULL;
   int * aux = urnas;
   aux  = urnas;
   dirStruct = readdir(dirptr);
   while(dirStruct != NULL){
      aux  = urnas;
      if(dirStruct->d_type == DT_REG){
         aux = urnas;
         stat(dirStruct->d_name,&statBuff);
         if(statBuff.st_size / tamUrnas > noUrnas){
            aux += noUrnas - 1;
         }
         else{
            aux += (statBuff.st_size / tamUrnas);
         }
         *aux += 1;
      }
      else if(dirStruct->d_type == DT_DIR && strcmp(dirStruct->d_name,".") != 0 && strcmp(dirStruct->d_name,"..") != 0){
         DIR *nuevoDirPtr;
         char * nuevoDir =(char*) malloc(tamTitulo*sizeof(char));
         getcwd(nuevoDir,tamTitulo);
         strcat(nuevoDir,"/");
         strcat(nuevoDir,dirStruct->d_name);
         nuevoDirPtr = opendir(nuevoDir);
         procesaDir(nuevoDirPtr, urnas);
         free(nuevoDir);
      }
      dirStruct = readdir(dirptr);
   }
}
