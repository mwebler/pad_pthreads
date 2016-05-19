# pad_pthreads

Compilar:

gcc -o soma_vetor.exe soma_vetor.c -lpthread

gcc -o conta_magica.exe conta_magica.c -lpthread


Executar:

soma_vetor.exe QTD_THREADS TAM_VETOR

conta_magica.exe QTD_CONSUMIDOR QTD PRODUTOR



OBS: Dependendo do número de consumidores ou produtores (se um for muito maior que o outro) uma thread pode ficar travada no wait para sempre por não ter mais ninguém consumindo ou produzindo.
