/**
 * Parallel program to perform matrix-matrix multiplication
 *
 * To run this program:
 * 	(compile): g++ -std=c++11 Parallel.cpp -o parallel
 * 	(run): parallel
 *
 *
 */

#include <iostream>
#include <random>
#include <chrono>
#include <math.h>
#include "timer.h"

using namespace std::chrono;
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,8);//The distribution in range 1-8

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = dis(gen);
        }
    }
}

/*A method to perfrom matrix multiplication on given two matrices*/
double multiplyMatrices(double **matA, double **matB, int size){
    double** resMat = initializeMatrix(size);

    double startTime = 0.0;
    double endTime = 0.0;

    //high_resolution_clock::time_point start = high_resolution_clock::now();//Start clock
    GET_TIME(startTime);
#pragma omp parallel for
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            resMat[row][col] = 0.0;
            for (int cur = 0; cur < size; cur++) {
                resMat[row][col] += matA[row][cur] * matB[cur][col];
            }
        }
    }


    //high_resolution_clock::time_point end = high_resolution_clock::now(); //End clock

    GET_TIME(endTime);
    //double duration = (double)duration_cast<nanoseconds>( end - start ).count()/1000000;   //Get duration in nano seconds
    double duration = endTime - startTime;

    delete  matA;     //Free the memory allocated for matA
    delete  matB;     //Free the memory allocated for matB
    delete  resMat;   //Free the memory allocated for resMat

    return duration;

}

/*This method do a rondom matrix multiplication for two matrices of given size*/
double matrixMultiply(int size){

    double** matA = initializeMatrix(size);    //Initialize matrix A
    double** matB = initializeMatrix(size);    //Initialize matrix B
    populateMatrix(matA , size);         //Populate matrix A with random values
    populateMatrix(matB, size);          //Populate matrix B with random values
    return multiplyMatrices(matA,matB, size); //Call the function to multiply the two matrices

}


int main(int argc, const char* argv[]) {
    int rounds;
    int size;
    cout << "No of Rounds:";
    cin >> rounds;
    cout << "Matrix size:";
    cin >> size;
    for(int i=0;i< rounds;i++)
    {
        double duration = matrixMultiply(size);
        cout << duration<< endl;
    }
    return 0;
}