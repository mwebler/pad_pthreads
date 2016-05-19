/*
 * Aplicação paralela para soma de vetores utilizando pthreads
 * Processamento de Alto Desempenho - Unisinos - 2016/2
 * 
 * Carolina Darski
 * Everton Correa
 * Matheus Webler
 * Tiago Vieira
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Parametro da função de execução das threads */
struct thread_info { 
	pthread_t thread_id;   /* id do pthread_create() */
	int       thread_num; /* id interno da aplicação */
	int *vetor1; /* vetor 1 */
	int *vetor2; /* vetor 2 */
	int *result; /* vetor resultado */
	int size; /* tamanho dos vetores */
};

/* Função de soma de vetores. Recebe como parâmetro thread_info */
static void * soma_vetor(void *arg){
	struct thread_info *tinfo = arg;
	for(int i = 0; i < tinfo->size; i++){
		tinfo->result[i] = tinfo->vetor1[i] + tinfo->vetor2[i];
	}
	
	return NULL;
}

int main(int argc, char* argv[]){
	/* Vetores */	
	int *vetor1;
	int *vetor2;
	int *result;

	/* Informações das threads */
	struct thread_info *tinfo;
	pthread_attr_t attr;
	
	/* Resultados */
	void *res;
	int ret;
	
	/* Pelo menos 3 parametros: soma_vetor QTD_THREADS TAM_VETOR */
	if(argc < 3){
		printf("Falta argumento \n");
		return 1;
	}
	
	/* Le parametros */
	int qtd_threads = atoi(argv[1]);
	int tam_vetor = atoi(argv[2]);
	
	printf("tamanho vetor: %d \n", tam_vetor);
	printf("quantidade threads: %d \n", qtd_threads);
	
	/* Cria os vetores onde serão armazenados os resultados */
	vetor1 = calloc(1, tam_vetor*4);
	vetor2 = calloc(1, tam_vetor*4);
	result = calloc(1, tam_vetor*4);

	/* Inicializa vetores com valores aleatorios */
	for(int i = 0; i < tam_vetor; i++){
		vetor1[i] = rand();
		vetor2[i] = rand();
	}	
	
	/* Cria estruturas de informações das threads */
	tinfo = calloc(qtd_threads, sizeof(struct thread_info));
	if (tinfo == NULL){
		printf("Erro calloc tinfo\n");
		return 1;
	}
	
	/* Inicializa estrutura de atributos dummy para a thread */
	ret = pthread_attr_init(&attr);
	if (ret != 0)
		printf("Erro pthread_attr_init\n");
	
	/* Quantidade de somas de cada thread */
	int tam_vetor_thread = tam_vetor / qtd_threads;
	/* Coloca o resto na primeira thread */
	int tamanho = tam_vetor_thread + tam_vetor % qtd_threads;	
	
	/* Cria as threads */
	int inicio = 0;
 	for(int i=0; i < qtd_threads; i++){

		/* Preenche estrutura de parâmetros para a thred */
		tinfo[i].thread_num = i + 1;
		tinfo[i].size = tamanho;
		tinfo[i].vetor1 = &vetor1[inicio];
		tinfo[i].vetor2 = &vetor2[inicio];
		tinfo[i].result = &result[inicio];
		
		/* Cria a thread */
		ret = pthread_create(&tinfo[i].thread_id, &attr, &soma_vetor, (void *)&tinfo[i]);
		if (ret != 0)
			printf("Erro pthread_create\n");
		
		/* Endereço de inicio nos vetores e tamanho para próxima thread */
		inicio += tamanho;
		tamanho = tam_vetor_thread;
	}
	
	/* Aguarda todas as threads finalizarem */
	for (int tnum = 0; tnum < qtd_threads; tnum++) {
		ret = pthread_join(tinfo[tnum].thread_id, &res);
		if (ret != 0)
			printf("Erro pthread_join\n");
		
		printf("Thread %d finalizada\n", tinfo[tnum].thread_num);
	}
	
	/* Impime resultados na tela */
	for(int i = 0; i < tam_vetor; i++){
		printf("%d + %d = %d \n", vetor1[i], vetor2[i], result[i]);
	}
			
	return 0;
}
