<img width="549" height="196" alt="image" src="https://i.ibb.co/S42Dzt4B/1-u-Ms06-ROBVq0-Q-5xcrc-A.jpg" alt="1-u-Ms06-ROBVq0-Q-5xcrc-A" border="0"/>


# Industrial_IoT_Sensor_System-simple (Simple Version)

## Introduction
- This project simulates a factory's digital transformation using IoT sensors and a statistical analysis DLL. 
By identifying bottlenecks with low availability or long cycle times, the system applies Industrial Engineering Line Balancing techniques. 
It recommends machine adjustments to optimize utilization rates and balance Takt time effectively.

### The member contribute 
| name           | student_id |                         work content                    | contribute % |           
|----------------|----------  |---------------------------------------------------------|--------------|
| 王界棠          | 114C73032  | Server architecture design , simluation function design |      50      |
| 蕭百淳          | 114C75024  | AES design , The ppt design , code quality check        |      50      |

## 1. Overview
- Purpose: monitor machine vibration, current, and environment, and apply SPC to detect abnormal states.
- Main modules:
    - `Socket/`: Linux -platform socket abstraction
    - `Parser/`: custom protocol
    - `Security_AES/`: AES-128 encryption/decryption for packet body
    - `Statistics_tool/`: online statistics and SPC function: Z-score
    - `Simulation/`: multi-process server + multi-thread client for load test

## 2. The system overview flow chat

```mermaid
%% {init: {"flowchart": {"defaultRenderer": "elk"}} }%%
%% The specy elk to daraw enegine
flowchart TB
    %% --- 上半部：控制層 (維持垂直) ---
    CLI[__CLI__<br/> 1.Center: specify instruction <br/> 2. opening simluation]
    SIM[__Simluation__<br/> 1.Simluation The Server <br/> 2. Simuation The client ]
    CEN[__Client__<br/> 1.Provide the Different sensor data <br/> 2.Use __pthread__ Send The package to server.]
    SVR[__Server__<br/> 1.Provide the statics result <br/> 2. Use __fork__ to handle multi-clients]


    subgraph Comm_Layer [Communication Layer]
        SCK(__Socket__<br/> Provide the The commucation)
        AES(__AES__<br/> encryption & decryption the package body <br/>)
        PAR(__Parser__ <br/> Provide the package build protocol <br/>)
        %% 設定水平順序：Client <-> Socket <-> Server

        SCK<-->AES
        SCK<-->PAR

    end

    %% --- 連線關係 ---
    CLI <--> SIM
    %% Simulation 分別呼叫 Client 與 Server
    SIM <--> CEN
    SIM <--> SVR
    SVR <--> SCK
    CEN <--> SCK
```


## 3. Protocol Design 
- Refer_code : Header_Data_Struct.h: Protocol_Header_t

### 3.1 Header (`Protocol_Header_t`)
| Field          | Type     | Description                         |
|----------------|----------|-------------------------------------|
| `version`      | uint8_t  | Protocol version                    |
| `op_code`      | uint8_t  | Operation (`OpCode_t`)             |
| `sensor_type`  | uint8_t  | Sensor type (`Sensortype_t`)       |
| `aes_iv[16]`   | uint8_t  | AES-128 IV used for body           |
| `body_len`     | uint16_t | Length of encrypted body (bytes)   |
| `seq_no`       | uint16_t | Per-device sequence number         |
| `machine_id`   | uint32_t | Machine identifier                  |
| `timestamp_sec`| uint32_t | Unix time (seconds)                |

### 3.2 Body (`Protocol_Body_t`)
- Refer_code : Body_Data_Struct.h: Protocol_Body_t

- Vibration payload (`Payload_Vibration_t`):
  - Units: velocity in 0.01 mm/s, accel in 0.01 g
- Current payload (`Payload_Current_t`):
  - Units: current in 10 mA, power in 0.1 W, energy in Wh
- Environment payload (`Payload_Env_t`):
  - Units: temperature in 0.01 °C, humidity in 0.01 %RH, light in Lux


# Document
- __[CLI](./Cli/README.md)__

- __[Simulation Module](./Simulation/README.md)__ 

- __[Socket](./Socket/README.md)__

- __[Parser](./Parser/README.md)__

- __[Log](./Log/README.md)__

- __[Security_AES](./Security_AES/README.md)__

- __[Statistics_tool](./Statistics_tool/README.md)__


## First_used

### The Security_AES setting

Step 1 : cd/ Security_AES

Step 2 : git clone https://github.com/kokke/tiny-AES-c.git (tiny-AES-c)


### Download The install log

Step 1 : sudo chmod +x install_log.sh

Step 2 : sudo ./install_log.sh
