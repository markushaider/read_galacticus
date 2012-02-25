#ifndef DATASTRUCT_H
#define DATASTRUCT_H

struct timeStruct {
  int counter;
  int nTimeSteps;
  hid_t file_id;
  int    * indexVector;
  double * timeGyr;
  double * scaleFactor;
  double * redshift;
  int    * nHalos;
  /* it is possible that early output groups do not contain */
  /* actual nodeData. We check for this, and set the valid flag */
  /* to 0 if the group contains data, and to -1 if it does not */
  int    * flagValid;
};

struct groupStruct {
  int counter;
  int index;
  int flagFound; 		/* set to 1 if requested group is found */
  int nHalos;
  hid_t file_id;
  hid_t groupId;
};

struct nodeStruct {
  int nodeIndex;
  double positionX;
  double positionY;
  double positionZ;
  double outflowedMetals;
};


#endif
