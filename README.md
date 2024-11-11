# TP2_SO
Trabajo practico especial de la materia 72.11 - Sistemas Operativos. Desarrollado por Magdalena Cullen, Timoteo Feeney, y Valentino Sanguinetti.

## Environment Setup

### 1. Create docker container
Using the following image, create the Docker container:
```bash
  docker pull agodio/itba-so-multi-platform:3.0
```
In the directory of this repository, create the container:
```bash
docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name SO-TP2 agodio/itba-so-multi-platform:3.0
```
Open the cointainer:
```bash
docker exec -ti SO-TP2  bash
cd root
```
### 2. Build the Toolchain
Within the container, build the Toolchain:
```bash
cd Toolchain
make all
```
### 2. Build the project
From the Toolchain directory, build the project:
```bash
cd ..
make all
```
If you want to use Buddy Memory Manager when building project, use the following command: 
```bash
cd ..
make all MM=BUDDY
```
### 3. Clean the project
To clean the project, run the following command from the root of the project directory:
```bash
make clean
```
### 4. Run project outside container
To run the project from outside the container, execute:
```bash
./run.sh
```
