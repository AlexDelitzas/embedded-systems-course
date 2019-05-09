# Assignment 1

Two versions of a simple real-time timer that is used for low-consumption periodic sampling.

## Project structure

```
project
├── README.md
|
├── Makefile
│
├── logs ( contains the log files with the timestamps for each version )
│   ├── timestamps_v1.txt
│   └── timestamps_v2.txt
│
├── src ( contains the source code )
│   ├── sampling_v1.c ( version 1 - use of nanosleep() )
│   └── sampling_v2.c ( version 2 - use of setitimer() )
|
├── utils 
│   ├── monitor_cpu.sh ( bash script that is used to measure the CPU usage )
│   └── statistics.m ( MATLAB script that is used for statistical analysis )
|
├── bin (contains the executables)
│   ├── sampling_v1 ( version 1 )
│   └── sampling_v2 ( version 2 )
|
└── report (contains the report files)
```

## Compilation

```
make
```

## Execution

First go to bin/ directory
```
cd bin
```

### For version 1
```
./sampling_v1 t dt
```
where:
  * t: total sampling duration (sec)
  * dt: sampling period (sec)
  
*Execution example*:
```
./sampling_v1 7200 0.1
```

### For version 2
```
./sampling_v2 t dt
```
where:
  * t: total sampling duration (sec)
  * dt: sampling period (sec)

*Execution example*:
```
./sampling_v2 7200 0.1
```

## CPU Usage
If you want to measure the CPU usage of the implementations, first go to utils/ directory
```
cd utils
```
Then run:
```
./monitor_cpu.sh pid
```
where pid is the process id of process that you want to measure the CPU usage
