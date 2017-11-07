import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os

# load the data file and separate out the parameters
N = 3
op_freq = 100

#create directory for plots
if not os.path.exists('./plots/'):
    os.makedirs('./plots/')
    
for i in range(op_freq):  
    
    data = np.loadtxt('./data/data'+str(i)+'.txt')
    
    fig = plt.figure()
    ax = Axes3D(fig)
    
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    
    ax.set_xlim([-3,3])
    ax.set_ylim([-3,3])
    ax.set_zlim([-0.5,0.5])

    x = data[:, 1] 
    y = data[:, 2] 
    z = data[:, 3]    
       
    ax.scatter(x, y, z)
    
    plt.savefig('./plots/plot'+str(i)+'.png', dpi=300)
    plt.close()
