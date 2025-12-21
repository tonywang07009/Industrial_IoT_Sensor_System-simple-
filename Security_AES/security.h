#ifndef SECURITY_H
#define SECURITY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include ".\tiny-AES-c\aes.h"
/**
 * @brief 安全模組定義
 * 包含 AES-128 加解密與 CRC-16 校驗介面
 */

#define AES_BLOCK_SIZE 16 // AES 標準區塊大小 (128 bits)
#define AES_KEY_SIZE 16   // AES-128 密鑰長度

const uint8_t *security_get_key(void);
/**
 * @brief AES 加密函式
 * @param plain_text  原始數據緩衝區 (長度必須是 AES_BLOCK_SIZE 的倍數)
 * @param cipher_text 輸出加密數據緩衝區
 * @param len         數據長度 (bytes)
 * @param key         16 bytes 的加密金鑰
 * @return int        0 代表成功，-1 代表長度未對齊
 */

int encrypt_packet_payload(const uint8_t *plain,
                           uint8_t *cipher,
                           size_t len,
                           const uint8_t *key,
                           const uint8_t iv[16]); // 16 byte iv

int aes_decrypt(const uint8_t *cipher_text,
                uint8_t *plain_text,
                size_t len,
                const uint8_t *key,
                const uint8_t iv[16]);

#endif // SECURITY_H

// need research the AES Princlpe.