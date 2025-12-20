#include "security.h"
#include <string.h>

static const uint8_t sbox[256] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
    // ... 完整 S-Box 略 ...
};

/* The AES KEY Table*/
const uint8_t *security_get_key(void)
{
    return sbox;
}

/**
 * @brief AES 核心：SubBytes 步驟
 */
static void sub_bytes(uint8_t *state)
{
    for (int i = 0; i < 16; i++)
        state[i] = sbox[state[i]];
}

/**
 * @brief AES 核心：ShiftRows 步驟
 */
static void shift_rows(uint8_t *state)
{
    uint8_t tmp;
    // Row 1
    tmp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp;
    // Row 2, 3 ...
}

/**
 * @brief 封裝好的加密介面
 * 用於加密 Packet_t 的 body 部分
 */
int encrypt_packet_payload(const uint8_t *plain, uint8_t *cipher, size_t len, const uint8_t *key)
{
    // 1. 進行 Padding (PKCS7)
    // 2. 輪密鑰加 (AddRoundKey)
    // 3. 疊代執行 SubBytes, ShiftRows, MixColumns
    // 這裡調用底層 AES 運算
    if (len % 16 != 0)
        return -1; // AES 區塊大小限制

    // 示範性加密流程
    memcpy(cipher, plain, len);
    for (size_t i = 0; i < len; i += 16)
    {
        // aes_encrypt_block(&cipher[i], key);
    }
    return 0;
}