#include <stdio.h>
#include <string.h>
#include <winsock2.h> // Windows 的 htons/ntohl
#include "protocol_parser.h"
#include "protocol_crc_16.h"

int main(void)
{
    uint8_t buffer[sizeof(Packet_t)] = {0};
    Packet_t *pkt = (Packet_t *)buffer;

    // 1. 填 header
    pkt->header.version = 1;
    pkt->header.op_code = OPCODE_DATA_REPORT;
    pkt->header.sensor_type = SENSOR_TYPE_VIBRATION;
    pkt->header.machine_id = htonl(12345);
    pkt->header.timestamp_sec = htonl(1734567890);
    pkt->header.seq_no = htons(1001);

    // 2. 填 vibration payload (定點數)
    pkt->body.vibration.velocity_rms_x = htons(450); // 4.50 mm/s
    pkt->body.vibration.velocity_rms_y = htons(300); // 3.00 mm/s
    pkt->body.vibration.velocity_rms_z = htons(0);
    pkt->body.vibration.accel_peak = htons(150); // 1.50 g
    pkt->body.vibration.status_flags = 1;        // Warning

    // 3. 計算 CRC (最後兩個 byte 保留給 CRC)
    uint16_t crc = protocol_crc16(buffer, sizeof(Packet_t) - 2);
    pkt->checksum = htons(crc);

    // 4. 呼叫 parser
    ParsedData_t out;
    ParserResult_t r = parse_protocol(buffer, sizeof(Packet_t), &out);

    printf("Parser result = %d\n", r);
    if (r != PARSER_OK)
    {
        printf("Parse failed\n");
        return 1;
    }

    // 5. 檢查輸出欄位是否正確
    printf("machine_id    = %u\n", out.meachine_id);
    printf("sensor_type   = %u\n", out.sensor_type);
    printf("vib.valid     = %d\n", out.vibration.valid);
    printf("vel_x         = %.2f mm/s\n", out.vibration.velocity_rms_x);
    printf("vel_y         = %.2f mm/s\n", out.vibration.velocity_rms_y);
    printf("accel_peak    = %.2f g\n", out.vibration.accel_peak);
    printf("status_flags  = %u\n", out.vibration.status_flags);

    // 簡單斷言風格檢查
    if (out.vibration.velocity_rms_x != 4.5f)
        printf("X value mismatch!\n");
    if (out.vibration.velocity_rms_y != 3.0f)
        printf("Y value mismatch!\n");
    if (out.vibration.accel_peak != 1.5f)
        printf("Accel mismatch!\n");

    return 0;
}
