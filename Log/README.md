# Log Module ('Log\')

## 1. Responsibility

- __Time Serialization__: Generates a consistent timestamp string (e.g., 2026-01-01 16:00:00) for every log entry

- __State Mapping:__ :  Maps the LogEventType (enum) to human-readable strings (e.g., "ERROR", "SERVER_START")

- __Unified Entry Point__ :  Provides `syslog_info()` as the single interface to write data. It combines metadata (time, event, port) with a user-defined message.

- __Variable Argument Handling__: Uses va_list and vfprintf to support flexible, printf-style formatting for log messages.

## 2. API Overview

| Function                      | Role                                       |
|------------------------------|--------------------------------------------|
| `make_time_string()`   | Fetches the current system time and formats it into a fixed-length string.       |
| `log_event_to_string()`| Converts the LogEventType enum into a standardized string identifier. |
| `syslog_info()`| The main wrapper function. Writes the full log entry (Header + Message) to the file. |

---

### `make_time_string(char* buffer, size_t buffer_size)`

**Purpose:** Converts the current epoch time into a readable string format within the provided buffer.

**Flow:**

1. Capture Time:
   - to get the current timestamp.

2. Convert to Struct:
   - Calls `localtime()` to fill the struct tm based on the local time zone.

3. Format: Calls:
   - `strftime()` to write the string (Format: %Y-%m-%d %H:%M:%S) into the buffer.


---

### `log_event_to_string(LogEventType type)`

**Purpose:** Standardizes event classification. This ensures that logs are machine-readable for future parsing (e.g., calculating error rates).

**Flow:**

1. Uses a switch statement to dispatch the correct string literal based on the enum value.

2.  Returns "UNKNOWN" for undefined types to prevent null pointer errors or empty logs.
---

### `syslog_info(LogEventType type, int port, int is_bad, const char *format, ...)`

**Purpose:** The public API for logging. It allows the caller to send a formatted message (like printf), while the function automatically handles the "overhead" (opening files, adding timestamps).


**Flow:**
1. calls `log_event_to_string(type)`.

2. system.log in append mode ("a"). 
   __If the file does not exist, it is created__.

3. system.log in append mode ("a"). If the file does not exist, it is created

4. Writes the standard metadata: `Time [EVENT] port=... is_bad=....`

5. __Variable Body Output__:

- Initializes va_list args.

- Uses va_start to access the variable arguments (...).

- Calls vfprintf(fp, format, args) to print the custom message to the file stream.
​

- Cleans up with va_end.

6. Writes a newline character (\n) and closes the file (`fclose`).