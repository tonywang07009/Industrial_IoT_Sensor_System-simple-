#include <stdio.h>
#include <string.h>
#include "security.h"

int main(void)
{

    uint8_t plain[64];
    uint8_t cipher[64];
    uint8_t decrypted[64];

    // The into test data.

    for (int i = 0; i < 64; ++i) // why used the ++i? note
    {
        plain[i] = (uint8_t)i;
    }

    const uint8_t *key = security_get_key(); // The lock key content.

    uint8_t iv[16] = {0};

    int r1 = encrypt_packet_payload(plain,  // 明文來源
                                    cipher, // 密文輸出
                                    64,     // 長度（16 的倍數）
                                    key,
                                    iv);

    if (r1 != 0)
    {
        printf("encrypt failed: %d\n", r1);
        return 1;
    }

    int r2 = aes_decrypt(cipher,    // 密文來源
                         decrypted, // 解密後明文
                         64,
                         key,
                         iv);

    if (r2 != 0)
    {
        printf("decrypt failed: %d\n", r2);
        return 1;
    }

    if (memcmp(plain, decrypted, 64) == 0)
    {
        printf("AES round-trip OK: decrypted == original\n");
    }
    else
    {
        printf("AES round-trip FAIL: decrypted != original\n");

        for (int i = 0; i < 64; ++i) // print different byte
        {
            if (plain[i] != decrypted[i])
            {
                printf("  mismatch at %d: plain=%02X decrypted=%02X\n",
                       i, plain[i], decrypted[i]);
                break;
            }
        }
    }

    return 0;
}