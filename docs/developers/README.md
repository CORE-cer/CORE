This documentation only talks baout the internal code of CORE. That is, anything under the namespace CORE::Internal.

# Project development.

## clang-format

it is crucial that your clang-format version is version 15 so that it is the same that is used in github actions. To download this version and set it as default you can use these commands in ubuntu:

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

This project is separated into three folders: core_client, core_server, core_streamer. The client deals with everything that has to do with non-streaming interactions with the server, and the streamer simply sends streams to the server. These three folders share datastructures that are in "shared", and these datastructures are serialized to pass from the client to the server, or viceversa or the streamer to the server.

All unit tests are stored inside the unit_tests/ folder, following the location of the file that will be tested.

## Internal Server Code

### General Data flow: (Crucial)

1. A User sends, through the core_client, a query as a string.
    For example:

    ```
    "SELECT * FROM S\n"
    "WHERE (SELL as T1; BUY as T2; BUY as T3)\n"
    "FILTER T1[name = 'INTC'] AND T2[name = 'RIMM'] AND T3[name = 'QQQ'] \n"
    "WITHIN TIMESTAMP [stock_time]\n");
    ```

2. Inside ClientMessageHandler::add_query This string is converted
   into a CEQL (Complex Event Query Language) query
   - file for the datastructure:: `core_server/internal/ceql/query/query.hpp`

3. the AnnotatePredicatesWithNewPhysicalPredicates visitor then identifies
   all predicates (For example: `T1[name = 'INTC']`) and:
    - Transforms the CEQL Predicate into a Physical Predicate either
      with the visitor CEQLStrongTypedPredicateToPhysicalPredicate or
      CEQLWeaklyTypedPredicateToPhysicalPredicate. A strongly typed
      predicate is a predicate that has an event name before the condition:
      For example: `Sell[name = 'INTC']` would be a strongly typed predicate,
      but `T1[name = 'INTC']` would be weakly typed, because it involves
      the variable T1
    - Stores all these physical predicates and annotates every CEQL predicate
      with an id that represents the physical predicate that is created.

4. With the newly created PhysicalPredicates a PredicateEvaluator
   is created
   that evaluates tupples and returns a mpz_class, ie, a bitset that
   represents what physical predicate is correctly evaluated.
    - NOTE: the first positions of the bitset represent predicates that
      just state whether a tuple is of the event type with the corresponding
      id. For instance: If there are 5 different event types, then the first
      5 positions will determine what type of event it is, and the rest determines
      whether a specific predicate is met.

5. The where clause of the query is transformed to a LogicalCEA
   using the visitor: CEQL::FormulaToLogicalCEA.

6. The LogicalCEA is transformed into a CEA, and then into a DetCEA. These
   transformations can be seen inside the constructors of these classes.

7. With the CEA and the PredicateEvaluator the query_evaluator is created inside
   Mediator::create_complex_event_stream, this query_evaluator is then stored
   and messages will be sent to it when pertinent tuples are sent through
   a streamer.

### Architecture of the CoreServer

![UML Diagram](imgs/architecture.png)

### Communication modalities

There are 6 communication modalities used, that correspond directly to
the direct ZMQ communication modalities, for more information I
recommend looking at that documentation, but in a nutshell,

- Broadcaster commmunicates to the subscriber through a subscriber
publisher modality
- The router handles a request from the dealer by receiving a message
  from it, handling it and sending a response back.
- The receiver waits for messages from the message_sender in a port,
  and just handles it.

These communication modalities are implemented using ZMQ, and are
shown inside of the folder shared/networking, if an implementation
using another framework is desired, they will have to follow the
interface from the base class.

1. message_broadcaster: The QueryEvaluator uses it, it is a publisher
2. message_subscriber: The Client uses this when it subscribes to
    a complex event. The handler that is passed under the subscription
    then handles this message inside of the function:
    `Client::subscribe_to_complex_event`.
3. message_router: The CORE Server passes through templating the
    class ClientMessageHandler that handles the string that is passed
    to the Routher through the function: ClientMessageHandler::operator()
4. message_dealer: The CORE Client uses this inside of the function
    `Client::send_request`.
5. message_receiver: The CORE Client waits for messages from the
    CORE Streamer through this communication scheme inside of the
    StreamsListener::start() function.
6. message_sender: The CORE Streamer uses this communication framework
    inside of the `Streamer::send_stream` function.

### Other Important details:

- The router is opened on the first port, the second port goes to the
  StreamsListener and the rest to individual QueryEvaluators.
- The communication scheme is under TCP, communicating by serializing
  the datastructures from the shared folder using the library cereal.
