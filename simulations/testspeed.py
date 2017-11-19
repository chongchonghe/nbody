import os
import time

t1 = time.time()
os.system("../BH-nbody/main cluster03_2000.txt cluster03_2000_BH 2000 0.001 0.00002 100 1")
print("BH-nbody, time elapsed:", time.time() - t1)

os.system("../pp-nbody/main cluster03_2000.txt cluster03_2000_PP 2000 0.001 0.00002 100 1")
print("pp-nbody, time elapsed:", time.time() - t1)
