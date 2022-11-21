# 2D_FEM_Solver

#PETSC is required?

A 2D Finite Element Solver... The rest of the description is a WIP.....

# Installing Dependencies
1. OpenMPI **(For Custom OpenMP Builds)**<br/> 
.././configure --prefix=/home/openmpi --with-devel-headers --enable-binaries <br/>
make -j3 <br/>
make all install <br/>

2. Petsc **(Default Install)**<br/>
./configure --with-scalar-type=real <br/> <br/>
./configure --with-scalar-type=real --prefix=$PWD/build --download-openmpi=/home/gmovillanueva/Dependencies/ --download-fblaslapack
Possible config options: **(With Custom OpenMP)** <br/>
--with-mpi-dir=/opt/openmpi --prefix=$PWD/build




