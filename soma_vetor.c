#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   

int* vetor1;
int* vetor2;


void soma_vetor(){
	
}

int main(int argv, char** argc){

	if(argv < 3){
		printf("Falta argumento \n");
		return 1;
	}
	int qtd_threads = atoi(argc[1]);
	int tam_vetor = atoi(argc[2]);
	
	vetor1 = malloc(tam_vetor);
	vetor2 = malloc(tam_vetor);
	
	printf("%d \n", qtd_threads);
	printf("%d \n", tam_vetor);
	
	
	
/* 	for(int i=0; i < qtd_threads; i++){
		
	}

       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg); */
			
	return 0;
}