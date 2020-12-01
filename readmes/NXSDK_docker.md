# NX-SDK Docker Environments

  NX-SDK provides the following Build Environments as docker images to use based on the use cases and where you want your 
  applicaitons to be deployed. 
  
  | Docker Image Tag | OS | Deployment Mode | Usage Notes |
  |------------------|----|-----------------|-------------| 
  |<a href="https://hub.docker.com/r/dockercisco/nxsdk">dockercisco/nxsdk:v1</a>    | Ubuntu | NX-OS  | Contains wrl5 toolchain needed to cross compile Applications to be deployed in NX-OS. |
  |<a href="https://hub.docker.com/r/dockercisco/nxsdk">dockercisco/nxsdk:v1.7.5</a>| Ubuntu | NX-OS  | Contains the same toolchain as v1 plus golang compiler for Go applications.|
  
