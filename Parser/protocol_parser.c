#include <stdio.h>
#include <string.h>
#include <winsock2.h> // 小端域大端序轉換
// #include<arpa/inet.h> -> for linux system

#include "protocol_parser.h"
#include "protocol_crc_16.h"

ParserResult_t parse_protocol(const uint8_t *input, size_t len, ParsedData_t *output)
{
    /*Package buffer to small*/
    if (!input || !output || !len < sizeof(Packet_t)) //! input == (input== NULL) != Ture -> False ,False->True
    {                                                 // ! input output 防止沒有指向任何記憶體
        return PARSER_BUFFER_TO_SHORT;
        // pack to short.
    }

    /*1. need Trainsformer package sturcut*/
    const Packet_t *package = (const Packet_t *)input;
    // 將uint8_t 的資料格式轉成 Packet_t

    // 2. CRC vailation
    if (!protocol_crc_validate(input, len)) // The package content, and long

    {
        return PARSER_CRC_FAIL; // CRC check fail
                                // The CRC calculation inspection
    }

    // 3. version checek
    if (package->header.version != 1)
    {
        return PARSER_VERSION_ERROR; // version error
                                     // enum
    }

    // 4. clear output menmory space
    memset(output, 0, sizeof(Packet_t)); // int* i == 0;

    output->meachine_id = ntohl(package->header.machine_id); // Big end to transformer small end.
                                                             // -> 32bit , need the data aliemnt
    output->timestamp_sec = ntohl(package->header.timestamp_sec);
    output->seq_no = ntohs(package->header.seq_no); // short 16 bit
    output->sensor_type = package->header.sensor_type;

    /*varable switsh*/

    switch (package->header.sensor_type) // sensor type-> to switch deferent information content.
    {
    default:
        return PARSER_SENSOR_UNKNOWN;
    }

    return PARSER_OK;
}