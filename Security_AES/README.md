# Security_AES Module ('Security_AES/`)

## 1. Responsibility

- Provide AES-128 CBC encryption/decryption for `PacketBody_t.raw_bytes`.
- Hide tiny-AES-c details behind a simple C interface.
- Ensure payload length is always a multiple of 16 bytes (AES block size).


## 2. Pubilc API

This module depends on the tiny-AES-c library:
```
Step 1:
 cd ./Industrial_IoT_Sensor_System-simple-/Parser

Step 2:
 git clone  https://github.com/kokke/tiny-AES-c.git

```

API
```
int encrypt_packet_payload(const uint8_t *plain,uint8_t *cipher,size_t len,const uint8_t *key,const uint8_t iv[16]);

  - Input: plaintext buffer and length (must be multiple of 16).  
  - Output: ciphertext buffer with same length.  
  - Uses AES‑128 CBC with the provided `iv`.  
  - Return: `0` on success, `-1` if length is not aligned.

int aes_decrypt(const uint8_t *cipher_text,uint8_t *plain_text,size_t len,const uint8_t *key,const uint8_t iv[16]); 

  - Symmetric to `encrypt_packet_payload()`.  
  - Decrypts ciphertext to plaintext using same key and IV.
                
```