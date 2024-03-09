# CORE: a Complex Event Recognition Engine

## Overview

This is a C++ reimplementation of the CORE engine, as presented in the paper ["CORE: a Complex Event Recognition Engine"](https://www.vldb.org/pvldb/vol15/p1951-riveros.pdf) by Marco Bucchi, Alejandro Grez, Andrés Quintana, Cristian Riveros, and Stijn Vansummeren. This engine is designed for the efficient evaluation of complex event queries over large data streams in real time.

## Features

- **Efficient Query Evaluation**: Specialized in evaluating a broad array of complex event queries, including those featuring the 'within time' operator.
  
- **Automaton-based Algorithm**: Utilizes an innovative automaton-based evaluation algorithm that maintains a data structure to represent the set of partial matches in constant time per input event.

- **Stable Performance**: Exhibits consistent performance regardless of query size or time window size.

- **High Scalability**: Aims to outperform state-of-the-art CER systems across multiple workloads.

- **Test Driven Development**: Developed using unit tests with the Catch2 framework.

- **Conan Package Manager**

### In Development

- **Support for UDP**: TCP network transport is the current bottleneck.

## Quick Start

To get started quickly in your local machine: 

```bash
# Clone the repository
git clone https://github.com/CER-CORE/CORE

# Navigate into the directory
cd CORE

# Compile the project and run third party and unit tests
./scripts/build_and_test.sh

```
You can also use Dockers, a Docker setup is available:

```bash
# Start the Docker container
sudo docker-compose run core-terminal bash

# Inside the docker terminal run:
./scripts/build_and_test.sh
```

Following these steps you will be building the project in Debug mode. There are another build options, which are Release, RelWithDebInfo and MinSizeRel:

```bash
./scripts/compile_and_test.sh -b Release

./scripts/compile_and_test.sh -b RelWithDebInfo

./scripts/compile_and_test.sh -b MinSizeRel
```

For more detailed installation and setup instructions, including manual installation steps, please refer to our [Installation Guide](docs/users/INSTALLATION.md).

## Target Files

### Client - Server in separate files:

#### Files:

- `src/targets/server.cpp`
- `src/targets/client.cpp`

### Simulation1 and Simulation2
- **Files**: `src/targets/simulation1.cpp` `src/targets/simulation2.cpp`
- Combines the server and client inside a single simulation.

## Experiments

Currently there are three data sets used as streams for doing experiments with this project. The files can be found in `src/targets/experiments`. Each experiment file has its own data, queries and expected results for every specific query.

There is also a script for every experiment which runs every query and compares the result with the expected results automarically. For example, to run the script of the smart_homes experiments you have to run the following command:

```bash
./scripts/build_and_test_smart_homes_queries.sh
```

For the same experiment, if you only want to try an specific query you'll have to run the following command:

```bash
build/Debug/offline_experiment_smart_homes src/targets/experiments/smart_homes/queries/q1_any.txt src/targets/experiments/smart_homes/smart_homes_data.csv
```

In this example, we are running the executable of the smart homes experiment in Debug mode for the specific query q1_any.txt, and the data set is the one in smart_homes_data.csv.

## Detailed Documentation

For comprehensive documentation:

- **Users**: Navigate to [User Documentation](./docs/users/README.md).
  
- **Developers**: Navigate to [Developer Documentation](./docs/developers/README.md).

## Related Work

- The original paper can be found [here](https://www.vldb.org/pvldb/vol15/p1951-riveros.pdf).

## Contributing

Contributions are welcome! See our [Contributing's Guide](docs/users/CONTRIBUTING.md) for details.

## License

[GNU GENERAL PUBLIC LICENSE](./LICENSE.txt)

## Contact

For additional details or feedback, feel free to [create an issue](https://github.com/CER-CORE/CORE/issues).
