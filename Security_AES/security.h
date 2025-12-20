#ifndef SECURITY_H
#define SECURITY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief 安全模組定義
 * 包含 AES-128 加解密與 CRC-16 校驗介面
 */

const uint8_t *security_get_key(void);

#define AES_BLOCK_SIZE 16 // AES 標準區塊大小 (128 bits)
#define AES_KEY_SIZE 16   // AES-128 密鑰長度

/**
 * @brief AES 加密函式
 * @param plain_text  原始數據緩衝區 (長度必須是 AES_BLOCK_SIZE 的倍數)
 * @param cipher_text 輸出加密數據緩衝區
 * @param len         數據長度 (bytes)
 * @param key         16 bytes 的加密金鑰
 * @return int        0 代表成功，-1 代表長度未對齊
 */
int aes_encrypt(const uint8_t *plain_text, uint8_t *cipher_text, size_t len, const uint8_t *key);

/**
 * @brief AES 解密函式
 * @param cipher_text 加密數據緩衝區
 * @param plain_text  輸出原始數據緩衝區
 * @param len         數據長度
 * @param key         16 bytes 的金鑰
 * @return int        0 代表成功
 */
int aes_decrypt(const uint8_t *cipher_text, uint8_t *plain_text, size_t len, const uint8_t *key);

/**
 * @brief 計算數據的 CRC-16 (CCITT-FALSE)
 * @param data 數據緩衝區
 * @param len  數據長度
 * @return uint16_t 計算出的 CRC 值
 */
uint16_t protocol_crc16(const uint8_t *data, size_t len);

/**
 * @brief 驗證封包尾部的 CRC 是否正確
 * @param packet 完整封包 (含最後兩 bytes 的 CRC)
 * @param len    封包總長度
 * @return true  驗證成功
 * @return false 驗證失敗 (數據損壞)
 */
bool protocol_crc_validate(const uint8_t *packet, size_t len);

/**
 * @brief 填充數據以符合 AES 區塊大小
 * @param data   數據緩衝區
 * @param curr_len 當前長度
 * @param total_buffer_size 緩衝區總容量
 * @return size_t 填充後的總長度，失敗返回 0
 */
size_t aes_apply_padding(uint8_t *data, size_t curr_len, size_t total_buffer_size);

#endif // SECURITY_H

// need research the AES Princlpe.