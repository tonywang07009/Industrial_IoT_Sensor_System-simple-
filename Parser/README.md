flowchart TB
    %% --- 上半部：控制層 (維持垂直) ---
    CLI[__CLI__<br/> 1.Center: specify instruction <br/> 2. opening simluation]
    SIM[__Simluation__<br/> 1.Simluation The Server <br/> 2. Simuation The client ]

    %% --- 下半部： (強制水平) ---
    subgraph Comm_Layer [Communication Layer]
        direction RL
        %% 定義節點
        CEN[__Client__<br/> 1.Provide the Different sensor data <br/> 2.Use __pthread__ Send The package to server.]
        SCK(__Socket__<br/> Provide the The commucation)
        SVR[__Server__<br/> 1.Provide the statics result <br/> 2. Use __fork__ to handle multi-clients]
        %% 設定水平順序：Client <-> Socket <-> Server
        CEN <--> SCK <--> SVR
    end

    %% --- 連線關係 ---
    CLI --> SIM
    
    %% Simulation 分別呼叫 Client 與 Server
    SIM --> CEN
    SIM --> SVR
