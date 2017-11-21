import os
import time

t1 = time.time()
os.system("../pp-nbody/main cluster03.txt cluster03 1000 0.005 0.0005 400 1")
print("PP-nbody, time elapsed:", time.time() - t1)

t1 = time.time()
os.system("../BH-nbody/main cluster03.txt cluster03_BH 1000 0.005 0.0005 400 1")
print("BH-nbody, time elapsed:", time.time() - t1)

t1 = time.time()
os.system("../BH-nbody/main_theta3 cluster03.txt cluster03_BH_theta3 1000 0.005 0.0005 400 1")
print("BH-nbody, time elapsed:", time.time() - t1)
