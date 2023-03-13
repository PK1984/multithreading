import ssl
import time

bytes_generated = 0
while True:
    try:
        rand_bytes = ssl.RAND_bytes(1024)
        bytes_generated += 1#len(rand_bytes)
    except ssl.SSLError:
        # Error indicates that the entropy pool has been exhausted
        break

print(f"Generated {bytes_generated} bytes before entropy pool exhaustion.")
