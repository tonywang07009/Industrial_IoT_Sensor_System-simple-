# Industrial_IoT_Sensor_System-simple-

### Introduction
- This project simulates a factory's digital transformation using IoT sensors and a statistical analysis DLL. 
By identifying bottlenecks with low availability or long cycle times, the system applies Industrial Engineering Line Balancing techniques. 
It recommends machine adjustments to optimize utilization rates and balance Takt time effectively.

### Protocol & Safety
1. Package formate:
    - Header:
        1. version: The package version
        2. op_code: The operation type
        3. sensor_type: The sensor recive payload
        4. aes_iv[16] : The aes check
        5. bodylen : The package content len
        6. seq_no : The package Sequence
        7. meachine_id: Now meachine_number 
        8. timestamp_sec: unix timestmep
    -

        
