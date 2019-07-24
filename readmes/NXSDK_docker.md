# NX-SDK Docker Environments

  NX-SDK provides the following Build Environments as docker images to use based on the use cases and where you want your 
  applicaitons to be deployed. Refer to <a href="https://hub.docker.com/r/dockercisco/nxsdk">NX-SDK Docker images</a> for 
  more information.
  
  | Docker Image Tag | OS | Deployment Mode | Usage Notes |
  |------------------|----|-----------------|-------------| 
  |dockercisco/nxsdk:v1    | Ubuntu | NX-OS  | Contains wrl5 toolchain needed to cross compile Applications to be deployed in NX-OS. |
  |dockercisco/nxsdk:v1.7.5| Ubuntu | NX-OS  | Contains the same toolchain as v1 plus golang compiler for Go applications.|
  |dockercisco/nxsdk:v2.0| Ubuntu | Remote | <ul><li>To deploy NX-SDK applications remotely.</li><li>No NX-OS wrl toolchain. </li><li>Contains grpc, rapidjson, remote NX-SDK libraries & other build essentials.</li></ul> |
  
