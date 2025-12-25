# Parser Module (`Parser/`)

## 1. Responsibility

- Decode raw `Packet_t` bytes received from `Socket/` into high-level `ParsedData_t`.
- Convert units from integer representation (e.g. 0.01 mm/s) to physical units (float).
- Provide human-readable printing for debugging and SPC verification.
- Validate packet version and basic structure before parsing.

## 2. Main Data Structures

### 2.1 `ParsedData_t`

- Common metadata:
  - `sensor_type` (from header)
  - `version`
  - `seq_no`
  - `machine_id`
  - `timestamp_sec`
- Per-sensor payloads (each with `valid` flag):
  - `vibration`: velocity X/Y/Z, accel_peak, status_flags.
  - `current`: current_rms, power_watts, total_energy_wh.
  - `env`: temperature, humidity, illuminance_lux.

> Now Function can do:
  Only one sensor_type is parsed per packet in the current implementation.

> Future extension: 
  allow one packet to contain multiple sensor payloads (e.g. vibration + current) 
  by turning the per-sensor blocks into a linked list of sensor records.

## 3. Public API
```
    ParserResult_t parse_protocol(const uint8_t *input, size_t len, ParsedData_t *output)
    
    void protocol_print_packet(const uint8_t *input, size_t len, ParsedData_t *output)
```
### 3.1 parse_protocol
    Parameters: (const uint8_t *input, size_t len, ParsedData_t *output)
        - uint8_t *input : pointer to raw `Packet_t` buffer.
        - size_t len : Total length of the received buffer, used for boundary checking.
        - ParsedData_t *output :  filled `ParsedData_t` with correct units and     `valid` flags.

        -Return: `ParserResult_t`
            - `PARSER_OK`
            - `PARSER_CRC_FAIL`
            - `PARSER_VERSION_ERROR`
            - `PARSER_SENSOR_UNKNOWN`
            - `PARSER_BUFFER_TOO_SHORT`

### 3.2 protocol_print_packe
    Parameters: (ParsedData_t *data)
        - Pretty-print content for logging and debugging.

        - No Retuen


## 4. Unit Conversions

| Sensor     | Raw type           | Scale factor | Float unit |
|-----------|--------------------|-------------:|-----------|
| Vibration | `velocity_rms_*`   | / 100.0      | mm/s      |
| Vibration | `accel_peak`       | / 100.0      | g         |
| Current   | `current_rms`      | / 100.0      | A         |
| Current   | `power_watts`      | / 10.0       | W         |
| Env       | `temperature`      | / 100.0      | °C        |
| Env       | `humidity`         | / 100.0      | %RH       |
| Env       | `illuminance_lux`  | / 1.0        | Lux       |

-tips:
    All raw values are stored in little-endian on the wire and converted using ntohs/ntohl before scaling.

## 5. Integration Points

- This module is called by proto_recv_and_parse() in the Socket layer.
- Uses:
  - `Data_struct/Package_Data_Struct.h` for `Packet_t`.
  - `Parser/protocol_crc_16.*` inside lower-level receive function.