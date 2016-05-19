/*
 * Aplicação paralela para soma de vetores utilizando pthreads
 * Processamento de Alto Desempenho - Unisinos - 2016/1
 * 
 * Carolina Darski
 * Everton Correa
 * Matheus Webler
 * Tiago Vieira
 */


#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

/* Parametro da função de execução das threads */
struct thread_info { 
	pthread_t thread_id;  /* id do pthread_create() */
	int       thread_num; /* id interno da aplicação */
	int 	  consumer;   /* se verdadeiro, retira dinheiro */
	int 	  qty_actions; /* Quantas vezes soma ou subtrai dinheiro */
};

struct  magic_account
{
	double min_amount;
	double max_amount;
	double actual_amount;
	pthread_mutex_t acc_lock;
} * conta_magica;

int QTD_ACTIONS = 100;

sem_t mutex_cons;
sem_t mutex_prod;

/* Função de soma de vetores. Recebe como parâmetro thread_info */
static void * change_balance(void *arg){
	struct thread_info *tinfo = arg;

	for(int i = 0; i < tinfo->qty_actions; i++){
		pthread_mutex_lock(&conta_magica->acc_lock);
		if( tinfo->consumer ){
			if( conta_magica->actual_amount - 1 < conta_magica->min_amount ){
				printf("Esbarrou no limite mínimo");			
				sem_wait (&mutex_cons);
			}
			conta_magica->actual_amount--;
			sem_post(&mutex_prod);
		}else{
			if( conta_magica->actual_amount + 1 > conta_magica->max_amount ){
				printf("Esbarrou no limite máximo");
				sem_wait (&mutex_prod);
			}
			conta_magica->actual_amount++;
			sem_post(&mutex_cons);
		}
		pthread_mutex_unlock(&conta_magica->acc_lock);
	}
	
	return NULL;
}

int main(int argc, char* argv[]){

	pthread_attr_t attr;

	/* Informações das threads */
	struct thread_info *tinfo_prod;
	struct thread_info *tinfo_cons;
	
	void *res;
	int ret;

	/* Pelo menos 2 parametros: soma_vetor QTD_PROD QTD_CONS */
	if(argc < 3){
		printf("Falta argumento \n");
		return 1;
	}
	
	sem_init(&mutex_cons, 0, 1);
	sem_init(&mutex_prod, 0, 1);

	/* Le parametros */
	int qtd_prod = atoi(argv[1]);
	int qtd_cons = atoi(argv[2]);
	
	printf("Quantidade de produtores: %d \n", qtd_prod);
	printf("Quantidade de consumidores: %d \n", qtd_cons);

	conta_magica = calloc(1, sizeof(struct magic_account));

	conta_magica->min_amount = 0;
	conta_magica->max_amount = 1000;

	printf("Escroto humano");

	//É o mais recomendado?
	conta_magica->actual_amount = conta_magica->max_amount;

	if( pthread_mutex_init(&conta_magica->acc_lock, NULL) != 0 ){
		printf("\n não foi possível inicializar a conta.");
		return 1;
	}
	
	/* Cria estruturas de informações das threads */
	tinfo_prod = calloc(qtd_prod, sizeof(struct thread_info));
	if (tinfo_prod == NULL){
		printf("Erro calloc tinfo_prod\n");
		return 1;
	}

	tinfo_cons = calloc(qtd_cons, sizeof(struct thread_info));
	if (tinfo_cons == NULL){
		printf("Erro calloc tinfo_cons\n");
		return 1;
	}
	
	/* Inicializa estrutura de atributos dummy para a thread */
	ret = pthread_attr_init(&attr);
	if (ret != 0){
		printf("Erro pthread_attr_init\n");
	}
	

	/* Cria as threads */
	int inicio = 0;
 	for(int i=0; i < qtd_prod; i++){

		/* Preenche estrutura de parâmetros para a thred */
		tinfo_prod[i].thread_num = i + 1;

	 	tinfo_prod[i].consumer = 0;   /* se verdadeiro, retira dinheiro */
		tinfo_prod[i].qty_actions = QTD_ACTIONS; /* Quantas vezes soma ou subtrai dinheiro */
		
		/* Cria a thread */
		ret = pthread_create(&tinfo_prod[i].thread_id, &attr, &change_balance, (void *)&tinfo_prod[i]);
		if (ret != 0)
			printf("Erro pthread_create\n");
	}

 	for(int i=0; i < qtd_cons; i++){

		/* Preenche estrutura de parâmetros para a thred */
		tinfo_cons[i].thread_num = i + 1;

	 	tinfo_cons[i].consumer = 1;   /* se verdadeiro, retira dinheiro */
		tinfo_cons[i].qty_actions = QTD_ACTIONS; /* Quantas vezes soma ou subtrai dinheiro */
		
		/* Cria a thread */
		ret = pthread_create(&tinfo_cons[i].thread_id, &attr, &change_balance, (void *)&tinfo_cons[i]);
		if (ret != 0)
			printf("Erro pthread_create\n");
	}
	
	/* Aguarda todas as threads finalizarem */
	for (int tnum = 0; tnum < qtd_prod; tnum++) {
		ret = pthread_join(tinfo_prod[tnum].thread_id, &res);
		if (ret != 0)
			printf("Erro pthread_join\n");
		
		printf("Thread %d finalizada\n", tinfo_prod[tnum].thread_num);
	}

	/* Aguarda todas as threads finalizarem */
	for (int tnum = 0; tnum < qtd_cons; tnum++) {
		ret = pthread_join(tinfo_cons[tnum].thread_id, &res);
		if (ret != 0)
			printf("Erro pthread_join\n");
		
		printf("Thread %d finalizada\n", tinfo_cons[tnum].thread_num);
	}
	
	/* Imprime resultado da conta */
	printf("Balanco final da conta: %5f", conta_magica->actual_amount);
			
	return 0;
}
