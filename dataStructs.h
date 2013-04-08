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
  int descendentIndex;
  double positionX;
  double positionY;
  double positionZ;
  double velocityX;
  double velocityY;
  double velocityZ;
  double outflowedMetals;
// additions by harre 20130408
  double diskGasMass;   
  double diskGasMetals; 
  double diskScaleLength;          
  double diskStarFormationRate;     
  double diskStellarMass;           
  double diskStellarMetals;         
  double hotHaloAngularMomentum;    
  double hotHaloMass;   
  double hotHaloMetals; 
  double hotHaloOuterRadius;        
  double hotHaloUnaccretedMass;     
  double isolatedHostIndex;         
  double nodeMass;      
  double nodeSpin;      
  double nodeVirialRadius;    
  double nodeVirialVelocity;        
  double outflowedAngularMomentum; 
  double outflowedMass;           
  double spheroidAngularMomentum;   
  double spheroidCircularVelocity;  
  double spheroidGasMass;           
  double spheroidGasMetals;         
  double spheroidScaleLength;       
  double spheroidStarFormationRate;  
  double spheroidStellarMass;       
  double spheroidStellarMetals; 
};


#endif
