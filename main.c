/* Reading Galacticus Output                 */
/* Markus Haider 21.02.2012                  */
/* alt: Harald Höller 27.07.2013             */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>

#include "dataStructs.h"

int getTimeTable(char *, struct timeStruct *);
int getNumberOfTimeSteps(char *, int *);
int getNumberOfHalos(struct groupStruct *);
int fillNodeArray(struct groupStruct *, struct nodeStruct *);
int writeTimeTable(char *, struct timeStruct *);
int writeNodeData(char *, char * ,char *, int, int);
int compareTime(const void *, const void *);

int main(int argc, char *argv[]) {

  /* specify the galacticus output filename */
  //char * filename = "/media/alfons/galacticus_output/r692_alltime/galacticus.hdf5";
  //char * filename = "/media/alfons/galacticus_output/r692/galacticus.hdf5";
  //char * filename = "/home/markus/Arbeitsfläche/read_galacticus/galacticus.hdf5";
  //char * filename = "/home/james/galacticus_output/galacticus.hdf5";

  if (argc != 2) {
  printf("Provide input file as argument \n");
  return 0;
  }
  char * filename; 
  filename = argv[1];

  int err;
  int nTimeSteps;
  /* Get the number of timeSteps in the Galacticus File */
  err = getNumberOfTimeSteps(filename,&nTimeSteps);
  if (err !=0) {
    printf("Encountered error in getNumberOfTimeSteps\n");
    return -1;
  }
  printf("%s contains %i timesteps\n",filename,nTimeSteps);
  
  /* Get the timeTable of the Outputs */
  struct timeStruct timeTable;
  timeTable.tStep = (struct timeStep *)malloc(nTimeSteps*sizeof(struct timeStep));
  timeTable.nTimeSteps = nTimeSteps;
  timeTable.counter = 0;
  timeTable.validTimeSteps = 0;
  err = getTimeTable(filename,&timeTable);
  if (err !=0) {
    printf("Encountered error in getTimeTable\n");
    return -1;
  }
  /* sort the timeTable */
  qsort(timeTable.tStep,nTimeSteps,sizeof(struct timeStep),compareTime);
  /* compute how many valid timesteps we have */
  int i;
  for(i=0;i<nTimeSteps;i++) {
    if(timeTable.tStep[i].flagValid==0) {
      timeTable.validTimeSteps++;
    }
  }
  printf("%s contains %i valid timesteps\n",filename,timeTable.validTimeSteps);

  /* print the timetable */
  /* for(i=0;i<nTimeSteps;i++) { */
  /*   printf("i: %i timeGyr: %g scale: %g redshift: %g group: %s\n", */
  /* 	   i,timeTable.tStep[i].timeGyr, timeTable.tStep[i].scaleFactor, */
  /* 	   timeTable.tStep[i].redshift,timeTable.tStep[i].outputGroupName); */
  /* } */

  writeTimeTable("output.hdf5",&timeTable);

  /* -------------- writing the node data -------------- */

  /* loop over the valid timesteps */
  int invalidSteps = timeTable.nTimeSteps-timeTable.validTimeSteps;
  for(i=0;i<timeTable.validTimeSteps;i++) {
    /* access groups over groupname in timetable */
    writeNodeData(filename, "output.hdf5",timeTable.tStep[i+invalidSteps].outputGroupName,timeTable.tStep[i+invalidSteps].nHalos,i);
  }

  /* free the timeTable */
  free(timeTable.tStep);

  printf("completed tasks on the galacticus file\n");

  return 0;

}


int compareTime(const void * t1, const void * t2) {
  double tmp1 = (*(struct timeStep *)t1).timeGyr;
  double tmp2 = (*(struct timeStep *)t2).timeGyr;
  if(tmp1>tmp2) {
    return 1;
  } else if (tmp2>tmp1) {
    return -1;
  } else {
    return 0;
  }
}
