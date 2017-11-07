from __future__ import division
import numpy as np
import matplotlib
matplotlib.use('TkAgg')
from matplotlib import cm
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def set_axes_equal(ax, x_middle=None, y_middle=None, z_middle=None, plot_radius=None):
    '''Make axes of 3D plot have equal scale so that spheres appear as spheres,
    cubes as cubes, etc..  This is one possible solution to Matplotlib's
    ax.set_aspect('equal') and ax.axis('equal') not working for 3D.

    Input
      ax: a matplotlib axis, e.g., as output from plt.gca().
    '''
    if not ((x_middle is not None and y_middle is not None and z_middle is not
             None and plot_radius is not None) or 
            (x_middle is None and y_middle is None and z_middle is 
             None and plot_radius is None)):
        print("Failed to set equal axes")
        return

    if x_middle is None:
        x_limits = ax.get_xlim3d()
        x_range = abs(x_limits[1] - x_limits[0])
        x_middle = np.mean(x_limits)

        y_limits = ax.get_ylim3d()
        y_range = abs(y_limits[1] - y_limits[0])
        y_middle = np.mean(y_limits)

        z_limits = ax.get_zlim3d()
        z_range = abs(z_limits[1] - z_limits[0])
        z_middle = np.mean(z_limits)

        plot_radius = 0.5*max([x_range, y_range, z_range])

    ax.set_xlim3d([x_middle - plot_radius, x_middle + plot_radius])
    ax.set_ylim3d([y_middle - plot_radius, y_middle + plot_radius])
    ax.set_zlim3d([z_middle - plot_radius, z_middle + plot_radius])


# Generate uniformly distributed particles inside a sphere, then stretched into an ellipsoid.
# x^2/a^2 + y^2/b^2 + z^2/c^2 = 1
N = 1000
semiaxis = [4, 4, 1]
x, y, z = [np.random.random(3 * N) * 2 * semiax - semiax for semiax in semiaxis]
isInside = x**2/semiaxis[0]**2 + y**2/semiaxis[1]**2 + z**2/semiaxis[2]**2 <= 1
assert(np.sum(isInside) > N)
x, y, z = [x[isInside][:N], y[isInside][:N], z[isInside][:N]]

mass = np.random.random(N)

plt.style.use('dark_background')


def make_plot(figid, elev=20, azim=10):
	fig = plt.figure(figsize=[8, 6])
	ax = fig.add_subplot(111, projection='3d')

	# setup the style
	ax.set_aspect('equal')
	ax.view_init(elev, azim)    # set the view angle
	ax.set_axis_off()
	#ax.disable_mouse_rotation()


	ax.scatter(x, y, z, s=mass, c='w', depthshade=True)

	set_axes_equal(ax)

	# fig.savefig("axes3d_figs/demo_axes3d_{:05d}.png".format(figid), dpi=200)
	fig.savefig("demo_axes3d.png", dpi=200)
	plt.close()

make_plot(0)

# The following code make a sequence of figures to make a video
# for i in range(120):
	# make_plot(i, azim=i * 1)

