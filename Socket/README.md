# Socket Module ('Socket_tool\')

## 1. Responsibility

- Create a TCP client socket and connect to server (`net_connect`).

- Send one encrypted & CRC-protected `Packet_t` over TCP (`proto_send_packet`).

- Receive one `Packet_t`, validate CRC, decrypt body, and parse to `ParsedData_t` (`proto_recv_and_parse`).

## 2. API Overview
| Function                      | Role                                       |
|------------------------------|--------------------------------------------|
| `net_connect()`              | Create and connect TCP socket              |
| `proto_send_packet()`        | Encrypt + CRC + send one packet           |
| `proto_recv_and_parse()`     | Timeout recv + CRC check + decrypt + parse |

## 3. net_connect()
- Steps:
  1. Create TCP socket with `socket(AF_INET, SOCK_STREAM, 0)`.
  2. Fill `sockaddr_in` with `ip` and `port` (`inet_addr`, `htons`).
  3. Call `connect()` to establish connection.
  4. On success, write socket fd into `out_sock`.

## 4. proto_send_packet()

Processing pipeline for client send:

1. Copy user `Packet_t` into local `temp`.
2. Set `temp.header.body_len = sizeof(temp.body)`.
3. Generate 16‑byte random IV into `temp.header.aes_iv`.
4. Call `encrypt_packet_payload()` to encrypt `temp.body` in-place with `security_get_key()`.
5. Compute CRC16 over `temp` except `checksum`, write result (network byte order) to `temp.checksum`.
6. Loop `send()` until all `sizeof(Packet_t)` bytes are written.

Return 0 on success, -1 on any error (AES fail or send fail).

## 5. proto_recv_and_parse()

Processing pipeline for server receive:

1. Configure socket receive timeout using `setsockopt(SO_RCVTIMEO)` and `struct timeval` in milliseconds.
2. Loop `recv()` until exactly `sizeof(Packet_t)` bytes are read into a buffer.
3. Run `protocol_crc_validate()` on the full packet; if CRC fail, return -1.
4. Read `body_len` and `aes_iv` from header; decrypt body in-place with `aes_decrypt()` and `security_get_key()`.
5. Call `parse_protocol()` to convert `Packet_t` into high-level `ParsedData_t`.
6. Return 0 on success, -1 on timeout, CRC fail, AES fail, or parser error.


