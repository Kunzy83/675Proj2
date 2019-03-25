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

  //create new communicator world for work by splitting into two sub Communicators using coloring
  //Error reporting rank will ignore this communicator as it will create one with only itself in it
  //Erro reporting rank will be rank 1;
  int color = 0;
  if(rank == 1)
  {
    color = 1;
  }
  MPI_Comm WORK_COMM;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &WORK_COMM);
  //may use these later
  int workRank, workCommunicatorSize;
  MPI_Comm_rank(WORK_COMM, &workRank); // unique ID for this process; 0<=rank<N where:
	MPI_Comm_size(WORK_COMM, &workCommunicatorSize); // N=communicatorSize (size of "world")

  if(rank == 0)
  {
    //report errors in command line arguements

    //read in data to dataElement array
    std::ifstream inputFile;
    inputFile.open (argv[1]);
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

    //figure out if the size if the array can be evenly split among processes in WORKCOMM?
    

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
