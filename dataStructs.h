#ifndef DATASTRUCT_H
#define DATASTRUCT_H


struct timeStep {
  double timeGyr;
  double scaleFactor;
  double redshift;
  char outputGroupName[100];
  int nHalos;
  /* it is possible that early output groups do not contain */
  /* actual nodeData. We check for this, and set the valid flag */
  /* to 0 if the group contains data, and to -1 if it does not */
  int flagValid;
};

struct timeStruct {
  int counter;
  int nTimeSteps;
  int validTimeSteps;
  hid_t file_id;
  struct timeStep * tStep;
};

struct groupStruct {
  int counter;
  char outputGroupName[100];
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
