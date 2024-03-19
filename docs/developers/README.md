This documentation only talks about the internal code of CORE. That is, anything under the namespace CORE::Internal.

# Project development

## clang-format

It is crucial that your clang-format version is version 15, so that it is the same that is used in github actions. To download this version and set it as default you can use these commands in ubuntu:

```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal main" | sudo tee -a /etc/apt/sources.list
sudo apt-get update
sudo apt-get install -y clang-format-15
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-15 1000
```

And after that, `clang-format --version` should be 15. Use the script: ./scripts/clang_format_all_files.sh before merging with main to pass that individual github action.

Note that if you wish to not use clang-format somewhere in your code, you just need to add these comments:

```
// clang-format off

// ... Code that is not formatted...

// clang-format on
```

It is important to note we also provide known working conan profiles, located at `./conan_profiles`.


## Folder Distribution

This project is separated into three folders: core_client, core_server, and core_streamer. The client deals with everything that has to do with non-streaming interactions with the server, and the streamer simply sends streams to the server. These three folders share data structures that are in "shared", and these data structures are serialized to pass from the client to the server, or viceversa or the streamer to the server.

All unit tests are stored inside the unit_tests/folder, following the location of the file that will be tested.

# Internal Server Code

## General Data flow (Crucial)

To understand the general data flow, we recommend to first read the description of CORE system presented in ["CORE: a Complex Event Recognition Engine"](https://www.vldb.org/pvldb/vol15/p1951-riveros.pdf). Several notions (like CEQL, LogicalCEA, CEA, DetCEA) are defined and explained there. The data flow of CORE is the following.

1. A user sends a query as a string through the core_client. For example:

    ```
    "SELECT * FROM S\n"
    "WHERE (SELL as T1; BUY as T2; BUY as T3)\n"
    "FILTER T1[name = 'INTC'] AND T2[name = 'RIMM'] AND T3[name = 'QQQ'] \n"
    "WITHIN TIMESTAMP [stock_time]\n";
    ```

2. A ClientMessageHandler object then receives the query from the client and parse it to obtain the information in an object calles Query. It also creates a ResultHandlerT which is in charge of sending the results of the query back to the specific client.

3. The Backend is in charge of receiving the Query object and creates another object for the specific query, which can be a SimpleQuery (when we don't consider partition by) or a PartitionByQuery. After this, the Backend initializes the query, where the QueryCatalog is created and the method create_query of the specific query object is called. This method is in charge of the following steps

4. The AnnotatePredicatesWithNewPhysicalPredicates visitor is created, which identifies all predicates (for example `T1[name = 'INTC']`) and:
    - Transforms the CEQL Predicate into a Physical Predicate either
      with the visitor CEQLStrongTypedPredicateToPhysicalPredicate or
      CEQLWeaklyTypedPredicateToPhysicalPredicate. A strongly typed
      predicate is a predicate that has an event name before the condition.
      For example, `Sell[name = 'INTC']` would be a strongly typed predicate,
      but `T1[name = 'INTC']` would be weakly typed, because it involves
      the variable T1.
    - Stores all these physical predicates and annotates every CEQL predicate
      with an id that represents the physical predicate that is created.

5. With the newly created PhysicalPredicates, a PredicateEvaluator
   is created
   that evaluates tuples and returns a mpz_class object, that is, a bitset that
   represents what physical predicate is correctly evaluated.
    - NOTE: the first positions of the bitset represent predicates that
      just state whether a tuple is of the event type with the corresponding
      id. For instance, if there are five different event types, then the first
      five positions will determine what type of event it is, and the rest determines
      whether a specific predicate is met.

6. The WHERE clause of the query is transformed to a LogicalCEA
   using the visitor: CEQL::FormulaToLogicalCEA.

7. The LogicalCEA is transformed into a CEA, and then into a DetCEA. These
   transformations can be seen inside the constructors of these classes.

8. An evaluator is then created in the specific query object (the evaluator in SimpleQuery and PartitionBy are different)

9. The evaluator is in charge of processing the events following the algorithm described in the paper. After processing the events, if there is a result it creates an enumerator. 

10. The results are send back to the client with the ResultHandler.

## Architecture of the CoreServer

The following figure describes the arquitecture of CoreServer. (Not updated, figure in development)

![UML Diagram](imgs/architecture.png)

## Communication modalities

There are six communication modalities used, that correspond directly to
the direct ZMQ communication modalities. For more information, we
recommend looking at that documentation. In a nutshell:

- Broadcaster commmunicates to the subscriber through a subscriber
publisher modality.
- The router handles a request from the dealer by receiving a message
  from it, handling it and sending a response back.
- The receiver waits for messages from the message_sender in a port,
  and just handles it.

These communication modalities are implemented using ZMQ, and are
shown inside of the folder shared/networking. If an implementation
using another framework is desired, they will have to follow the
interface from the base class.

1. message_broadcaster: The QueryEvaluator uses it, it is a publisher.
2. message_subscriber: The Client uses this when it subscribes to
    a complex event. The handler that is passed under the subscription
    then handles this message inside of the function:
    `Client::subscribe_to_complex_event`.
3. message_router: The CORE Server passes through templating the
    class ClientMessageHandler that handles the string that is passed
    to the Routher through the function: `ClientMessageHandler::operator()`.
4. message_dealer: The CORE Client uses this inside of the function
    `Client::send_request`.
5. message_receiver: The CORE Client waits for messages from the
    CORE Streamer through this communication scheme inside of the
    `StreamsListener::start()` function.
6. message_sender: The CORE Streamer uses this communication framework
    inside of the `Streamer::send_stream` function.

## Other important details

- The router is opened on the first port, the second port goes to the
  StreamsListener, and the rest to individual QueryEvaluators.
- The communication scheme is under TCP, communicating by serializing
  the data structures from the shared folder using the library cereal.
