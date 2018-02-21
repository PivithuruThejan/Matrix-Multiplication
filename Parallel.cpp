/**
 * Parallel program to perform matrix-matrix multiplication
 *
 * To run this program:

 * 	(compile): g++ -fopenmp Parallel.cpp -o parallel

 * 	(run): parallel
 *
 *
 */

#include <iostream>
#include <random>

#include <math.h>
#include <omp.h>
#include "timer.h"

using namespace std;

/*A method to initialize a matrix)*/
double** initializeMatrix(int size){
    double** mat = new double*[size];
    for (int i = 0; i < size; i++) {
        mat[i] = new double[size];
    }
    return mat;
}

/*A method to populate a matrix with random values*/
void populateMatrix(double** matrix, int size){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (rand()%1000)/10.0;
        }
    }
}

/*A method to perfrom matrix multiplication on given two matrices*/
double multiplyMatrices(double **matA, double **matB, int size){
    double** resMat = initializeMatrix(size);


    double startTime;
    double endTime;
    GET_TIME(startTime);//Start clock
    #pragma omp parallel for
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            resMat[row][col] = 0.0;
            for (int cur = 0; cur < size; cur++) {
                resMat[row][col] += matA[row][cur] * matB[cur][col];
            }
        }
    }


    GET_TIME(endTime);//End clock

    double duration = endTime - startTime;

    delete  matA;     //Free the memory allocated for matA
    delete  matB;     //Free the memory allocated for matB
    delete  resMat;   //Free the memory allocated for resMat


    return duration;// Get duration in seconds

}

/*This method do a random matrix multiplication for two matrices of given size*/

double matrixMultiply(int size){

    double** matA = initializeMatrix(size);    //Initialize matrix A
    double** matB = initializeMatrix(size);    //Initialize matrix B
    populateMatrix(matA, size);                //Populate matrix A with random values
    populateMatrix(matB, size);                //Populate matrix B with random values
    return multiplyMatrices(matA,matB, size);  //Call the function to multiply the two matrices

}

/*A method to get the mean when an array of running times is given*/
double getMean(double* runningTimes, int size){
    double sum = 0;
    double mean = 0;
    for(int i=0;i<size;i++){         //A loop to add up the values
        sum += runningTimes[i];
    }

    mean = sum / size;
    return mean;
}

/*A method to calculate the standard deviation when the distribution and the mean are given*/
double getSD(double* runningTimes, int size, double mean){
    double variance = 0, sd =0;
    double* temp =  new double[size];
    for (int i = 0; i < size; i++) {
        temp[i] = runningTimes[i] - mean;
        temp[i] = pow(temp[i], 2.0); //to get the (x-average)……2
        variance += temp[i];
    }
    variance = variance / (size-1); // sample variance
    sd = sqrt(variance);
}


int main(int argc, const char* argv[]) {
    int rounds;
    int size;
    cout << "No of Rounds:";
    cin >> rounds;
    cout << "Matrix size:";
    cin >> size;
    double* runningTimes = new double[rounds];
    double mean = 0, sd =0;
    for(int i=0;i< rounds;i++)
    {
        double duration = matrixMultiply(size);
        runningTimes[i] = duration;
        cout << duration<< endl;
    }
    mean = getMean(runningTimes,rounds);
    cout<<"\nmean = "<<mean<<endl;
    sd = getSD(runningTimes,rounds,mean);//Get the standard deviation for the initial 10 samples
    cout<<"sample SD = "<<sd<<endl;
    return 0;
}