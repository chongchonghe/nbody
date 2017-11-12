import matplotlib.pyplot as plt
import numpy as np
import os

N = 2

#create directory for plots
#if not os.path.exists('./plots/'):
#    os.makedirs('./plots/')

data_files = [name for name in os.listdir('./data/') if name.endswith(".txt")]

x_pos = np.zeros((2, len(data_files)))
y_pos = np.zeros((2, len(data_files)))
    

for i in range(len(data_files)):  
    f_name = data_files[i]
    data = np.loadtxt('./data/'+f_name)
    
    x_pos[:, i] = data[:, 1] 
    y_pos[:, i] = data[:, 2] 

#print len(x_pos[1])  
#    fig = plt.figure()
#    #ax = Axes3D(fig)
#    plt.xlabel('x')
#    plt.xlabel('y')
#    
#    plt.xlim( (-3, 3) )
#    plt.ylim( (-3, 3) )
#    
#    x = data[:, 1] 
#    y = data[:, 2]    
#       
#    plt.scatter(x, y)
#    
#    plt.savefig('./plots/'+f_name+'.png', dpi=300)
#    plt.close()
plt.scatter(x_pos[0, :],y_pos[0, :])
plt.scatter(x_pos[1, :],y_pos[1, :])

plt.show()
