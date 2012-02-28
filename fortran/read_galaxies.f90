
 program read_galaxies

   USE HDF5
   IMPLICIT NONE

   CHARACTER(LEN=30), PARAMETER :: filename = "../output.hdf5" ! File name
   INTEGER(HID_T) :: file_id
   INTEGER(HID_T) :: dset_id
   INTEGER(HID_T) :: gId
   INTEGER(HID_T) :: aId
   INTEGER(HID_T) :: dspace_id
   INTEGER(HSIZE_T), DIMENSION(1:1) :: data_dims
   INTEGER(HSIZE_T), DIMENSION(1:1) :: scalar = (/1/)
   INTEGER(HSIZE_T) :: size
   INTEGER :: error ! Error flag
   CHARACTER(LEN=100) :: format_string
   
   CHARACTER(LEN=100) :: dsetname
   ! Timetable Variables
   INTEGER, allocatable :: nHalos(:)
   REAL*8, allocatable :: timeGyr(:)
   REAL*8, allocatable :: redshift(:)
   INTEGER :: nTimeSteps
   INTEGER :: tStep
   ! Node Data variables
   INTEGER, allocatable :: nodeIndex(:)
   INTEGER, allocatable :: descendentIndex(:)
   REAL*8, allocatable :: outflowedMetals(:)
   REAL*8, allocatable :: positionX(:)
   REAL*8, allocatable :: positionY(:)
   REAL*8, allocatable :: positionZ(:)


   write (*,*) 'The AWESOME Galacticus Reader'
  
   CALL h5open_f(error) ! Initialize Fortran interface

   CALL h5fopen_f(filename, H5F_ACC_RDONLY_F,file_id,error,H5P_DEFAULT_F)
   write (*,*) 'opened file ', filename

   ! Get the number of timesteps in the file
   CALL h5gopen_f(file_id,"/Output",gId,error)
   CALL h5aopen_name_f(gId,"nTimeSteps",aId,error)
   CALL h5screate_f(H5S_SCALAR_F, dspace_id,error)
   CALL h5aread_f(aId, H5T_STD_U32LE, nTimeSteps,scalar, error)
   CALL h5sclose_f(dspace_id,error)
   CALL h5aclose_f(aId,error)
   CALL h5gclose_f(gId,error)
   write (*,*) 'the file contains', nTimeSteps, ' timesteps.'
   data_dims = nTimeSteps
   
   allocate ( nHalos(nTimeSteps) )
   dsetname = "/TimeTable/nHalos"
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_STD_I32LE, nHalos, data_dims, error)
   CALL h5dclose_f(dset_id,error)
   write (*,*) 'number of halos: ', nHalos

   allocate ( timeGyr(nTimeSteps) )
   dsetname = "/TimeTable/timeGyr"
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_IEEE_F64LE, timeGyr, data_dims, error)
   CALL h5dclose_f(dset_id,error)
   write (*,*) 'output Times:', timeGyr

   allocate ( redshift(nTimeSteps) )
   dsetname = "/TimeTable/redshift"
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_IEEE_F64LE, redshift, data_dims, error)
   CALL h5dclose_f(dset_id,error)
   write (*,*) 'output redshifts:', redshift


   ! load node data
   ! Attention!!! Fortran Arrays Start with 1, not 0
   ! -> tStep 3 corresponds to dataSet_0002, as we start with 0
   !tStep = 3
   ! Attention!!! getTimeStep is just a dummy function which
   ! does not catch all relevant cases
   tStep = getTimeStep(12.8,nTimeSteps,timeGyr)
   write(*,*) 'We got timestep ', tStep
   data_dims = nHalos(tStep)
   format_string = "(A,I4.4)"
   ! write(*,*) 'test ', nHalos(tStep)
   ! open the corresponding data
   allocate ( nodeIndex(nHalos(tStep)) )
   allocate ( descendentIndex(nHalos(tStep)) )
   allocate ( positionX(nHalos(tStep)) )
   allocate ( positionY(nHalos(tStep)) )
   allocate ( positionZ(nHalos(tStep)) )
   allocate ( outflowedMetals(nHalos(tStep)) )

   ! get the correct name
   write(dsetname,format_string) '/Output/nodeIndex/nodeIndex_',tStep-1
   ! write(*,*) 'test ', dsetname
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_STD_I32LE, nodeIndex, data_dims, error)
   CALL h5dclose_f(dset_id,error)

   write(dsetname,format_string) '/Output/descendentIndex/descendentIndex_',tStep-1
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_STD_I32LE, descendentIndex, data_dims, error)
   CALL h5dclose_f(dset_id,error)

   write(dsetname,format_string) '/Output/positionX/positionX_',tStep-1
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_IEEE_F64LE, positionX, data_dims, error)
   CALL h5dclose_f(dset_id,error)

   write(dsetname,format_string) '/Output/positionY/positionY_',tStep-1
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_IEEE_F64LE, positionY, data_dims, error)
   CALL h5dclose_f(dset_id,error)

   write(dsetname,format_string) '/Output/positionZ/positionZ_',tStep-1
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_IEEE_F64LE, positionZ, data_dims, error)
   CALL h5dclose_f(dset_id,error)

   write(dsetname,format_string) '/Output/outflowedMetals/outflowedMetals_',tStep-1
   CALL h5dopen_f(file_id,dsetname,dset_id,error)
   CALL h5dread_f(dset_id,H5T_IEEE_F64LE, outflowedMetals, data_dims, error)
   CALL h5dclose_f(dset_id,error)


   write (*,*) 'Output some data as test:'
   write (*,*) 'nodeIndex:', nodeIndex(1), nodeIndex(10), nodeIndex(nHalos(tStep))
   write (*,*) 'descendentIndex:', descendentIndex(1), descendentIndex(10), descendentIndex(nHalos(tStep)) 
   write (*,*) 'positionX:', positionX(1), positionX(10), positionX(nHalos(tStep))
   write (*,*) 'positionY:', positionY(1), positionY(10), positionY(nHalos(tStep))
   write (*,*) 'positionZ:', positionZ(1), positionZ(10), positionZ(nHalos(tStep))
   write (*,*) 'outflowedMetals:', outflowedMetals(1), outflowedMetals(10), outflowedMetals(nHalos(tStep))


   CALL h5fclose_f(file_id,error)
   CALL h5close_f(error) ! Close Fortran interface

   deallocate(nodeIndex)
   deallocate(descendentIndex)
   deallocate(positionX)
   deallocate(positionY)
   deallocate(positionZ)
   deallocate(outflowedMetals)
   deallocate(nHalos)
   deallocate(timeGyr)
   deallocate(redshift)

CONTAINS
 integer function getTimeStep(tRequested, nTimeSteps, timeGyr)

   REAL*8 :: timeGyr(nTimeSteps)
   REAL :: tRequested
   INTEGER :: nTimeSteps
   INTEGER :: i
   DO i = 1, nTimeSteps
      write(*,*) 'i: ', i

      IF (timeGyr(i) .ge. tRequested) THEN
         getTimeStep = i-1
         RETURN
      END IF

   END DO
   write(*,*) 'It seems we did not find the timestep'
   RETURN
  end function getTimeStep

 end program read_galaxies


