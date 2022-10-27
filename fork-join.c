#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

void *child(void *arg) {
    printf("child\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    Pthread_create(&p, NULL, child, NULL);
    printf("parent: end\n");
    return 0;
	}