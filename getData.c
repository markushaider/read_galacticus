/* Reading Galacticus Output */
/* Markus Haider 21.02.2012                  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>

#include "dataStructs.h"

herr_t makeTimetable(hid_t, const char *, const H5L_info_t *,struct timeStruct *);

int getNumberOfTimeSteps(char * filename, int * nTimeSteps) {
  hid_t file_id;
  file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  if(file_id < 0) {
    printf("Error opening HDF5 file\n");
    return -1;
  }
  printf("Succesfully Opened file\n");

  /* Opening the outputs group to find out how many outputs we have */
  hid_t gOutputs;
  gOutputs = H5Gopen(file_id,"/Outputs");
  hsize_t nOutputs;
  H5Gget_num_objs(gOutputs,&nOutputs);
  *nTimeSteps = (int)nOutputs;
  H5Gclose(gOutputs);
  H5Fclose(file_id);
  return 0;
}

int getTimeTable(char * filename, struct timeStruct * timeTable) {

  hid_t file_id;
  file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  if(file_id < 0) {
    printf("Error opening HDF5 file\n");
    return -1;
  }
  printf("Succesfully Opened file\n");

  hid_t gOutputs;
  gOutputs = H5Gopen(file_id,"/Outputs");
  (*timeTable).file_id = file_id;
  H5Literate(gOutputs,H5_INDEX_NAME,H5_ITER_NATIVE,NULL,(H5L_iterate_t)makeTimetable,timeTable);

  H5Gclose(gOutputs);
  H5Fclose(file_id);

  return 0;
}

int getOutputGroup(struct groupStruct * outputGroup, char * outputGroupName) {

  sprintf((*outputGroup).outputGroupName,"%s",outputGroupName);
  (*outputGroup).groupId = H5Gopen((*outputGroup).file_id,(*outputGroup).outputGroupName);
  if((*outputGroup).groupId<0) {
    printf("Problems opening group %s\n",(*outputGroup).outputGroupName);
    return -1;
  }
  return 0;

}

int getNumberOfHalos(struct groupStruct * outputGroup) {

  hid_t dset;
  dset = H5Dopen((*outputGroup).groupId,"nodeIndex");
  hid_t dspace;
  dspace = H5Dget_space(dset);
  hssize_t nHalos;
  nHalos = H5Sget_simple_extent_npoints(dspace);
  H5Sclose(dspace);
  H5Dclose(dset);
  return nHalos;
}

int fillNodeArray(struct groupStruct * outputGroup, struct nodeStruct * nodeArray, int nHalos) {

  int err;
  printf("Filling the node array\n");
  int i;
 
  long int intBuffer[nHalos];
  err = getIntData(outputGroup,intBuffer,"nodeIndex");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].nodeIndex = intBuffer[i];
  }

  err = getIntData(outputGroup,intBuffer,"descendentIndex");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].descendentIndex = intBuffer[i];
  }

  /* fill positions */
  double doubleBuffer[nHalos];
  err += getDoubleData(outputGroup,doubleBuffer,"positionPositionX");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].positionX = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"positionPositionY");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].positionY = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"positionPositionZ");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].positionZ = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"positionVelocityX");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].velocityX = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"positionVelocityY");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].velocityY = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"positionVelocityZ");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].velocityZ = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"hotHaloOutflowedAbundancesMetals");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].outflowedMetals = doubleBuffer[i];
  }

// added by harre 20130408
  err += getDoubleData(outputGroup,doubleBuffer,"hotHaloOutflowedMass");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].outflowedMass = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"diskStarFormationRate");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].diskStarFormationRate = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"spheroidStarFormationRate");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].spheroidStarFormationRate = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"nodeVirialRadius");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].nodeVirialRadius = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"nodeVirialVelocity");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].nodeVirialVelocity = doubleBuffer[i];
  }
// added by harre 20130717
  err += getDoubleData(outputGroup,doubleBuffer,"diskMassGas");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].diskGasMass = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"diskMassStellar");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].diskStellarMass = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"diskRadius");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].diskScaleLength = doubleBuffer[i];
  }
// added by harre 20130804
  err += getDoubleData(outputGroup,doubleBuffer,"spheroidMassGas");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].spheroidGasMass = doubleBuffer[i];
  }
  err += getDoubleData(outputGroup,doubleBuffer,"spheroidMassStellar");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].spheroidStellarMass = doubleBuffer[i];
  }
