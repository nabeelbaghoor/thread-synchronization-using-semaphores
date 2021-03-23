#include <iostream>
#include <semaphore.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
using namespace std;
char *buffer = new char[100]; //shared memory
sem_t semaphore1;
sem_t mutex;
void *Write(void *param)
{
    cout << "Enter Input:\n";
    cin.ignore();
    cin >> buffer;
    cout << "input Read!\n\n";
    sem_post(&mutex);
    pthread_exit(NULL); //mb
}
void *Read(void *param)
{
    sem_wait(&semaphore1);
    cout << "Thread ID = " << pthread_self() << endl;
    char *output = new char[strlen(buffer) + 1];

    //sleep(1);
    strcpy(output, buffer);
    cout << "Data Read!\n";
    cout << "Printing Data...\n";
    cout << output << endl
         << endl;
    sem_post(&semaphore1);
    pthread_exit(NULL); //mb
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "No of Readers Thread Not Specified!\n";
        exit(1); //mb
    }

    int N = atoi(argv[1]);

    sem_init(&semaphore1, 1, 1); //1 means semaphore is used for process synchronization
    sem_init(&mutex, 1, 0);      //1 means semaphore is used for process synchronization

    pthread_t writer;
    pthread_t reader[N];
    while (true) //infinite
    {
        if (pthread_create(&writer, NULL, &Write, NULL) == -1)
        {
            cout << "Thread Creation Failed!" << endl;
            return 0; //mb
        }
        sem_wait(&mutex);

        for (int i = 0; i < N; i++)
        {

            if (pthread_create(&reader[i], NULL, &Read, NULL) == -1)
            {
                cout << "Thread Creation Failed!" << endl;
                return 0; //mb
            }
        }

        pthread_join(writer, NULL); //mb
        for (int i = 0; i < N; i++)
            pthread_join(reader[i], NULL); //mb

        //sem_post(&mutex);
    }

    sem_destroy(&semaphore1);
    sem_destroy(&mutex);
    system("pause");
}
//done