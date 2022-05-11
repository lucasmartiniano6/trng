from random import randint

with open('data.bin', 'wb') as f:
    for _ in range(0, 1000000):
        #f.write(bytes([50]))
        f.write(bytes([randint(0,255)]))
