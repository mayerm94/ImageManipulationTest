/** CS515 Lab 3
 * File: image.cpp
 * Name: Matt Mayer
 * Section: 01
 * Date: 9/19/2013 
 *
 *Description: image.cpp takes 4 arguments,
 a properly formatted ascii decimal .pmg file,
 *and 3 new filenames for the inverted,rotated,
 *and flipped new images to go.
 *
*/
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <cstdlib>
using namespace std;

/**createMatrix(int,int)******************
 * creates and initializes 2D array of type int and 
 * returns its location.
 */
int** createMatrix(int rows, int cols)
{
	//new 2d array
	int **inMatrix = new int*[rows];
	for(int x=0;x<rows;x++)
	{
		inMatrix[x]=new int[cols];
		//gotta get me some array initialization
		for(int y=0; y<cols;y++)
			inMatrix[x][y]=0;

	}
	//return location on inMatrix
	return inMatrix;
}

/**fillInputMatrix(int**,ifstream,int,int)**
 * given an inputstream and row/col partitioning
 * fill an array inMatrix.
 */
void fillInputMatrix(int** inMatrix,ifstream &inStream,int rows, int cols)
{
	string line,tok;
	int row=0,col=0;
	//while there are still more lines in the instream
	while(getline(inStream,line))
	{
		stringstream ss(line);
		//while tok can be taken from the stream
		while(ss>>tok)
		{
			//if column limit is reached
			if(col==cols)
			{
				row++;
				col=0;
			}
			inMatrix[row][col]=atoi(tok.c_str());
			col++;
		}
	}

}

int** makeNegativeImage(int** inMatrix,int rows, int cols,int maxVal)
{
	int **negMatrix = createMatrix(rows,cols);
	for(int y=0;y<rows;y++)
	{
		for(int x=0;x<cols;x++)
		{//abs(value-max) = inverse color
			negMatrix[y][x]=abs(inMatrix[y][x]-maxVal);
		}
	}
	return negMatrix;
}

int** makeFlipImage(int** inMatrix,int rows, int cols)
{
	int **flipMatrix=createMatrix(rows,cols);
	
	for(int y=0;y<rows;y++)
        {
                for(int x=0;x<cols;x++)
                {
			//opposite end of the row
                        flipMatrix[y][x]=inMatrix[y][(cols-1)-x];
                }
        }
        
	return flipMatrix;

}

int** makeRotatedImage(int** inMatrix,int rows, int cols)
{
	int **rotatedMatrix=createMatrix(cols,rows);
	for(int y=0;y<rows;y++)
        {
                for(int x=0;x<cols;x++)
                {
	//flip row & column
                        rotatedMatrix[x][y]=inMatrix[y][x];
                }
        }

	return rotatedMatrix;
}

void writeToFile
(int** writeMatrix,int rows, int cols,const char* filename,
string fileType, int maxValue)
{	//++++++++++++header++++
	ofstream outFile;
	outFile.open(filename);
	outFile<<fileType<<endl;
	outFile<<cols<<" "<<rows<<endl;
	outFile<<maxValue<<endl;
	//++++++++++++++++++++++
	for(int y=0;y<rows;y++)
	{
		for(int x=0;x<cols;x++)
		{//space dilimited
			outFile<<writeMatrix[y][x]<<" ";
		}
		outFile<<endl;
	}
	outFile.close();
}

/**deleteMatrix(int**,rows,cols)
 * iterate through rows deleting columns
 * delete rows
 */
void deleteMatrix(int** toDelete,int rows,int cols)
{
	for(int x=0;x<rows;x++)
	{
		delete[] toDelete[x];
		toDelete[x]=0;
	}
	delete[] toDelete;
}

int main(int argc, char **argv){

	if(argc != 5)
	{
		cout<<"Error: invalid number of arguments"<<endl;
		return 0;
	}

	const char* inImage = argv[1];
	const char* negImage = argv[2];
	const char* flipImage = argv[3];
	const char* rotateImage = argv[4];
	//file info vars
	string fileType;
	int maxValue;
	string max;
	
	//vars used in getting rows/cols
	int rows =0;
	int cols =0;
	string line;
	string rowcol;
	
	ifstream myInFile;
        myInFile.open(inImage);

        if(!myInFile)
        {
                cout<<"Error: invalid input file"<<endl;
                return 0;
        }

	
	//get first line of file, should be 'P2'
	getline(myInFile,fileType);
	//this line shuold contain image resolution
	getline(myInFile,line);
	stringstream ss(line);
	ss>>rowcol;
	cols = atoi(rowcol.c_str());
	ss>>rowcol;
	rows = atoi(rowcol.c_str());
	//should be max value line
	getline(myInFile,max);
	maxValue=atoi(max.c_str());	
	//fill initial matrix
	int** inputMatrix = createMatrix(rows,cols);
	fillInputMatrix(inputMatrix,myInFile,rows,cols);
	//create negative image file
	int** negMatrix = makeNegativeImage(inputMatrix,rows,cols,maxValue);
	writeToFile(negMatrix,rows,cols,negImage,fileType,maxValue);
	//create flipped image file
	int** flipMatrix = makeFlipImage(inputMatrix,rows,cols);
	writeToFile(flipMatrix,rows,cols,flipImage,fileType,maxValue);
	//create rotated image file
	int** rotateMatrix = makeRotatedImage(inputMatrix,rows,cols);
        writeToFile(rotateMatrix,cols,rows,rotateImage,fileType,maxValue);
	
	//delete all used memory
	deleteMatrix(inputMatrix,rows,cols);
	deleteMatrix(negMatrix,rows,cols);
	deleteMatrix(flipMatrix,rows,cols);
	deleteMatrix(rotateMatrix,rows,cols);

	return 0;

}
