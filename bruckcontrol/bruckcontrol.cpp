#include <chrono>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include <mpi.h>

#include "../common/bruck.cpp"
#include "../common/error-catch.cpp"

// Distribute each process rank using MPI Bruck
int main(int argc, char **argv)
{
  int nodes = atoi(argv[1]);
  int ppn = atoi(argv[2]);
  int radix = atoi(argv[3]);

  // Initialize MPI
  MPICHECK(MPI_Init(&argc, &argv));

  // Set MPI size and rank
  int size;
  int rank;
  MPICHECK(MPI_Comm_size(MPI_COMM_WORLD, &size));
  MPICHECK(MPI_Comm_rank(MPI_COMM_WORLD, &rank));

  const int start_bytes = 16;
  const int stop_bytes = 4096;

  // Allocated variables
  char *send_data;
  char *recv_data;

  // Warm-up loop
  for (int i = 0; i < 5; i++)
  {
    for (int count = start_bytes; count <= stop_bytes; count *= 2)
    {
      // Send and recieve buffers must be the same size
      const int buffer_size = size * count;
      send_data = new char[buffer_size];
      recv_data = new char[buffer_size];
      for (int j = 0; j < 20; j++)
      {
        for (int j = 0; j < buffer_size; j++)
        {
          send_data[j] = rank % 64;
          recv_data[j] = 0;
        }
        uniform_radix_r_bruck(radix, (char *)send_data, count, MPI_CHAR, (char *)recv_data, count, MPI_CHAR, MPI_COMM_WORLD);
      }
    }
  }

  std::ofstream log;
  if (rank == 0)
  {
    std::ostringstream filename;
    filename << size << "-proc-" << nodes << "-node-" << ppn << "-ppn.log";
    log.open(filename.str(), std::ios_base::app);
    log << "Bruckcontrol,";
  }

  // Benchmark loop
  const int num_executions = 50;
  for (int count = start_bytes; count <= stop_bytes; count *= 2)
  {
    // Send and recieve buffers must be the same size
    const int buffer_size = size * count;
    send_data = new char[buffer_size];
    recv_data = new char[buffer_size];

    std::vector<float> times(num_executions);
    for (int j = 0; j < num_executions; j++)
    {
      // Reset buffers
      for (int k = 0; k < buffer_size; k++)
      {
        send_data[k] = rank % 64;
        recv_data[k] = 0;
      }
      MPICHECK(MPI_Barrier(MPI_COMM_WORLD));

      // Perform all to all
      auto start = std::chrono::high_resolution_clock::now();
      uniform_radix_r_bruck(radix, (char *)send_data, count, MPI_CHAR, (char *)recv_data, count, MPI_CHAR, MPI_COMM_WORLD);
      auto stop = std::chrono::high_resolution_clock::now();

      // Compute elapsed time
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
      const float localElapsedTime = duration.count();

      MPI_Barrier(MPI_COMM_WORLD);
      float elapsedTime;
      MPI_Reduce(&localElapsedTime, &elapsedTime, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
      if (rank == 0)
      {
        times[j] = localElapsedTime;
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
      float sum = 0;
      for (int i = 0; i < num_executions; i++)
      {
        sum += times[i];
      }
      float average = sum / num_executions;

      log << average << ",";
    }

    // Free allocated memory
    delete[] send_data;
    delete[] recv_data;
  }
  log << std::endl;
  log.close();

  // Finalize MPI
  MPICHECK(MPI_Finalize());
  return 0;
}