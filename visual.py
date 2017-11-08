from __future__ import division, print_function
import sys
import os
import glob
import numpy as np
import matplotlib.pyplot as plt
#import mayavi
from mayavi import mlab

def init(N):
    # Generate uniformly distributed particles inside a sphere, then stretched into an ellipsoid.
    # x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
    N = 1000
    semiaxis = [4, 4, 1]
    x, y, z = [np.random.random(3 * N) * 2 * semiax - semiax for semiax in semiaxis]
    isInside = x**2/semiaxis[0]**2 + y**2/semiaxis[1]**2 + z**2/semiaxis[2]**2 <= 1
    assert(np.sum(isInside) > N)
    x = x[isInside][:N]
    y = y[isInside][:N]
    z = z[isInside][:N]
    m = np.random.random(N)
    data = np.zeros([4, N])
    for i, xx in zip(range(4), [m, x, y, z]):
        data[i, :] = xx
    data = np.transpose(data)
    return data

def initLozenge(N):
    x = np.random.random(N)
    y = np.random.random(N) * 0.3
    y = y * (-x + 1)
    z = np.zeros(N)
    mass = np.ones(N)
    data = np.zeros([N, 4])
    data[:, 0] = mass
    data[:, 1] = x
    data[:, 2] = y
    data[:, 3] = z
    data4 = np.ones([4*N, 4])
    data4[:N, :] = data
    data4[N:(2*N), 1] = -data[:, 1]
    data4[N:(2*N), 2] = data[:, 2]
    data4[N:(2*N), 3] = data[:, 3]
    data4[(2*N):, 1] = data4[:(2*N), 1]
    data4[(2*N):, 2] = -data4[:(2*N), 2]
    data4[(2*N):, 3] = data4[:(2*N), 3]
    return data4

def rotate(data):
    theta = 0.1
    R = np.matrix([[np.cos(theta), -np.sin(theta), 0], 
                   [np.sin(theta), np.cos(theta), 0], 
                   [0, 0, 1]])
    data[:, 1:4] = data[:, 1:4].dot(R)
    return data

def demo():
    #data = init(1000)
    data = initLozenge(100)
    plt.plot(data[:,1], data[:,2], '.')
    plt.savefig('demo1.pdf')
    for i in range(10):
        np.savetxt("demo1/data_{:04d}.txt".format(i), data)
        data = rotate(data)

def single_plot(dataPath, azimuth=0, elevation=60):
    """ Input data as [m, x, y, z] """

    # transform data into [x, y, z, m]
    data = np.loadtxt(dataPath)
    data = np.transpose(data)[:4, :]
    data = data[(1, 2, 3, 0), :]

    mlab.clf()
    mlab.points3d(*data, color=(1, 1, 1), resolution=4, # colormap='viridis',
                  scale_factor=0.1) #, extent=[-1, 1, -1, 1, -1, 1])
    box = np.array([[-1, -1, 1, 1, -1, -1, 1, 1],
                   [-1, 1, -1, 1, -1, 1, -1, 1],
                   [-1, -1, -1, -1, 1, 1, 1, 1]])
    scale = 7
    box = scale * box
    mlab.points3d(*box, color=(0, 0, 0))

    # control the angle of the camera
    # view = mlab.view()
    # roll = mlab.roll()
    # mlab.view(*view)
    # mlab.roll(roll)
    mlab.view(azimuth=azimuth, elevation=elevation)

    # control the position of the camera
    f = mlab.gcf()
    camera = f.scene.camera
    camera.zoom(1)

    # save the figure
    path1, path0 = os.path.split(os.path.abspath(dataPath))
    figDir = path1 + '_fig'
    if not os.path.isdir(figDir):
        os.system("mkdir {}".format(figDir))
    fName = os.path.join(figDir, path0[:-4] + '.jpg')
    mlab.savefig(fName, magnification=True, quality=100, progressive=True)
    print(fName, "saved")


def main(dataPath):

    if dataPath[-1] == '/':
        dataPath = dataPath[:-1]

    if os.path.isfile(dataPath):
        single_plot(dataPath)
    elif os.path.isdir(dataPath):
        for fn in glob.glob(dataPath + "/*"):
            single_plot(fn)
        topDir, dataBasePath = os.path.split(os.path.abspath(dataPath))
        movieDir = os.path.join(topDir, "movie")
        if not os.path.isdir(movieDir):
            os.system("mkdir {}".format(movieDir))
        movieName = "{}/{}.mp4".format(movieDir, dataBasePath)
        os.system("ffmpeg -i {}_fig/data_%04d.jpg -y -pix_fmt yuv420p {}".format(dataPath, movieName))
        print("\n{} saved\n".format(movieName))


if __name__ == "__main__":

    # RandomParticle(1000)

    #demo()
    #sys.exit()

    if len(sys.argv) != 2:
        raise SystemExit("usage: python visual.py path/to/data")
    dPath = sys.argv[1]

    mlab.figure(size=[1600, 1200], bgcolor=(0, 0, 0))

    main(dPath)

    # elif os.path.isdir(dPath):
    #     for fn in glob.glob(dPath):
    #         _data = raed_info(dPath)
    #         make_plot(_data, figid=i, azimuth=i/3.0)
