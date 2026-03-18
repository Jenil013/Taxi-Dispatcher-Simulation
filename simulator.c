#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "simulator.h"

#include "taxi.c"
#include "display.c"
#include "dispatchCenter.c"

int main() {
  DispatchCenter     ottawaDispatch;

  
  srand(time(NULL));
  ottawaDispatch.numTaxis = 0;
  ottawaDispatch.numRequests = 0;

  // Create a taxi in a random area of the city. 
  Taxi *taxi = (Taxi *) malloc(sizeof(Taxi));
  int randomArea = rand() % NUM_CITY_AREAS;
  taxi->x = AREA_X_LOCATIONS[randomArea];  //Will be placed randomly in one of the 6 city areas
  taxi->y = AREA_Y_LOCATIONS[randomArea];
  taxi->plateNumber = 0;
  taxi->currentArea = randomArea;
  taxi->status = AVAILABLE;
  taxi->pickupArea = UNKNOWN_AREA;
  taxi->dropoffArea = UNKNOWN_AREA;
  taxi->eta = 0;

  ottawaDispatch.taxis[0] = taxi;
  ottawaDispatch.numTaxis = 1;

  // Forking to start new taxi processes
  pid_t taxiPid = fork();
  if (taxiPid == 0) {
    // Child process: run the taxi, then exit
    runTaxi(taxi);
    exit(0);
  }
  // Parent process: store the child's PID
  taxi->pID = taxiPid;
    

  // Spawn a thread to handle incoming requests
  pthread_t requestThread;
  pthread_create(&requestThread, NULL, handleIncomingRequests, &ottawaDispatch);  // Passing the pointer to the Incoming Requests function


  // Spawn a thread to handle display
  pthread_t displayThread;
  pthread_create(&displayThread, NULL, showSimulation, &ottawaDispatch);  // Passing the pointer to the Show Simulation function


  // Wait for the dispatch center thread to complete, from a STOP command
  pthread_join(requestThread, NULL);
  pthread_join(displayThread, NULL);

  // Kill all the taxi processes
  kill(taxiPid);

  // Free the memory
  free(taxi);
  
  printf("Simulation complete.\n");
}
