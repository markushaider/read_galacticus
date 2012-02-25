#include <hdf5.h>
#include "dataStructs.h"

int writeTimeTable(char * filename, struct timeStruct * timeTable) {

  /* make timestep struct to sort */

  /* determine how many valid timesteps we have */

  /* make sorted array, perhaps already with the dataset names in the output group? */
  

  /* write the timeTable to an hdf5 file */
  hid_t file_id;
  file_id = H5Fcreate(filename , H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  hid_t group_id;
  group_id = H5Gcreate(file_id, "/Output", H5P_DEFAULT);
  /* creating attributes */
  /* number of output timesteps */
  /* change (*timeTable).nTimeSteps to the actual number of timesteps */
  hid_t dspace_id;
  hid_t a_id;
  dspace_id = H5Screate(H5S_SCALAR);
  a_id = H5Acreate(group_id,"nTimeSteps", H5T_STD_U32LE,
  			   dspace_id,H5P_DEFAULT);
  H5Awrite(a_id, H5T_STD_U32LE, &(*timeTable).nTimeSteps);
  H5Aclose(a_id);
  H5Gclose(group_id);
  group_id = H5Gcreate(file_id, "/TimeTable", H5P_DEFAULT);

  hsize_t length = (*timeTable).nTimeSteps;
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
  for(i=0; i<(*timeTable).nTimeSteps; i++) {
    H5Sselect_hyperslab(dspace_id,H5S_SELECT_SET,&offset,NULL,&count,NULL);
    H5Dwrite(dset_id, H5T_NATIVE_INT, memspace_id, dspace_id, H5P_DEFAULT, &i);
    offset += 1;
  }
  H5Sclose(memspace_id);
  H5Dclose(dset_id);

  /* write the timeGyr*/
  dset_id = H5Dcreate(group_id,"timeGyr",H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, (*timeTable).timeGyr);
  H5Dclose(dset_id);

  /* write timeScaleFactor*/
  dset_id = H5Dcreate(group_id,"scaleFactor",H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, (*timeTable).scaleFactor);
  H5Dclose(dset_id);

  /* write redshift*/
  dset_id = H5Dcreate(group_id,"redshit",H5T_IEEE_F64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, (*timeTable).redshift);
  H5Dclose(dset_id);

  /* write nHalos*/
  dset_id = H5Dcreate(group_id,"nHalos",H5T_STD_I64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, (*timeTable).nHalos);
  H5Dclose(dset_id);

 /* write flagValid*/
  dset_id = H5Dcreate(group_id,"flagValid",H5T_STD_I64LE,dspace_id, H5P_DEFAULT);
  H5Dwrite(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, (*timeTable).flagValid);
  H5Dclose(dset_id);


  H5Gclose(group_id);
  H5Fclose(file_id);

  return 0;
};

int writeNodeData(char * filename, struct nodeStruct * test) {


  return 0;
};
