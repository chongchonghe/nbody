import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os

# load the data file and separate out the parameters
N = 3

data_files = sorted([name for name in os.listdir('./data/')])

n_points = len(data_files)

x = np.zeros((n_points, N))
y = np.zeros((n_points, N))

for i in range(len(data_files)):  
    f_name = data_files[i]
    data = np.loadtxt('./data/'+f_name)
    
    #extract x and y positions and velocities
    x[i] = data[:, 1] 
    y[i] = data[:, 2] 
    
#Orbit Plot
plt.figure()
plt.title('Orbit diagram (x vs. y)', fontsize = 20)
#for t in range(n_points):
#    for i in range(N):
#        plt.scatter(x[t, i],y[t, i])
#    plt.pause(0.01)
    
for i in range(N):
    plt.scatter(x[:, i],y[:, i])
    
    
plt.xlabel('x', fontsize = 20)
plt.ylabel('y', fontsize = 20)
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
#plt.savefig('name.png', dpi=400, bbox_inches='tight')
#plt.show()
    
