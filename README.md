# Chrono Exchange Engine

**A C++17 limit order matching engine with a TCP order gateway, price-time priority, and real-time execution reports.**

Chrono Exchange Engine is a systems-programming project designed to explore the core components of an electronic trading system. It combines an in-memory order book, deterministic order matching, and a TCP interface built with Boost.Asio.

The project focuses on modern C++, data structures, networking, concurrency design, and software architecture.

> **Project status:** In development. The core matching engine is implemented, and TCP server/client integration is being developed.

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Features](#features)
- [Technology Stack](#technology-stack)
- [How Order Matching Works](#how-order-matching-works)
- [Communication Protocol](#communication-protocol)
- [Project Structure](#project-structure)
- [Build and Run](#build-and-run)
- [Example Session](#example-session)
- [Design Decisions](#design-decisions)
- [Testing](#testing)
- [Roadmap](#roadmap)
- [Learning Objectives](#learning-objectives)

## Overview

An exchange matching engine receives buy and sell orders and determines when they can be matched according to defined trading rules.

Chrono Exchange Engine implements the core of this process:

1. Accept an order request.
2. Validate and parse the request.
3. Add, modify, or cancel an order.
4. Match compatible buy and sell orders.
5. Generate execution reports and trade records.
6. Return the results to the requesting client.

The system is designed with clear separation between the networking layer and the matching logic, allowing each component to be developed and tested independently.

## Architecture

The system is organized into the following logical components:

```text
                 +-------------------------+
                 |       TCP Clients       |
                 | CLI / Interactive Mode  |
                 +------------+------------+
                              |
                         TCP / Boost.Asio
                              |
                 +------------v------------+
                 |      Order Gateway      |
                 | Parse and Validate Input|
                 +------------+------------+
                              |
                 +------------v------------+
                 |    Request Processing   |
                 | Queue / Session Routing |
                 +------------+------------+
                              |
                 +------------v------------+
                 |     Matching Engine     |
                 | Order Matching & Reports|
                 +------------+------------+
                              |
                 +------------v------------+
                 |        Order Book       |
                 | Bids / Asks / Order Index|
                 +-------------------------+

          Supporting components: Logger, Parser,
                 CMake build and automated tests
```

The diagram represents the intended component boundaries. Queue-based processing and dedicated matching-thread execution are architectural goals for the concurrent server implementation; they should be considered complete only once implemented and tested.

### Component responsibilities

| Component | Responsibility |
|---|---|
| TCP Client | Sends orders and displays server responses |
| Order Gateway | Accepts TCP connections and handles incoming requests |
| Parser and Validator | Converts input messages into valid order requests |
| Matching Engine | Processes requests and generates matching results |
| Order Book | Maintains active orders and price-time priority |
| Logger | Records connection and request-processing events |
| Tests | Verify order-book and matching-engine behavior |

## Features

### Matching Engine

- In-memory limit order book.
- Buy and sell order management.
- Price-time priority.
- Order addition, modification, and cancellation.
- Matching of compatible buy and sell orders.
- Support for partially filled and fully filled orders.
- Execution reports and trade records.
- Integer-based price representation to avoid floating-point price comparisons.

### Networking

- TCP communication using Boost.Asio.
- Server-side connection handling.
- Text-based order requests.
- Parsing and validation of incoming messages.
- Client/server separation for independent testing.

### Software Engineering

- C++17.
- CMake-based build system.
- Modular component design.
- Logging for troubleshooting.
- A structure intended to support automated tests and future extensions.

## Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Core implementation |
| STL containers | Order-book data structures |
| Boost.Asio | TCP networking |
| Boost.Program_options | Planned/configured CLI argument handling for the client |
| CMake | Build configuration |
| Git and GitHub | Version control and project documentation |

## How Order Matching Works

The order book maintains two sides:

- **Bid book:** Buy orders, organized by descending price.
- **Ask book:** Sell orders, organized by ascending price.

At the same price level, orders are maintained in a queue to preserve time priority.

The implementation uses ordered maps and queues:

```cpp
std::map<
    std::int64_t,
    std::deque<Order>,
    std::greater<std::int64_t>
> bids;

std::map<
    std::int64_t,
    std::deque<Order>
> asks;
```

An order lookup structure provides access to active orders by order ID.

### Matching rules

1. The highest-priced buy order has priority among bids.
2. The lowest-priced sell order has priority among asks.
3. Orders at the same price are processed in arrival order.
4. A trade can occur when the best bid price is greater than or equal to the best ask price.
5. Executed quantity is the minimum of the remaining quantities of the two matching orders.
6. Remaining quantity determines whether an order is partially filled or fully filled.

For example:

- Buy order: 10 units at price 100.
- Sell order: 7 units at price 98.
- Trade quantity: 7 units.
- Remaining buy quantity: 3 units.

The precise trade price follows the matching engine's configured execution-price rule.

## Communication Protocol

The TCP interface uses newline-terminated text messages.

### Example requests

Add a buy order:

```text
ADD 1 BUY 100 10
```

This represents order ID `1`, side `BUY`, price `100`, and quantity `10`.

Add a sell order:

```text
ADD 2 SELL 98 7
```

Cancellation and modification commands must follow the syntax implemented by the server parser.

### Example responses

Execution report:

```text
REPORT 1 NEW 0 10
```

Trade record:

```text
TRADE 1 2 98 7
```

The exact response sequence depends on the order state and the matching engine's execution rules. The protocol is intentionally simple for development and testing; it is not a production exchange protocol.

## Project Structure

The following is the intended logical organization. Adjust file names to match the current repository as implementation evolves.

```text
Chrono-Exchange-Engine/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── common/
│   ├── engine/
│   ├── gateway/
│   └── utils/
├── src/
│   ├── engine/
│   │   ├── order_book.cpp
│   │   └── matching_engine.cpp
│   ├── gateway/
│   │   └── order_gateway.cpp
│   ├── utils/
│   │   ├── logger.cpp
│   │   └── parser.cpp
│   ├── client/
│   │   ├── main.cpp
│   │   └── client.cpp
│   └── main.cpp
└── tests/
```

The server and client should be separate executables, with independent entry points.

## Build and Run

### Prerequisites

- A C++17-compatible compiler.
- CMake.
- Boost libraries, including Boost.Asio dependencies.
- Git.

On Ubuntu or Debian-based Linux distributions, the dependencies can typically be installed with:

```bash
sudo apt update
sudo apt install build-essential cmake git libboost-all-dev
```

### Clone the repository

```bash
git clone <YOUR_GITHUB_REPOSITORY_URL>
cd Chrono-Exchange-Engine
```

Replace the placeholder with your actual GitHub repository URL.

### Configure and build

```bash
cmake -S . -B build
cmake --build build -j
```

### Run the server and client

The exact executable paths depend on the current CMake targets.

Start the server in one terminal:

```bash
./build/exchange_server
```

Start the client in another terminal:

```bash
./build/exchange_client
```

If the client supports command-line order submission, an example invocation could be:

```bash
./build/exchange_client --order "ADD 1 BUY 100 10"
```

These commands assume the corresponding executable names and client options have been configured in `CMakeLists.txt`. Update them if your current build uses different names or arguments.

## Example Session

A client submits a buy order:

```text
ADD 1 BUY 100 10
```

The server returns a new-order report:

```text
REPORT 1 NEW 0 10
```

The client then submits a sell order:

```text
ADD 2 SELL 98 7
```

The matching engine processes the crossing orders and produces results similar to:

```text
TRADE 1 2 98 7
```

The buy order retains a remaining quantity of three units. The actual report ordering and status messages depend on the implemented response-generation logic.

## Design Decisions

### 1. Separate networking from matching

The TCP gateway handles communication, while the matching engine owns the trading logic. This makes the core matching behavior easier to test without opening network connections.

### 2. Price-time priority

Ordered price levels and FIFO queues provide a straightforward representation of a limit order book with price-time priority.

### 3. Integer price representation

Prices are represented as integer ticks rather than floating-point values, avoiding common precision problems when comparing prices.

### 4. Explicit order lookup

An order lookup structure supports efficient identification of active orders for modification and cancellation.

### 5. Concurrency and deterministic processing

A concurrent server must protect the order book from unsynchronized access. The intended architecture is to pass validated requests through a thread-safe queue and process them through a single matching thread per instrument.

This design aims to preserve deterministic ordering while allowing network connections to be handled concurrently. It will be documented as implemented once the queue, synchronization, and response-routing behavior are complete and tested.

## Testing

Testing should cover the matching engine independently of the TCP layer, followed by integration tests for client/server communication.

Important cases include:

- Adding buy and sell orders.
- Matching orders at the same price and at crossing prices.
- Partial fills and full fills.
- Multiple orders at the same price and time-priority preservation.
- Orders that do not match immediately.
- Cancellation and modification.
- Invalid input and malformed messages.
- Multiple clients submitting orders.
- Correct response routing and safe concurrent request processing.

Automated test commands will be documented here once the test target and framework are configured in CMake.

## Roadmap

- [x] Implement the in-memory order book.
- [x] Implement core matching logic and execution results.
- [x] Add order lookup and order-management operations.
- [x] Implement initial TCP server and request parsing.
- [ ] Complete a reusable TCP client.
- [ ] Add command-line and interactive client modes.
- [ ] Add a thread-safe request queue and dedicated matching thread.
- [ ] Test concurrent client connections and response routing.
- [ ] Add automated unit and integration tests.
- [ ] Add performance benchmarks for throughput and latency.
- [ ] Add metrics and structured logging.
- [ ] Explore persistence and recovery mechanisms.

## Learning Objectives

This project is a practical exploration of:

- Modern C++ and resource management.
- STL data structures and algorithmic complexity.
- Order-book design and price-time priority.
- TCP client/server programming with Boost.Asio.
- CMake and modular project organization.
- Thread safety and producer-consumer architectures.
- Unit testing, integration testing, and performance measurement.
- Systems design trade-offs between simplicity, correctness, and latency.

## Disclaimer

Chrono Exchange Engine is an educational and portfolio project. It is not intended for production trading, and it does not provide exchange-grade reliability, regulatory compliance, persistence guarantees, or production risk controls.

## Author

Developed as a hands-on C++ systems-engineering project to explore networking, data structures, matching algorithms, and concurrent server design.

**Feedback and suggestions are welcome.**
