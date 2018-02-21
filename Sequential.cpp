/**
 * Sequential program to perform matrix-matrix multiplication
 *
 * To run this program:
 * 	(compile): g++ -std=c++11 sequential.cpp -o sequential
 * 	(run): sequential
 *
 * 
 */

#include <iostream>
#include <random>
#include <chrono>
#include <math.h>

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
long multiplyMatrices(double **matA, double **matB, int size){
  double** resultMatrix = initializeMatrix(size);

  high_resolution_clock::time_point start = high_resolution_clock::now();//Start clock

  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      resultMatrix[i][j] = 0.0;
      for (int c = 0; c < size; c++) {
        resultMatrix[i][j] += matA[i][c] * matB[c][j];
      }
    }
  }


  high_resolution_clock::time_point end = high_resolution_clock::now(); //End clock

  double duration = (double)duration_cast<nanoseconds>( end - start ).count()/1000000;   //Get duration in nano seconds

  delete  matA;     //Free the memory allocated for matA
  delete  matB;     //Free the memory allocated for matB
  delete  resultMatrix;   //Free the memory allocated for resMat

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