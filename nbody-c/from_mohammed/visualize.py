import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import sys

# load the data file and separate out the parameters
data = np.loadtxt('positions.txt')

N = 100
duration = int( len(data[:,0]) / N )

#create directory for plots
if not os.path.exists('./plots/'):
    os.makedirs('./plots/')
    
for i in range(duration):   
    fig = plt.figure()
    ax = Axes3D(fig)
    
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    
    ax.set_xlim([-5,15])
    ax.set_ylim([-5,15])
    ax.set_zlim([-5,15])

    x = data[i*N:i*N+N, 0] 
    y = data[i*N:i*N+N, 1] 
    z = data[i*N:i*N+N, 2]    
       
    ax.scatter(x, y, z)
    
    plt.savefig('./plots/plt'+str(i)+'.png', dpi=300)
    plt.close()
