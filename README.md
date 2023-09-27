# CORE: a Complex Event Recognition Engine

## Overview

This is a C++ reimplementation of the CORE engine, as presented in the paper ["CORE: a Complex Event Recognition Engine"](https://arxiv.org/abs/2111.04635) by Marco Bucchi, Alejandro Grez, Andrés Quintana, Cristian Riveros, and Stijn Vansummeren. This engine is designed for the efficient evaluation of complex event queries over large data streams in real time.

## Features

- **Efficient Query Evaluation**: Specialized in evaluating a broad array of complex event queries, including those featuring the 'within time' operator.
  
- **Automaton-based Algorithm**: Utilizes an innovative automaton-based evaluation algorithm that maintains a data structure to represent the set of partial matches in constant time per input event.

- **Stable Performance**: Exhibits consistent performance regardless of query size or time window size.

- **High Scalability**: Aims to outperform state-of-the-art CER systems across multiple workloads.

- **Test Driven Development**: Developed using unit tests with the Catch2 framework.

- **Conan Package Manager**

### In Development

- **Partition-by Operator**: The feature to support the partition-by event correlation operator is currently under development.

- **Support for UDP**: TCP network transport is the current bottleneck.

## Quick Start

To get started quickly, a Docker setup is available:

```bash
# Clone the repository
git clone https://github.com/CER-CORE/CORE

# Navigate into the directory
cd CORE

# Start the Docker container
sudo docker-compose run core-terminal bash
```

Inside the docker terminal run:

```bash
./scripts/compile_and_test.sh
```

For more detailed installation and setup instructions, including manual installation steps, please refer to our [Installation Guide](docs/users/Installation.md).

## Target Files

### Client - Server in separate files:

#### Files:

- `src/targets/server.cpp`
- `src/targets/client.cpp`

### Simulation1 and Simulation2
- **Files**: `src/targets/simulation1.cpp` `src/targets/simulation2.cpp`
- Combines the server and client inside a single simulation.

## Detailed Documentation

For comprehensive documentation:

- **Users**: Navigate to [User Documentation](./docs/users/README.md).
  
- **Developers**: Navigate to [Developer Documentation](./docs/developers/README.md).

## Related Work

- The original paper can be found [here](https://arxiv.org/abs/2111.04635).

## Contributing

Contributions are welcome! See our [Developer's Guide](./docs/developers/CONTRIBUTING.md) for details.

## License

[GNU GENERAL PUBLIC LICENSE](./LICENSE.txt)

## Contact

For additional details or feedback, feel free to [create an issue](https://github.com/CER-CORE/CORE/issues).
