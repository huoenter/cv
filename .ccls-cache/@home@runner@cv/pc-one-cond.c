#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int buffer = -1;
int count  = 0;
int loops  = 1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

void put(int value) {
  assert(count == 0);
  count = 1;
  buffer = value;
}

int get() {
  assert(count == 1);
  count = 0;
  return buffer;
}

void *producer(void *arg) { // fig 30.6
  for (int i = 0; i < loops; i++) {
    pthread_mutex_lock(&mutex);
    while (count == 1) // does "if" work here?
      pthread_cond_wait(&cond, &mutex);
    put(i);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }

	return NULL;
}


void *consumer(void *arg) { // fig 30.6
  for (int i = 0; i < loops; i++) {
    pthread_mutex_lock(&mutex);
    while (count == 0) // does "if" work here?
      pthread_cond_wait(&cond, &mutex);
    int tmp = get();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    printf("%d\n", tmp);
  }

	return NULL;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "usage: pc-one-cond N_consumer N_producer N_loops\n");
	}

	int NC = atoi(argv[1]);
	int NP = atoi(argv[2]);
	loops  = atoi(argv[3]);

	pthread_t consumers[NC], producers[NP];

	for (int i = 0; i < NC; i++)
		pthread_create(&consumers[i], NULL, consumer, NULL);
	for (int i = 0; i < NP; i++)
		pthread_create(&producers[i], NULL, producer, NULL);

	for (int i = 0; i < NC; i++)
		pthread_join(consumers[i], NULL);
	for (int i = 0; i < NP; i++)
		pthread_join(producers[i], NULL);
	
	return 0;
}