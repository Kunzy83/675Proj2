#include <mpi.h>
#include <iostream>
#include <fstream>

struct dataElement
{
  int modelNum;
  int date;
  char custType;
  float numSales;
};


int main (int argc, char **argv)
{
	MPI_Init(&argc, &argv);

	int rank, communicatorSize;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // unique ID for this process; 0<=rank<N where:
	MPI_Comm_size(MPI_COMM_WORLD, &communicatorSize); // N=communicatorSize (size of "world")

  //create new communicator world for work
  MPI_Comm* workComm;
  MPI_Comm_split(MPI_COMM_WORLD, worldGroup);

  if(rank == 0)
  {
    //report errors in command line arguements

    //read in data to dataElement array
    std::ifstream inputFile;
    inputFile.open ("test.txt");
    if(!inputFile.is_open())
    {
      std::cout<<"File failed to open"<<std::endl;
      //end program
      return 0;
    }
    int numLines = 0;
    inputFile >> numLines;
    if(numLines <= 0)
    {
      std::cout<<"Invalid number of lines given at top of file... killing program"<<std::endl;
      return 0;
    }

    int numModels = 0;
    inputFile >> numModels;
    if(numModels <= 0)
    {
      std::cout<<"Invalide Model number at top of file... killing program"<<std::endl;
      return 0;
    }

    dataElement dataArray[numLines];

    for(int i = 0; i < numLines; i++)
    {
      int modelNum, date;
      char custType;
      float numSales;
      inputFile >> modelNum >> date >> custType >> numSales;
      dataArray[i] = {modelNum, date, custType, numSales};

    }
    // for(int i = 0; i < numLines; i++)
    // {
    //   std::cout<< dataArray[i].modelNum << " "<< dataArray[i].date << " "<< dataArray[i].custType << " "<< dataArray[i].numSales <<std::endl;
    // }

    //scatter dataElement array to processes of new communicator world

    //use MPI_Reduce to combine the results

    //Produce final report
  }
  // else if (I am error reporting rank)
  // {
  //   //receive an error message
  //
  //   //report the error message using only the tag
  //
  //   //print the entire record exactly as received and report the rank that sent the error
  // }
  else
  {
    //receive slice of data from dataElement array

    //for each record
      //check for error, if so then send report with correct tag to error reporting rank
      //if no error, accumulate data

    //use some version of MPI_Reduce to send back data
  }

  MPI_Finalize();
  return 0;
}
