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
int compareTime(const void *, const void *);

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

  for(i=0;i<nTimeSteps;i++) {
    printf("i: %i timeGyr: %g scale: %g redshift: %g group: %s\n",
  	   i,timeTable.tStep[i].timeGyr, timeTable.tStep[i].scaleFactor,timeTable.tStep[i].redshift,timeTable.tStep[i].outputGroupName);
  }

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
