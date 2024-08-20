#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t wrt;           // Semaphore to control access to shared data (writers)
pthread_mutex_t mutex;  // Mutex to protect reader_count

int reader_count = 0;
int data = 0;  // The shared data

void *writer(void *arg) {
    int writer_id = *(int *)arg;
    
    // Writer process
    sem_wait(&wrt);  // Request access to shared data
    
    // Critical section: writing the data
    data++;
    printf("Writer %d is writing data = %d\n", writer_id, data);
    sleep(1);  // Simulate writing time

    sem_post(&wrt);  // Release access to shared data
    
    return NULL;
}

void *reader(void *arg) {
    int reader_id = *(int *)arg;
    
    // Reader process
    pthread_mutex_lock(&mutex);  // Lock access to reader_count
    reader_count++;
    if (reader_count == 1) {
        sem_wait(&wrt);  // First reader locks the writer
    }
    pthread_mutex_unlock(&mutex);  // Unlock access to reader_count

    // Critical section: reading the data
    printf("Reader %d is reading data = %d\n", reader_id, data);
    sleep(1);  // Simulate reading time

    pthread_mutex_lock(&mutex);  // Lock access to reader_count
    reader_count--;
    if (reader_count == 0) {
        sem_post(&wrt);  // Last reader unlocks the writer
    }
    pthread_mutex_unlock(&mutex);  // Unlock access to reader_count
    
    return NULL;
}

int main() {
    pthread_t read_threads[5], write_threads[3];
    int reader_ids[5], writer_ids[3];

    // Initialize the semaphores and mutex
    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    // Create writer threads
    for (int i = 0; i < 3; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&write_threads[i], NULL, writer, &writer_ids[i]);
    }

    // Create reader threads
    for (int i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&read_threads[i], NULL, reader, &reader_ids[i]);
    }

    // Wait for writer threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(write_threads[i], NULL);
    }

    // Wait for reader threads to finish
    for (int i = 0; i < 5; i++) {
        pthread_join(read_threads[i], NULL);
    }

    // Destroy semaphores and mutex
    sem_destroy(&wrt);
    pthread_mutex_destroy(&mutex);

    return 0;
}
