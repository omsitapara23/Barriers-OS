#include <iostream>
#include <pthread.h>
#include <ctime>
#include <chrono>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

int preL, postL;

int n, k;


FILE *fp;
ifstream infile("inp-params.txt");
ofstream outfile;

int total_time = 0;
int *avg_time;
/*i ma changing pthread*/

pthread_barrier_t my_barrier;

void barrierInit(int N)
{
    pthread_barrier_init(&my_barrier, NULL, N);
}

/*void barrierPoint()
{
    cout << "The incoming baarrrr" << barrierVal << endl;
    sem_wait(&lockVal);
    //mtx.lock();
    barrierVal--;
    if(barrierVal == 0)
    {
        flag = true;
        cout << "All inside" << endl;
    }

    sem_post(&lockVal);
    //mtx.unlock();
    while(/*barrierVal > 0 && !flag);
    //mtx.lock();
    /*sem_wait(&lockVal);
    cout << "Increasing the value" << endl;
    barrierVal++;
    if(barrierVal == n)
        flag = false;
    //mtx.unlock();
    sem_post(&lockVal);
}*/

void *testBarrier(void *param)
{
    int beforeBarrierSleep, afterBarrierSleep;
    int id = (intptr_t)(param);
    for (int barrIter = 0; barrIter < k; barrIter++)
    {
        srand(preL);
        beforeBarrierSleep = rand()%1000 + 1;
        time_t _time = time(0);
		tm *p_time  = localtime(&_time);
        fprintf(fp,"Going to sleep before the %dth barrier invocation time: %d : %d By Thread %d  \n", barrIter + 1,  p_time->tm_min, p_time->tm_sec, id + 1);
        //cout << "Going to sleep before the " << barrIter <<"th barrier invocation time: " << p_time->tm_min << ":" <<  p_time->tm_sec << "By Thread " << id << endl;
        sleep((beforeBarrierSleep)/1000);

        time_t enterTime = time(0);
        tm* e_time = localtime(&enterTime);
        fprintf(fp,"Before the %dth barrier invocation time: %d : %d By Thread %d  \n", barrIter + 1,  e_time->tm_min, e_time->tm_sec, id + 1);
        //cout << "Before the " << barrIter <<"th barrier invocation time: " << e_time->tm_min << ":" <<  e_time->tm_sec << "By Thread " << id << endl;
        avg_time[id] += -1*(enterTime);
        pthread_barrier_wait(&my_barrier);
        //while(flag);
        time_t exitTime = time(0);
        tm* ex_time = localtime(&exitTime);
        fprintf(fp,"After the %dth barrier invocation time: %d : %d By Thread %d  \n", barrIter + 1,  ex_time->tm_min, ex_time->tm_sec, id + 1);
        //cout << "After the " << barrIter <<"th barrier invocation time: " << ex_time->tm_min << ":" <<  ex_time->tm_sec << "By Thread " << id << endl;
        avg_time[id] += exitTime; 

        srand(postL);
        afterBarrierSleep = rand()%100 + 1;
        time_t exs_time = time(0);
		tm *exsl_time  = localtime(&exs_time);
        fprintf(fp,"Going to sleep after the %dth barrier invocation time: %d : %d By Thread %d  \n", barrIter + 1,  exsl_time->tm_min, exsl_time->tm_sec, id + 1);
        //cout << "Going to sleep before exit the " << barrIter <<"th barrier invocation time: " << exsl_time->tm_min << ":" <<  exsl_time->tm_sec << "By Thread " << id << endl;
        sleep((afterBarrierSleep)/1000);
        
    }
}

int main()
{
	fp = fopen("pthread-barr-log.txt", "w+");
	outfile.open("Average-Times.txt", ios::app);
	infile >> n >> k >> preL >> postL;

    barrierInit(n);
    cout << "bye" << endl;
	avg_time = new int[n];

	for(int i = 0; i < n; i++)
		avg_time[i] = 0;

    pthread_t tid; //the thread identifier
	pthread_attr_t attr; //set of thread attributes
	pthread_t threads[n];
	//for loop to set attributes for threads
	for(int i = 0; i < n; i++){
		pthread_attr_init(&attr); // get default attributes

		pthread_create(&threads[i], &attr, testBarrier, (void *)(intptr_t)(i)); //create the thread
	}

	for(int i = 0; i < n; i++){
		//wait for the thread to exit
		pthread_join(threads[i], NULL);
	}

	for(int i = 0; i < n; i++)
	{
		total_time += avg_time[i];
	}

	outfile << "The average time for pthread Barrier is :" << (float)total_time/((float)(n*k)) << endl;
	outfile.close();

	fclose(fp);
	

	return 0;
}