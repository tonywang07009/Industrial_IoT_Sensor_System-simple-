#include "security.h"
#include <string.h>

static const uint8_t AES_KEY[AES_KEY_SIZE] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F};

const uint8_t *security_get_key(void)
{
    return AES_KEY;
}

/* AES-128 encrypt */
int encrypt_packet_payload(const uint8_t *plain,
                           uint8_t *cipher, // this way parmeter need note
                           size_t len,
                           const uint8_t *key,
                           const uint8_t iv[16])
{

    if (!plain || !cipher || !key)
    {
        return -1;
    }
    if (len % AES_BLOCK_SIZE != 0)
    {
        return -1;
    }
    /* 暫時：不做真正加密，只複製（保持介面正確） */

    struct AES_ctx ctx; // The annoucument varable.
    memcpy(cipher, plain, len);
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, cipher, len); // The real encoding

    return 0;
}

int aes_decrypt(const uint8_t *cipher_text,
                uint8_t *plain_text,
                size_t len,
                const uint8_t *key,
                const uint8_t iv[16])
{
    if (!cipher_text || !plain_text || !key)
    {
        return -1;
    }
    if (len % AES_BLOCK_SIZE != 0)
    {
        return -1;
    }
    struct AES_ctx ctx;
    memcpy(plain_text, cipher_text, len);
    AES_init_ctx_iv(&ctx, key, iv);                // set the infunction ctx need information
    AES_CBC_decrypt_buffer(&ctx, plain_text, len); // The decode
    return 0;
}
