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

## Stream Declaration

If a user wants to connect a stream to CORE he has to declare the stream that is going to connect. In order to declare the stream, the client has to specify the name of the stream, the events that this stream has and the attributes (with its specific type) of each event. Here is an example:  

```bash
"CREATE STREAM StreamName {\n"
"EVENT EventName { AttributeName:AttributeType, Value1:string, Value2:boolean },\n"
"EVENT Hum { ID:int, Value3:double },\n"
"EVENT Hum2 { ID:int }\n"
"}";
```

The types of attributes that are available are int, string, double and bool.

One feature that CORE supports is that it allows multiple streams to share events. For that, the event must have the same name in every stream that is declared.

## Target Files

### Client - Server in separate files:

#### Files:

- `src/targets/online/server.cpp`
- `src/targets/online/client.cpp`

To run the server and client you can run them either in online and offline method:

##### Offline:

The offline method only allows one cient at a time. In the build folder there will be an executable for runing a stream in a data set for an specific query.  The command to run the executable is the following:

```bash
./build/BUILD_METHOD/offline /path/to/specific/query /path/to/stream/declaration path/to/stream/data
```

where BUILD_METHOD corresponds to the method you used to build the project (see more info. in the Quick Start section). You will need the path to the specific query, the path to the stream declaration and the path to the data set you are using as a stream.

##### Online:

The online method allows more clients to connect to the server. First you need to start the server, for this, you'll need to run the following command:

```bash
./build/BUILDING_METHOD/online_server
```

Once the server has started, you can start adding clients to it. To add a client you'll need to run a command similar to the one in the online method, so you'll need the path to the query, to the stream declaration and the data set:

```bash
./build/BUILD_METHOD/online_client /path/to/specific/query /path/to/stream/declaration path/to/stream/data
```

## Experiments

Currently there are three data sets used as streams for doing experiments with this project. The files can be found in `src/targets/experiments`. Each experiment file has its own data, queries and expected results for every specific query.

There is also a script for every experiment which runs every query and compares the result with the expected results automarically. For example, to run the script of the smart_homes experiments you have to run the following command:

```bash
./scripts/build_and_test_smart_homes_queries.sh
```

For the same experiment, if you only want to try an specific query you'll have to run the following command:

```bash
./build/Debug/offline ./src/targets/experiments/smart_homes/queries/q1_none.txt ./src/targets/experiments/smart_homes/declaration.core ./src/targets/experiments/smart_homes/smart_homes_data.csv
```

In this example, we are running the executable of the smart homes experiment in Debug mode for the specific query q1_any.txt, the stream declaration is declaration.core and the data set is the one in smart_homes_data.csv.

## Detailed Documentation

For comprehensive documentation:

- **Users**: Navigate to [User Documentation](./docs/users/README.md). (Documentation in development)
  
- **Developers**: Navigate to [Developer Documentation](./docs/developers/README.md).

## Related Work

- The original paper can be found [here](https://www.vldb.org/pvldb/vol15/p1951-riveros.pdf).

## Contributing

Contributions are welcome! See our [Contributing's Guide](docs/users/CONTRIBUTING.md) for details.

## License

[GNU GENERAL PUBLIC LICENSE](./LICENSE.txt)

## Contact

For additional details or feedback, feel free to [create an issue](https://github.com/CER-CORE/CORE/issues).
