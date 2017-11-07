from __future__ import division
import sys
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
    m = np.random.random(N)
    return [m, x[isInside][:N], y[isInside][:N], z[isInside][:N]]

def read_info(fname):
    data = np.loadtxt(fname)
    data = np.transpose(data)[:4, :]
    return data

def make_plot(data, figid=None, azimuth=0, elevation=75):
    mlab.clf()
    mlab.points3d(*data, color=(1, 1, 1), resolution=4, colormap='viridis',
                  scale_factor=0.08)
    mlab.view(azimuth, elevation)
    # mlab.colorbar()
    # mlab.show()
    fname = "mayavi_figs/demo_mayavi"
    fname += ".jpg" if figid is None else "_{:05d}.jpg".format(figid)
    mlab.savefig(fname, magnification=True, quality=100, progressive=True)


if __name__ == "__main__":

    if len(sys.argv) == 1:
        N = 1000
        data = RandomParticle(N)
    else:
        data = read_info(sys.argv[1])

    mlab.figure(size=[1600, 1200], bgcolor=(0, 0, 0))

    make_plot(data)

    # The following code make a sequence of figures to make a video
    # for i in range(120):
    #     make_plot(i, azimuth=i/3.0)
