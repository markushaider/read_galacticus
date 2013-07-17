#include <hdf5.h>
#include <stdlib.h>
#include "dataStructs.h"

int writeTimeTable(char * filename, struct timeStruct * timeTable) {

  /* write the timeTable to an hdf5 file */
  hid_t file_id;
  file_id = H5Fcreate(filename , H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  hid_t group_id;
  group_id = H5Gcreate(file_id, "/Output", H5P_DEFAULT);
  /* creating attributes */
  /* number of output timesteps */
  hid_t dspace_id;
  hid_t a_id;
  dspace_id = H5Screate(H5S_SCALAR);
  a_id = H5Acreate(group_id,"nTimeSteps", H5T_STD_U32LE,
  			   dspace_id,H5P_DEFAULT);
  H5Awrite(a_id, H5T_STD_U32LE, &(*timeTable).validTimeSteps);
  H5Aclose(a_id);
  H5Gclose(group_id);
  group_id = H5Gcreate(file_id, "/TimeTable", H5P_DEFAULT);

  /* create also other necessary groups */
  hid_t dg_id;
  dg_id = H5Gcreate(file_id, "/Output/nodeIndex", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/outflowedMetals", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/positionX", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/positionY", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/positionZ", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/velocityX", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/velocityY", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/velocityZ", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/descendentIndex", H5P_DEFAULT);
  H5Gclose(dg_id);
// addition by harre 20130408
  dg_id = H5Gcreate(file_id, "/Output/outflowedMass", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/diskStarFormationRate", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/spheroidStarFormationRate", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/nodeVirialRadius", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/nodeVirialVelocity", H5P_DEFAULT);
  H5Gclose(dg_id);
// addition by harre 20130717
  dg_id = H5Gcreate(file_id, "/Output/diskGasMass", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/diskStellarMass", H5P_DEFAULT);
  H5Gclose(dg_id);
  dg_id = H5Gcreate(file_id, "/Output/diskScaleLength", H5P_DEFAULT);
  H5Gclose(dg_id);





  int invalidSteps = (*timeTable).nTimeSteps-(*timeTable).validTimeSteps;
  hsize_t length = (*timeTable).validTimeSteps;
  /* specify the dimensions of the dataset */
  dspace_id = H5Screate_simple(1,&length,NULL);
  /* create the dataset */
  hid_t dset_id;
  dset_id = H5Dcreate(group_id,"timeIndex",H5T_STD_I64LE,dspace_id, H5P_DEFAULT);
  /* fill the dataset element by element through selecting hyperslabs */
  hsize_t offset = 0;
  hsize_t count = 1;
  hid_t memspace_id = H5Screate(H5S_SCALAR);
  int i;
  for(i=0; i<(int)length; i++) {
    H5Sselect_hyperslab(dspace_id,H5S_SELECT_SET,&offset,NULL,&count,NULL);
    H5Dwrite(dset_id, H5T_NATIVE_INT, memspace_id, dspace_id, H5P_DEFAULT, &i);
    offset += 1;
  }
  H5Sclose(memspace_id);
  H5Dclose(dset_id);

  /* write the timeGyr*/
  double * dBuf = (double *)malloc((int)length*sizeof(double));
  for(i=0;i<(int)length;i++) {
    dBuf[i] = (*timeTable).tStep[i+invalidSteps].timeGyr;
  }
  dset_id = H5Dcreate(group_id,"timeGyr",H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);

  /* write timeScaleFactor*/
  for(i=0;i<(int)length;i++) {
    dBuf[i] = (*timeTable).tStep[i+invalidSteps].scaleFactor;
  }
  dset_id = H5Dcreate(group_id,"scaleFactor",H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);

  /* write redshift*/
  for(i=0;i<(int)length;i++) {
    dBuf[i] = (*timeTable).tStep[i+invalidSteps].redshift;
  }
  dset_id = H5Dcreate(group_id,"redshift",H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  free(dBuf);

  /* write nHalos*/
  int * iBuf = (int *)malloc((int)length*sizeof(int));
  for(i=0;i<(int)length;i++) {
    iBuf[i] = (*timeTable).tStep[i+invalidSteps].nHalos;
  }
  dset_id = H5Dcreate(group_id,"nHalos",H5T_STD_I64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, iBuf);
  H5Dclose(dset_id);

  /* write flagValid*/
  for(i=0;i<(int)length;i++) {
    iBuf[i] = (*timeTable).tStep[i+invalidSteps].flagValid;
  }
  dset_id = H5Dcreate(group_id,"flagValid",H5T_STD_I64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, iBuf);
  H5Dclose(dset_id);
  free(iBuf);

  H5Gclose(group_id);
  H5Fclose(file_id);

  return 0;
};

int writeNodeData(char * inputfile, char * outputfile, char * outputGroupName, int nHalos, int counter) {

  /* get a specified output group */
  struct groupStruct outputGroup;
  
  outputGroup.file_id = H5Fopen(inputfile, H5F_ACC_RDONLY, H5P_DEFAULT);
  if(outputGroup.file_id < 0) {
    printf("Error opening HDF5 file\n");
    return -1;
  }
   int err = getOutputGroup(&outputGroup,outputGroupName);
  if (err !=0) {
    printf("Encountered error in getOutputGroup\n");
    return -1;
  }

  /* make array of the nodes */
  struct nodeStruct * nodeArray;
  nodeArray = (struct nodeStruct *)malloc(nHalos*sizeof(struct nodeStruct));
  /* fill array with the node data */
  err = fillNodeArray(&outputGroup,nodeArray,nHalos);
  if (err !=0) {
    printf("Encountered error in fillNodeArray\n");
    return -1;
  }
  /* close the group */
  H5Gclose(outputGroup.groupId);
  H5Fclose(outputGroup.file_id);

  /* data should now be present, write to hdf5 file */

  /* write to the hdf5 file */
  hid_t outputFile_id;
  outputFile_id = H5Fopen(outputfile , H5F_ACC_RDWR, H5P_DEFAULT);
  hid_t gId;
  gId = H5Gopen(outputFile_id, "/Output/nodeIndex");
  hsize_t length = nHalos;  
  int * iBuf = (int *)malloc((int)length*sizeof(int));
  int i;
  for(i=0;i<(int)length;i++)
    iBuf[i] = nodeArray[i].nodeIndex;
  hid_t dspace_id = H5Screate_simple(1,&length,NULL);
  char dsetName[100];
  sprintf(dsetName,"nodeIndex_%04i",counter);
  hid_t dset_id = H5Dcreate(gId,dsetName,H5T_STD_I64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, iBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id,"/Output/descendentIndex");  
  for(i=0;i<(int)length;i++)
    iBuf[i] = nodeArray[i].descendentIndex;
  sprintf(dsetName,"descendentIndex_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_STD_I64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, iBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id,"/Output/positionX");  
  double * dBuf = (double *)malloc((int)length*sizeof(double));
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].positionX;
  sprintf(dsetName,"positionX_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id,"/Output/positionY");  
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].positionY;
  sprintf(dsetName,"positionY_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/positionZ");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].positionZ;
  sprintf(dsetName,"positionZ_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/velocityX");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].velocityX;
  sprintf(dsetName,"velocityX_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/velocityY");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].velocityY;
  sprintf(dsetName,"velocityY_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/velocityZ");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].velocityZ;
  sprintf(dsetName,"velocityZ_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/outflowedMetals");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].outflowedMetals;
  sprintf(dsetName,"outflowedMetals_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

// addition by harre 20130408 

  gId = H5Gopen(outputFile_id, "/Output/outflowedMass");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].outflowedMass;
  sprintf(dsetName,"outflowedMass_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/diskStarFormationRate");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].diskStarFormationRate;
  sprintf(dsetName,"diskStarFormationRate_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/spheroidStarFormationRate");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].spheroidStarFormationRate;
  sprintf(dsetName,"spheroidStarFormationRate_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/nodeVirialRadius");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].nodeVirialRadius;
  sprintf(dsetName,"nodeVirialRadius_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/nodeVirialVelocity");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].nodeVirialVelocity;
  sprintf(dsetName,"nodeVirialVelocity_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/diskGasMass");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].diskGasMass;
  sprintf(dsetName,"diskGasMass_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/diskStellarMass");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].diskStellarMass;
  sprintf(dsetName,"diskStellarMass_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

  gId = H5Gopen(outputFile_id, "/Output/diskScaleLength");
  for(i=0;i<(int)length;i++)
    dBuf[i] = nodeArray[i].diskScaleLength;
  sprintf(dsetName,"diskScaleLength_%04i",counter);
  dset_id = H5Dcreate(gId,dsetName,H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, dBuf);
  H5Dclose(dset_id);
  H5Gclose(gId);

 
  H5Sclose(dspace_id);

  H5Fclose(outputFile_id);
  free(nodeArray);
  free(iBuf);

  return 0;

};
