# CLI Interface Module  ('CLI_Interface\')

## 1. Responsibility

- __User Configuration Managementn__ Parses and validates user inputs (IP, port, threads, package count) into a structured Cli_information object.

- __Process & Thread Orchestration:__   
  - Spawns multiple client threads using `run_multi_client_sessions` (clinet_mt.c logic).

  - Forks child processes for the server using `run_server_process` (server_mt.c logic).


- __Resource Lifecycle Control:__  
  - __Manages shared memory__ (`shm_open`,
  ` mmap`) for server statistics.

  - Handles proper cleanup of __zombie processes__ (`wait`, `signal`) and memory resources
  
- __System Monitoring & Auditing:__: 
  - Provides `cli_show_system_log` to read and display the tail of `system.log.`
  
  - Tracks "bad packages" (simulated failures) across all client threads.

## 2. API Overview

| Function                      | Role                                       |
|------------------------------|--------------------------------------------|
| `run_multi_client_sessions()`   | Validates config, creates client threads, joins them, and aggregates failure statistics. |
| `run_server_process()`| Initializes shared memory/mutex, binds socket, forks worker processes, and manages server lifecycle. |
| `cli_show_system_log()`| Reads the last __N__ lines from `system.log` using a circular buffer approach for memory efficiency. |

---

### `run_multi_client_sessions(const Cli_information *config)`

**Purpose:** Manages the lifecycle of concurrent client simulations.

**Flow:**

1. __Validation:__
   - Checks if thread_count and package_count are positive integers. Returns error 1 if invalid.

2. __Configuration Prep:__
   - Allocates arrays for thread IDs (`pthread_t`) and client arguments (`ClientArg_t`).

   - Distributes global config (`IP, port`) to individual thread configs.

   - Assigns unique `machine_id` to each thread.

3. __Thread Execution:__
   - Loops to call `pthread_create()` for each client.

   -  Passes specific `cfg[i]` to avoid race conditions on argument data.

4. __Aggregation:__
   
   - Loops to `pthread_join()` all threads to ensure completion.

   - Sums up `bad_count` from all threads to report total transmission failures.

5. __Reporting:__ 
    
   - Prints total bad packages or a success message.

---

### `run_server_process(const Cli_information* cfg)`

**Purpose:** Sets up the server environment, including IPC (Inter-Process Communication) and process forking.

**Flow:**

1. __Shared Memory Setup:__
    - Uses `shm_open()` and `ftruncate()` to create a shared memory object (SHM_NAME).
  
    - Maps it to process memory `via mmap()` to store ServerSharedStats_t.

2. __Network Initialization:__
    - Creates a TCP socket (`socket`).
  
    - Sets `SO_REUSEADDR` via `setsockopt()` to allow immediate restart on the same port.
  
    - Binds to `INADDR_ANY` and the specified port.
  
    - Starts listening with a backlog of 500.

3. __Concurrency Setup:__
    - Initializes a Process-Shared Mutex (`PTHREAD_PROCESS_SHARED`) in shared memory to protect the `total_requests` counter.

    - Logs the server start event using `syslog_info()`.

4. __Worker Forking:__

    - Forks `child_process_count (32)` worker processes.

    - __Child:__ Executes run_single_process_server() and exits.

    - __Parent:__ Waits for all children (wait()) to prevent zombie processes.
  
5. __Cleanup:__

    - Unmaps and unlinks shared memory (__shm_unlink__).

    - Closes the main server socket.


--- 

### `cli_show_system_log(void)`

**Purpose:** Displays the most recent activity from the system log file without reading the entire file into memory at once.


**Flow:**
1. Opens system.log in read mode. Handles file open errors gracefully..


2. __Circular Buffer Reading:__
  - Uses a string array __lines[LOG_TAIL_LINES]__ as a circular buffer.

  - Reads the file line-by-line using __fgets__.
  
  - Overwrites old lines cyclically: __idx = count % LOG_TAIL_LINES__.

  - Uses strdup to allocate memory for each line (and frees the old line before overwriting).

3. __Display Logic__:
   - Calculates the __starting index__ to print lines in correct chronological order.

   - Prints the last __N lines__ (up to `LOG_TAIL_LINES`).

4. Frees all allocated strings in the buffer before returning to prevent memory leaks.
