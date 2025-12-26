# Simulation  Module ('Simulation\')

## 1. Responsibility

- Build the TCP connect 
- Create the pthread in Client (`clinet_mt.c `)

- Used the mutex to lock total_requests, share menory in server(`server_mt.c `)

- Into the random parameter to package body 
(` random_fill_parameters.c`)

- Recive the client value, than display the  statistic value in server (` server_mt.c `) call (` Statistics_tool`)

## 2. API Overview

| Function                      | Role                                       |
|------------------------------|--------------------------------------------|
| `client_thread_function()`   | Create the client program for thread       |
| `run_single_client_session()`| Build the socket to connect the server, into package head & random body info |
| `run_single_process_server()`| Accept the client process, return the Z-Test statistic result |

---

### `client_thread_function()`

**Purpose:** Thread wrapper for creating concurrent client sessions

**Flow:**

1. Thread Initialization
   - Receives `ClientArg_t` structure containing connection parameters (IP, port, machine_id, send_count)
   - Casts void pointer argument to `ClientArg_t*` type

2. Session Delegation
   - Calls `run_single_client_session()` with extracted parameters
   - Passes pointer to `bad_count` for tracking failed transmissions

3. Thread Termination
   - Returns NULL after session completes
   - Thread resources freed by `pthread_join()` in main process

**Usage Context:** Spawned by main client program to enable concurrent connections from multiple simulated machines

---

### `run_single_client_session()`

**Purpose:** Establish TCP connection and transmit sensor data packets to server

**Parameters:**
- `ip` - Server IP address
- `port` - Server port number
- `machine_id` - Unique machine identifier
- `send_count` - Number of packets to send
- `bad_count` - Pointer to counter for failed transmissions

**Flow:**

1. Socket Creation
   - Creates TCP socket using `socket(AF_INET, SOCK_STREAM, 0)`
   - Configures destination address structure with provided IP and port

2. Connection Establishment
   - Executes `connect()` to server
   - If connection fails, increments `bad_count` and returns error

3. Data Preparation Loop (repeats `send_count` times)
   - Generates protocol-compliant packet structure
   - Header: Machine ID, timestamp, packet sequence number
   - Body: Random vibration data (velocity_rms_x/y/z, acceleration, temperature)
   - Calls random data generation functions to simulate sensor readings

4. Packet Transmission
   - Uses `proto_send()` to transmit formatted packet
   - Packet structure matches `ParsedData_t` format expected by server

5. Error Handling
   - Tracks transmission failures in `bad_count`
   - Implements timeout mechanisms for send operations

6. Connection Teardown
   - Closes socket after all packets sent
   - Returns status code (0 for success, -1 for failure)

**Data Format:** Adheres to custom protocol with header (machine_id, timestamp) + vibration sensor payload

---

### `run_single_process_server()`

**Purpose:** Accept client connections and perform real-time Statistical Process Control (SPC) analysis using Z-test

**Parameters:**
- `listen_fd` - Listening socket file descriptor (shared across worker processes)

**Flow:**

#### Connection Phase

1. Client Acceptance
   - Calls `accept()` on shared `listen_fd` (inherited from parent process)
   - Multiple child processes compete for incoming connections
   - Blocks until client connects

2. Connection Logging
   - Prints confirmation message with connection details

#### Data Processing Loop (infinite until client disconnects)

3. Packet Reception
   - Calls `proto_recv_and_parse()` with 3000ms timeout
   - Parses incoming bytes into `ParsedData_t` structure
   - Validates packet integrity (checksum, format compliance)

4. Error Detection
   - If `result == EOF`: Client disconnected gracefully → break loop
   - If `result != 0`: Parse error → log error with machine_id and sample count → break loop

5. Successful Parse Branch (`result == 0`)

   a. Shared Statistics Update
   - Acquires process-shared mutex lock (`pthread_mutex_lock`)
   - Increments `g_stats->total_requests` in shared memory
   - Releases mutex lock

   b. Data Extraction
   - Extracts `machine_id` from parsed packet
   - Retrieves vibration value (e.g., `velocity_rms_x`)
   - Creates `StatSample_t` structure with value, machine_id, timestamp

   c. Statistical Analysis
   - Calls `stats_add_sample()` to append data to machine-specific buffer (`vib_stats[id]`)
   - Calls `stats_compute()` to calculate Z-score using historical data

   d. Sample Count Check
   - If `count < 35`: Prints "Need More Sample" message
   - If `count >= 35`: Proceeds to threshold analysis

6. Z-Score Threshold Classification
   - Calculates absolute Z-score: `z_value = fabs(z_score)`
   - Applies industrial control thresholds:
     - **GREEN:** `z < 1.0` (Normal operation)
     - **ORANGE:** `1.0 ≤ z < 1.645` (Warning zone, ~90% confidence)
     - **RED:** `1.645 ≤ z < 1.96` (Alert zone, ~95% confidence)
     - **CRITICAL:** `z ≥ 1.96` (Out of control, ~95%+ confidence)

7. Result Output
   - Calls `stats_print_result()` to display machine ID, current sample count, mean and standard deviation, Z-score value, and classification status

#### Cleanup Phase

8. Connection Termination
   - Closes `client_sock`
   - Returns 0 for normal termination, -1 for error

**Statistical Method:** Implements Z-test for single-sample hypothesis testing against historical process mean and standard deviation

**Concurrency Model:** Multi-process architecture with process-shared mutex protecting shared memory statistics counter

---

## System Architecture

### Parent-Child Process Model

- Main process creates 4 child worker processes via `fork()`
- All children share same listening socket (SO_REUSEADDR enabled)
- Kernel distributes incoming connections across available workers
- Shared memory segment (`ServerSharedStats_t`) tracks aggregate request count
- Parent process blocks on `wait()` until all children terminate

### Thread Safety

- POSIX process-shared mutex (`PTHREAD_PROCESS_SHARED`) protects `total_requests` counter
- Per-machine statistics buffers (`vib_stats[]`) are process-local (no locking needed)

### Protocol Design

Custom binary protocol with header validation, enabling reliable sensor data transmission over TCP
