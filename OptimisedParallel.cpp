/**
 * Parallel program to perform matrix-matrix multiplication
 *
 * To run this program:
 * 	(compile): g++ -O2 -fopenmp OptimisedParallel.cpp -o optimisedparallel
 * 	(run): optimisedparallel
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

/*A method to get the transpose of a given matrix*/
void getTranspose(double** matrix,int size){
    double temp;
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){        //mat[i][j] = mat[j][i]
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

/*A method to perfrom matrix multiplication on given two matrices*/
double multiplyMatrices(double **matA, double **matB, int size){
    double** resMat = initializeMatrix(size);

    double startTime;
    double endTime;

    getTranspose(matB,size);                    //Get transpose of matB

    double* arrA = (double*)malloc(sizeof(double)*size*size);
    double* arrB = (double*)malloc(sizeof(double)*size*size);
#pragma omp parallel for
    for(int i = 0; i<size; i++) {
        for (int j = 0; j < size; j++) {          // Flatting two matrices A and B
            int id = i*size + j;
            arrA[id] = matA[i][j];
            arrB[id] = matB[i][j];
        }
    }

    GET_TIME(startTime);                       //Start clock
#pragma omp parallel for
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            double temp = 0.0;
            for (int cur = 0; cur < size; cur+=5) {
                temp += arrA[row*size + cur] * arrB[col*size + cur];        // Reduce loop iterations so that reduce the amount of jump operations
                temp += arrA[row*size + cur+1] * arrB[col*size + cur+1];
                temp += arrA[row*size + cur+2] * arrB[col*size + cur+2];
                temp += arrA[row*size + cur+3] * arrB[col*size + cur+3];
                temp += arrA[row*size + cur+4] * arrB[col*size + cur+4];
            }
            resMat[row][col] = temp;
        }
    }

    GET_TIME(endTime);//End clock
    double duration = endTime - startTime;//Get duration in  seconds

    delete  matA;     //Free the memory allocated for matA
    delete  matB;     //Free the memory allocated for matB
    delete  resMat;   //Free the memory allocated for resMat

    return duration;

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
    double sum=0,sqSum=0;
    double* temp =  new double[size];
    for (int i = 0; i < size; i++) {
        sqSum += runningTimes[i]*runningTimes[i];
    }
    variance = (sqSum/size) - mean*mean; // sample variance = Sum(x^2)/n - mean^2
    sd = sqrt(variance);                 // sd = variance^0.5
    return sd;
}

/*A method to calculate required rounds when mean and sd is given for 95% confidence*/
double getRounds(double mean,double sd){
    double n = (100*1.96*sd)/(5.0*mean);
    return n*n;
}

int main(int argc, const char* argv[]) {
    int rounds;
    int size;
    cout << "------------------------Optimized Parallel Approach-----------------------------\n"<<endl;
    cout << "Matrix Dimention:";
    cin >> size;            //Get matrix dimention
    cout << "No of Rounds:";
    cin >> rounds;          //Get number of rounds
    cout << "\n-------------------------Time Values------------------------------------\n"<<endl;
    double* runningTimes = new double[rounds];
    double mean = 0, sd =0, requiredRounds=0;
    for(int i=0;i< rounds;i++)
    {
        double duration = matrixMultiply(size);
        runningTimes[i] = duration;
        cout << duration<< endl;
    }
    mean = getMean(runningTimes,rounds);
    cout<<"\nmean = "<<mean<<endl;
    sd = getSD(runningTimes,rounds,mean);
    cout<<"sample SD = "<<sd<<endl;
    requiredRounds = getRounds(mean,sd);
    cout<<"required rounds  = "<<requiredRounds<<endl;
    return 0;
}