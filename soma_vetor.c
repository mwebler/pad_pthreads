#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   

int *vetor1;
int *vetor2;
int *result;

struct thread_info {    /* Used as argument to thread_start() */
	pthread_t thread_id;        /* ID returned by pthread_create() */
	int       thread_num;       /* Application-defined thread # */
	int start;
	int size;
};

static void * soma_vetor(void *arg){
	struct thread_info *tinfo = arg;
	printf("thread %d\n", tinfo->thread_num);
	for(int i = 0; i < tinfo->size; i++){
		int pos = tinfo->start + i;
		result[pos] = vetor1[pos] + vetor2[pos];
	}
}

int main(int argc, char* argv[]){

	if(argc < 3){
		printf("Falta argumento \n");
		return 1;
	}
	int qtd_threads = atoi(argv[1]);
	int tam_vetor = atoi(argv[2]);
	pthread_attr_t attr;
	void *res;
	
	vetor1 = malloc(tam_vetor);
	vetor2 = malloc(tam_vetor);
	result = malloc(tam_vetor);
	
	printf("%d \n", qtd_threads);
	printf("%d \n", tam_vetor);
	
	struct thread_info *tinfo;
	tinfo = calloc(qtd_threads, sizeof(struct thread_info));
	if (tinfo == NULL){
		printf("calloc\n");
		return 1;
	}
	
	/* Initialize thread creation attributes */
	int s;
	s = pthread_attr_init(&attr);
	if (s != 0)
		printf("pthread_attr_init\n");
	
	int tam_vetor_thread = tam_vetor / qtd_threads;
	//Coloca o resto na primeira thread
	int tamanho = tam_vetor_thread + tam_vetor % qtd_threads;
	
	printf("%d\n", tam_vetor_thread);
	printf("%d\n", tamanho);
	printf("%d\n", tam_vetor % qtd_threads);
	
	
	int inicio = 0;
 	for(int i=0; i < qtd_threads; i++){
		
		tinfo[i].thread_num = i + 1;
		tinfo[i].size = tamanho;
		tinfo[i].start = inicio;
		
		s = pthread_create(&tinfo[i].thread_id, &attr, &soma_vetor, (void *)&tinfo[i]);
		if (s != 0)
			printf("pthread_create\n");
		
		inicio += tamanho;
		tamanho = tam_vetor_thread;
		
	}
	
	for (int tnum = 0; tnum < qtd_threads; tnum++) {
		s = pthread_join(tinfo[tnum].thread_id, &res);
		if (s != 0)
			printf("pthread_join\n");

		printf("Joined with thread %d;\n",
			   tinfo[tnum].thread_num);
	}
	
	for(int i = 0; i < tam_vetor; i++){
		printf("%d + %d = %d \n", vetor1[i], vetor2[i], result[i]);
	}

			
	return 0;
}