from __future__ import division, print_function
import sys
import os
import glob
from mayavi import mlab
import numpy as np

def RandomParticle(N):
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
    np.savetxt('data/data.txt', data)
    # return data


def single_plot(dataPath, azimuth=0, elevation=75):
    """ Input data as [m, x, y, z] """

    # transform data into [x, y, z, m]
    data = np.loadtxt(dataPath)
    data = np.transpose(data)[:4, :]
    data = data[(1, 2, 3, 0), :]

    mlab.clf()
    mlab.points3d(*data, color=(1, 1, 1), resolution=4, # colormap='viridis',
                  scale_factor=0.08)

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

    if os.path.isfile(dataPath):
        single_plot(dataPath)
    elif os.path.isdir(dataPath):
        for fn in glob.glob(dataPath + "/*"):
            single_plot(fn)
        movieDir, dataBasePath = os.path.split(dataPath)
        movieDir += "/movie"
        if not os.path.isdir(movieDir):
            os.system("mkdir {}".format(movieDir))
        movieName = "{}/{}.mp4".format(movieDir, dataBasePath)
        os.system("ffmpeg -i {}_fig/data_%04d.jpg -pix_fmt yuv420p {}".format(dataPath, movieName))
        print("\n{} saved\n".format(movieName))

if __name__ == "__main__":

    # RandomParticle(1000)

    if len(sys.argv) != 2:
        raise SystemExit("usage: python visual.py path/to/data")
    dPath = sys.argv[1]

    mlab.figure(size=[1600, 1200], bgcolor=(0, 0, 0))

    main(dPath)

    # elif os.path.isdir(dPath):
    #     for fn in glob.glob(dPath):
    #         _data = raed_info(dPath)
    #         make_plot(_data, figid=i, azimuth=i/3.0)
