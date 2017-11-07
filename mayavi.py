from __future__ import division
from mayavi import mlab
import numpy as np

# Generate uniformly distributed particles inside a sphere, then stretched into an ellipsoid.
# x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
N = 1000
semiaxis = [4, 4, 1]
x, y, z = [np.random.random(3 * N) * 2 * semiax - semiax for semiax in semiaxis]
isInside = x**2/semiaxis[0]**2 + y**2/semiaxis[1]**2 + z**2/semiaxis[2]**2 <= 1
assert(np.sum(isInside) > N)
x, y, z = [x[isInside][:N], y[isInside][:N], z[isInside][:N]]

mass = np.random.random(N)

mlab.figure(size=[1600, 1200], bgcolor=(0, 0, 0))

def make_plot(figid, azimuth=0, elevation=75):
	mlab.clf()
	mlab.points3d(x, y, z, mass, color=(1, 1, 1), resolution=4, colormap='viridis',
		scale_factor=0.08)
	mlab.view(azimuth, elevation)
	# mlab.colorbar()
	# mlab.show()
	mlab.savefig("mayavi_figs/demo_mayavi_{:05d}.jpg".format(figid), magnification=True,
		quality=100, progressive=True)

# make_plot(0)

# The following code make a sequence of figures to make a video
for i in range(120):
	make_plot(i, azimuth=i)