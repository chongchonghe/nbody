'''
ASTR615 - HW#4
Plot problem 1 figures using the output of the C code from the directory ./data

Authors: Mohammed Khalil and ChongChong He

Date: 11/18/17

Filename: p1_plots.py
'''

import matplotlib.pyplot as plt
import numpy as np
import os

data_files = sorted([name for name in os.listdir('./data/')])

n_points = len(data_files)
method = 'LF2' #choose method 'RK4' or 'LF2'; only changs the plots file name
e = 0.5   #use eccentricity e = 0.5 or e = 0.9
period = np.pi * np.sqrt(2 / (1 + e)**3)


t = np.linspace(0, 100*period, n_points)
x = np.zeros((n_points, 2))
y = np.zeros((n_points, 2))
vx = np.zeros((n_points, 2))
vy = np.zeros((n_points, 2))

for i in range(len(data_files)):  
    f_name = data_files[i]
    data = np.loadtxt('./data/'+f_name)
    
    #extract x and y positions and velocities
    x[i] = data[:, 1] 
    y[i] = data[:, 2] 
    vx[i] = data[:, 4]
    vy[i] = data[:, 5]     
    
#calculate r=sqrt((x1-x2)^2, (y1-y2)^2) and vr=v.r/r
r = np.sqrt( (x[:,0] - x[:,1])**2 + (y[:,0] - y[:,1])**2 )

rel_rx = x[:,0] - x[:,1]
rel_ry = y[:,0] - y[:,1]
rel_vx = vx[:,0] - vx[:,1]
rel_vy = vy[:,0] - vy[:,1]
vr = (rel_rx*rel_vx + rel_ry*rel_vy)/r

#calculate fractional change in total energy
v1 = np.sqrt(vx[:,0]**2 + vy[:,0]**2)
v2 = np.sqrt(vx[:,1]**2 + vy[:,1]**2)
energy = 0.5 * v1**2 + 0.5 * v2**2 - 1/r
E0 = -0.5 * (1 + e)
frac_energy = (energy - E0) / abs(E0)

#create directory for plots
if not os.path.exists('./plots_p1/'):
    os.makedirs('./plots_p1/')

#Orbit Plot
plt.figure()
plt.scatter(x[:, 0],y[:, 0],c='g')
plt.scatter(x[:, 1],y[:, 1])
plt.xlabel('x', fontsize = 20)
plt.ylabel('y', fontsize = 20)
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
plt.title('Orbit diagram (x vs. y) for e = '+str(e)+' using '+method, fontsize = 20)
plt.savefig('plots_p1/'+method+'_e'+str(e)+'_xy.png', dpi=400, bbox_inches='tight')
plt.show()

#phase plot 
plt.figure()
plt.scatter(r,vr)
plt.axis('equal')
plt.xlabel('Separation r', fontsize = 20)
plt.ylabel(r'Relative radial velocity $v_r$', fontsize = 20)
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
plt.title(r'Phase diagram of $v_r$ vs r for e = '+str(e)+' using '+method, fontsize = 20)
plt.savefig('plots_p1/'+method+'_e'+str(e)+'_rv.png', dpi=400, bbox_inches='tight')
plt.show()

#energy plot 
plt.figure()
plt.plot(t, frac_energy)
plt.xlabel('Time', fontsize = 20)
plt.ylabel('Fractional change in total energy', fontsize = 20)
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
plt.title('Fractional change in energy vs t for e = '+str(e)+' using '+method, fontsize = 20, y=1.04)
plt.savefig('plots_p1/'+method+'_e'+str(e)+'_energy.png', dpi=400, bbox_inches='tight')
plt.show()
