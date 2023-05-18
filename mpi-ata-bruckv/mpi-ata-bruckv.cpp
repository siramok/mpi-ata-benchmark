#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <mpi.h>

#include "../common/error-catch.cpp"
#include "../common/twophase.cpp"

// Distribute each process rank using MPI_Alltoallv
int main(int argc, char **argv)
{

  // Initialize MPI
  MPICHECK(MPI_Init(&argc, &argv));

  // Set MPI size and rank
  int size;
  int rank;
  MPICHECK(MPI_Comm_size(MPI_COMM_WORLD, &size));
  MPICHECK(MPI_Comm_rank(MPI_COMM_WORLD, &rank));

  const int start_bytes = 16;
  const int stop_bytes = 2048;

  // Allocated variables
  char *send_data;
  char *recv_data;

  int sendcounts[size];
  int recvcounts[size];
  int displs[size];

  // Warm-up loop
  for (int count = start_bytes; count <= stop_bytes; count *= 2)
  {
    // Send and recieve buffers must be the same size
    const int buffer_size = size * count;
    send_data = new char[buffer_size];
    recv_data = new char[buffer_size];

    for (int i = 0; i < size; ++i)
    {
      sendcounts[i] = count;
      recvcounts[i] = count;
      displs[i] = i * count;
    }

    for (int j = 0; j < 20; j++)
    {
      for (int j = 0; j < buffer_size; j++)
      {
        send_data[j] = rank % 64;
        recv_data[j] = 0;
      }
      twophase_bruck_alltoallv(send_data, sendcounts, displs, MPI_CHAR, recv_data, recvcounts, displs, MPI_CHAR, MPI_COMM_WORLD);
    }
  }

  // Benchmark loop
  const int num_executions = 1000;
  for (int count = start_bytes; count <= stop_bytes; count *= 2)
  {
    // Send and recieve buffers must be the same size
    const int buffer_size = size * count;
    send_data = new char[buffer_size];
    recv_data = new char[buffer_size];

    for (int i = 0; i < size; ++i)
    {
      sendcounts[i] = count;
      recvcounts[i] = count;
      displs[i] = i * count;
    }

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
      twophase_bruck_alltoallv(send_data, sendcounts, displs, MPI_CHAR, recv_data, recvcounts, displs, MPI_CHAR, MPI_COMM_WORLD);
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

      std::ofstream log;
      log.open("run.log", std::ios_base::app);
      log << "[MPI_Alltoallv] " << size << " processes sending " << count << " bytes each: " << average << " us avg of " << num_executions << " executions" << std::endl;
      log.close();
    }

    // Free allocated memory
    delete[] send_data;
    delete[] recv_data;
  }

  // Finalize MPI
  MPICHECK(MPI_Finalize());
  return 0;
}