// added by harre 20130913
  err += getDoubleData(outputGroup,doubleBuffer,"basicMass");
  for(i=0;i<nHalos;i++) {
    nodeArray[i].basicMass = doubleBuffer[i];
  }



  if(err!=0) {
    return -1;
  }
  return 0;
}

int getDoubleData(struct groupStruct * outputGroup, double * buffer, char * dSetName) {
  hid_t dset;
  hid_t type;
  int err;
  dset = H5Dopen((*outputGroup).groupId,dSetName);
  if(dset<0) {
    printf("Problem opening dataset %s\n",dSetName);
    return -1;
  }
  type = H5Dget_type(dset);
  err = H5Dread(dset,type , H5S_ALL, H5S_ALL,H5P_DEFAULT,buffer);
  if(err<0) {
    printf("Problem reading dataset %s\n",dSetName);
    return -1;
  }
  H5Tclose(type);
  H5Dclose(dset);
  return 0;
}

int getIntData(struct groupStruct * outputGroup, long int * buffer, char * dSetName) {
  hid_t dset;
  hid_t type;
  int err;
  dset = H5Dopen((*outputGroup).groupId,dSetName);
  if(dset<0) {
    printf("Problem opening dataset %s\n",dSetName);
    return -1;
  }
  type = H5Dget_type(dset);
  err = H5Dread(dset,type , H5S_ALL, H5S_ALL,H5P_DEFAULT,buffer);
  if(err<0) {
    printf("Problem reading dataset %s\n",dSetName);
    return -1;
  }
  H5Tclose(type);
  H5Dclose(dset);
  return 0;
}


/* callback function for creating the time table */
herr_t makeTimetable(hid_t loc_id, const char *name, const H5L_info_t *info,
           struct timeStruct * timeTable ) {

  //printf(" %s\n",name);
  /* get output time of the specified group */
  hid_t gId;
  char gName[100];
  sprintf(gName,"/Outputs/%s",name);
  //printf("%s\n",gName);
  gId = H5Gopen((*timeTable).file_id,gName);
  /* declare the variables for the attributes */
  double timeGyr;
  double timeInSeconds;
  double scaleFactor;
  double redshift;
  /* open the attributes */
  hid_t aId;
  aId = H5Aopen(gId,"outputTime",H5P_DEFAULT);
  H5Aread(aId,H5T_IEEE_F64LE,&timeGyr);
  H5Aclose(aId);

  aId = H5Aopen(gId,"outputExpansionFactor",H5P_DEFAULT);
  H5Aread(aId,H5T_IEEE_F64LE,&scaleFactor);
  H5Aclose(aId);
  H5Gclose(gId);

  sprintf(gName,"/Outputs/%s/nodeData",name);
  gId = H5Gopen((*timeTable).file_id,gName);
  H5G_info_t groupInfo;
  H5Gget_info(gId, &groupInfo);
  int nLinks = (int)groupInfo.nlinks;
  //printf("node data contains %i links\n",nObjects);
  if(nLinks==0) {			/* if nodeData does not exist */
    (*timeTable).tStep[(*timeTable).counter].flagValid = -1;
    (*timeTable).tStep[(*timeTable).counter].nHalos = -1;
    (*timeTable).tStep[(*timeTable).counter].outputGroupName;
    printf("Attention, %s does not contain nodeData\n",name);
  } else {			/* true if the group is present */
    /* /\* get the number of Halos *\/ */
    hid_t dset;
    dset = H5Dopen(gId,"nodeIndex");
    hid_t dspace;
    dspace = H5Dget_space(dset);
    hssize_t nHalos;
    nHalos = H5Sget_simple_extent_npoints(dspace);
    H5Sclose(dspace);
    H5Dclose(dset);
    H5Gclose(gId);
    (*timeTable).tStep[(*timeTable).counter].flagValid = 0;
    (*timeTable).tStep[(*timeTable).counter].nHalos = (int)nHalos;
  }

  //printf("corresponds to %f\n",timeGyr);
  sprintf((*timeTable).tStep[(*timeTable).counter].outputGroupName,"%s",gName);
  (*timeTable).tStep[(*timeTable).counter].timeGyr = timeGyr;
  (*timeTable).tStep[(*timeTable).counter].scaleFactor = scaleFactor;
  redshift = 1./scaleFactor-1.0;
  (*timeTable).tStep[(*timeTable).counter].redshift = redshift;

  (*timeTable).counter++;

  return 0;
}

