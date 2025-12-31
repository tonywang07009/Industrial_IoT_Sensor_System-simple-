#include "server_function.h"

void server_set_shared_stats(ServerSharedStats_t *p)
{
    g_stats = p;
}

int run_single_process_server(int listen_fd)
{
    uint32_t bad_ids[MAX_MEACHINES];
    int bad_count = 0;

    net_socket_t client_sock = accept(listen_fd, NULL, NULL);

    if (client_sock == -1)
    {
        perror("accept");
        return -1;
    }

    printf("The client is connet\n");

    for (;;) // Infiniti loop
    {

        ParsedData_t data; // 宣告符合 protocol 的資料型態
        int result = proto_recv_and_parse(client_sock, &data, 3000);
        if (result == EOF)
        {
            break;
        }
        if (result != 0)
        {
            uint32_t id = data.machine_id;
            printf("recv/parse fail , break \n");
            printf("recv/parse fail at sample #%u, result=%d\n",
                   vib_stats[id].count + 1, result);
            break;
        }
        if (result == 0)
        {
            if (g_stats != NULL)
            {
                pthread_mutex_lock(&g_stats->lock);
                g_stats->total_requests++;
                pthread_mutex_unlock(&g_stats->lock);
            }
            printf("✅ The parser sussful.\n");
            protocol_print_packet(&data);
            int id = (int)data.machine_id;

            if (id < MAX_MEACHINES)
            {
                StatSample_t s = {                                        // 用法研究
                                  .value = data.vibration.velocity_rms_x, // 例如只看 X
                                  .machine_id = id,
                                  .timestamp_sec = data.timestamp_sec};

                stats_add_sample(&vib_stats[id], &s);

                StatResult_t static_value;
                if (stats_compute(&vib_stats[id], s.value, &static_value) == 0)
                {
                    if (static_value.count < 35)
                    {
                        printf("Need More Sample %u \n", (35 - static_value.count));
                    }
                    else
                    {
                        printf("Sample Enough %u", static_value.count);
                        const char *status;
                        double z_value = fabs(static_value.z_score); // fabs -> the float abs value

                        /*The value inspection*/
                        if (z_value < 1.0)
                        {
                            status = "GREEN";
                        }
                        else if (z_value <= 1.645)
                        {
                            status = "ORANGE";
                        }
                        else if (z_value <= 1.96)
                        {
                            status = "RED";
                        }
                        else
                        {
                            status = " CRITICAL ";
                        }

                        if(static_value.z_score > 1.645)
                        {
                            bad_ids[bad_count++] = id; // store bad
                            stats_print_result(id, &static_value);
                        }
                        // printf("Sample Enough %u, status=%s\n", static_value.count, status);
                    }
                }
            }
        }
    }
    printf("\n[Need Audit Machines]: ");
    /*RD*/
    for(int i = 0 ; i< bad_count;i++)
    {
        int already_printed = 0;
        for (int j = 0 ; j < i ;j++)
        {
            if (bad_ids[i] == bad_ids[j]) 
            {
                already_printed = 1;
                break;
            }
        }
        if (!already_printed) 
        {
        printf("Bad_machine : %u\n", bad_ids[i]);
        }
    }

    close(client_sock);
    return 0;
}
