#include <mpi.h>
#include <iostream>
#include <fstream>

static const int NUM_DATAELEMENT_FIELDS = 4;

struct dataElement
{
  int modelNum;
  int date;
  char custType;
  float numSales;
};

void defineStructDataElementToMPI(MPI_Datatype* typeToBeCreated)
{
  //the  data we need to define for MPI to create a type for "dataElement"
  int blklen[NUM_DATAELEMENT_FIELDS];
  // MPI_Aint is commonly defined as "ptrdiff_t", a C++11 type for
	// holding pointersmodelNum and (especially) differences between pointers
	// that may be negative. "ptrdiff_t" works on all platforms
	// regardless of pointer size.
  MPI_Aint displ[NUM_DATAELEMENT_FIELDS];
	MPI_Datatype types[NUM_DATAELEMENT_FIELDS];

  //all block lengths are 1
  blklen[0] = 1;
  blklen[1] = 1;
  blklen[2] = 1;
  blklen[3] = 1;

  // Compute offsets to the various fields of a dataElement. (MPI calls these
	// "displacements".)
	displ[0] = 0; // 'modelNum' starts at the beginning

	// We use "sample" to compute the remaining displacements:
	dataElement sample;

	MPI_Aint base; // base address of a given dataElement instance
	MPI_Get_address(&sample.modelNum, &base);

	// base address of successive fields of a given dataElement instance:
	MPI_Aint oneField;

	MPI_Get_address(&sample.date, &oneField);
	displ[1] = oneField - base; // offset (displacement) to 'date'

	MPI_Get_address(&sample.custType, &oneField);
	displ[2] = oneField - base; // offset (displacement) to 'custType'

	MPI_Get_address(&sample.numSales, &oneField);
	displ[3] = oneField - base; // offset (displacement) to 'numSales'

	types[0] = MPI_INT; // type of modelNum
	types[1] = MPI_INT;   // type of date
	types[2] = MPI_CHAR; // base type of custType
	types[3] = MPI_FLOAT;  // type of numSales

	MPI_Type_create_struct (NUM_DATAELEMENT_FIELDS, blklen, displ, types, typeToBeCreated);
	MPI_Type_commit(typeToBeCreated);
}

int main (int argc, char **argv)
{
	MPI_Init(&argc, &argv);

	int rank, communicatorSize;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // unique ID for this process; 0<=rank<N where:
	MPI_Comm_size(MPI_COMM_WORLD, &communicatorSize); // N=communicatorSize (size of "world")

  //The following must be executed in all processes:
  MPI_Datatype dataElementStruct;
  defineStructDataElementToMPI(&dataElementStruct);

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

    //figure out if the size if the array can be evenly split among processes in WORK_COMM
    if(numLines > communicatorSize)
    {
      //there are less processes than lines to compute on
      //check to see if the records divide evenly on number of processes
      if(numLines%communicatorSize == 0)
      {
        //records do divide evenly, nothing tricky to do here
        //send off work with Scatter call to WORK_COMM

        int result = MPI_Scatter(&dataArray, numLines/communicatorSize, dataElementStruct, , );
      }
      else
      {
        //records do not divide evenly, TODO: figure out how to properly send
      }
    }
    else
    {
      //there are more processes than records to compute on
      //will send each process one record until there are no more lines, then all other processes get no work to do
      //TODO
    }

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

    //use MPI_Reduce 3 times for each wanted record to send back data to rank 0
  }

  MPI_Finalize();
  return 0;
}
