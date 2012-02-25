/* Reading Galacticus Output */
/* Markus Haider 21.02.2012                  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>

#include "dataStructs.h"

int getTimeTable(char *, struct timeStruct *);
int getNumberOfTimeSteps(char *, int *);
int getOutputGroup(struct groupStruct *,int);
int getNumberOfHalos(struct groupStruct *);
int fillNodeArray(struct groupStruct *, struct nodeStruct *);
int writeTimeTable(char *, struct timeStruct *);
int writeNodeData(char *, struct nodeStruct *);

int main(int argc, char *argv[]) {

  /* specify the galacticus output filename */
  //char * filename = "/media/alfons/galacticus_output/r692/galacticus.hdf5";
  char * filename = "/home/james/galacticus_output/galacticus.hdf5";

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
  timeTable.nTimeSteps = nTimeSteps;
  timeTable.counter = 0;
  /* allocate the memory for the timeTable */
  timeTable.indexVector = (int *   ) malloc(nTimeSteps*sizeof(int));
  timeTable.timeGyr     = (double *) malloc(nTimeSteps*sizeof(double));
  timeTable.scaleFactor = (double *) malloc(nTimeSteps*sizeof(double));
  timeTable.redshift    = (double *) malloc(nTimeSteps*sizeof(double));
  timeTable.nHalos      = (int    *) malloc(nTimeSteps*sizeof(int));
  timeTable.flagValid   = (int    *) malloc(nTimeSteps*sizeof(int));
  err = getTimeTable(filename,&timeTable);
  if (err !=0) {
    printf("Encountered error in getTimeTable\n");
    return -1;
  }
  /* print the timeTable */
  /* int i; */
  /* for(i=0;i<nTimeSteps;i++) { */
  /*   printf("i: %i %i timeGyr: %g scale: %g redshift: %g\n", */
  /* 	   i,timeTable.indexVector[i],timeTable.timeGyr[i], timeTable.scaleFactor[i],timeTable.redshift[i]); */
  /* } */
  writeTimeTable("output.hdf5",&timeTable);

  /* get a specified output group */
  struct groupStruct outputGroup;
  outputGroup.file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  if(outputGroup.file_id < 0) {
    printf("Error opening HDF5 file\n");
    return -1;
  }
  err = getOutputGroup(&outputGroup,1);
  if (err !=0) {
    printf("Encountered error in getOutputGroup\n");
    return -1;
  }
  /* check how many nodes we have in that group */
  outputGroup.nHalos = getNumberOfHalos(&outputGroup);
  printf("Number of Halos in selected tstep %i\n",outputGroup.nHalos);

  /* make array of the nodes */
  struct nodeStruct * nodeArray;
  nodeArray = (struct nodeStruct *)malloc(outputGroup.nHalos*sizeof(struct nodeStruct));
  /* fill array with the node data */
  err = fillNodeArray(&outputGroup,nodeArray);
  if (err !=0) {
    printf("Encountered error in fillNodeArray\n");
    return -1;
  }

  /* /\* check the node array (for debugging purposes) *\/ */
  /* int aIndex = 0; */
  /* printf("Node %i:\n",aIndex); */
  /* printf("nodeIndex:       %i\n",nodeArray[aIndex].nodeIndex); */
  /* printf("positionX:       %g\n",nodeArray[aIndex].positionX); */
  /* printf("positionY:       %g\n",nodeArray[aIndex].positionY); */
  /* printf("positionZ:       %g\n",nodeArray[aIndex].positionZ); */
  /* printf("outflowedMetals: %g\n",nodeArray[aIndex].outflowedMetals); */

  /* aIndex = 10; */
  /* printf("Node %i:\n",aIndex); */
  /* printf("nodeIndex:       %i\n",nodeArray[aIndex].nodeIndex); */
  /* printf("positionX:       %g\n",nodeArray[aIndex].positionX); */
  /* printf("positionY:       %g\n",nodeArray[aIndex].positionY); */
  /* printf("positionZ:       %g\n",nodeArray[aIndex].positionZ); */
  /* printf("outflowedMetals: %g\n",nodeArray[aIndex].outflowedMetals); */

  /* aIndex = outputGroup.nHalos-1; */
  /* printf("Node %i:\n",aIndex); */
  /* printf("nodeIndex:       %i\n",nodeArray[aIndex].nodeIndex); */
  /* printf("positionX:       %g\n",nodeArray[aIndex].positionX); */
  /* printf("positionY:       %g\n",nodeArray[aIndex].positionY); */
  /* printf("positionZ:       %g\n",nodeArray[aIndex].positionZ); */
  /* printf("outflowedMetals: %g\n",nodeArray[aIndex].outflowedMetals); */

  /* close the group */
  H5Gclose(outputGroup.groupId);
  H5Fclose(outputGroup.file_id);

  /* free the timeTable */
  free(timeTable.indexVector);
  free(timeTable.scaleFactor);
  free(timeTable.timeGyr);
  free(timeTable.redshift);

  printf("completed tasks on the galacticus file\n");

  return 0;

}
