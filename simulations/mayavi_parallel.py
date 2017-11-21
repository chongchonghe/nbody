from __future__ import division, print_function
import sys
import os
import glob
import numpy as np
import matplotlib.pyplot as plt
#import mayavi
from mayavi import mlab
import json
try:
    import configparser as cp
except ImportError:
    import ConfigParser as cp
from mpi4py import MPI
from mpi4py.MPI import ANY_SOURCE


class Parameters():
    
    def __init__(self, iniFile):
        """ Read the init file """

        self.scale = 1.0
        self.distance = None
        self.phi = 0
        self.theta = 0
        self.resolution = 8
        self.phiChange = None
        self.thetaChange = None
        self.distanceChange = None
        self.singleview = False
        self.totalframes = 20

        if not os.path.isfile(iniFile):
            raise SystemExit("Config file '{}' does not exit! Aborting.".
                            format(iniFile))

        config = cp.ConfigParser()
        config.read(iniFile)
        #for section in config.sections():
        #    for (key, val) in config.items(section):
        #        self.__dict__(key) = val

        if config.has_option("main", "scale"):
            self.scale = config.getfloat("main", "scale")
        else:
            print("Escaping scale")

        if config.has_option("main", "distance"):
            self.distance = config.getfloat("main", "distance")
        else:
            print("Escaping distance")

        if config.has_option("main", "phi"):
            self.phi = config.getfloat("main", "phi")
        else:
            print("Escaping phi")

        if config.has_option("main", "theta"):
            self.theta = config.getfloat("main", "theta")
        else:
            print("Escaping theta")

        if config.has_option("main", "resolution"):
            self.resolution = config.getfloat("main", "resolution")
        else:
            print("Escaping resolution")

        if config.has_option("main", "focalpoint"):
            self.focalpoint = json.loads(config.get("main", "focalpoint"))
        else:
            self.focalpoint = [0, 0, 0]
            print("Escaping focalpoint")

        if config.has_option("main", "hold"):
            self.hold = config.getboolean("main", "hold")
        else:
            self.hold = False
            print("Escaping hold")

        if config.has_option("advanced", "phiChange"):
            self.phiChange = config.getfloat("advanced", "phiChange")
        else:
            print("Escaping phiChange")
            
        if config.has_option("advanced", "thetaChange"):
            self.thetaChange = config.getfloat("advanced", "thetaChange")
        else:
            print("Escaping thetaChange")

        if config.has_option("advanced", "distanceChange"):
            self.distanceChange = config.getfloat("advanced", "distanceChange")
        else:
            print("Escaping distanceChange")

        if config.has_option("singleview", "singleview"):
            self.singleview = config.getboolean("singleview",
                                                     "singleview")
        else:
            print("Escaping singleview")

        if config.has_option("singleview", "totalframes"):
            self.totalframes = config.getint("singleview",
                                                     "totalframes")
        else:
            print("Escaping totalframes")


def single_plot(dataPath, azimuth=0, elevation=0, scale=None, distance=None,
                resolution=8, index=None):
    """ Input data as [m, x, y, z] """

    # transform data into [x, y, z, m]
    data = np.loadtxt(dataPath)
    data = np.transpose(data)[:4, :]
    data = data[(1, 2, 3, 0), :]
    data[3, :] = np.log10(data[3, :]) + 4

    mlab.clf()
    mlab.points3d(*data, color=(1, 1, 1), resolution=resolution, # colormap='viridis',
                  scale_factor=scale) #, extent=[-1, 1, -1, 1, -1, 1])

    ##### control the angle of the camera #####
    # view = mlab.view()
    # roll = mlab.roll()
    # mlab.view(*view)
    # mlab.roll(roll)

    print("distance={}, azimuth={}, ele={}, reso={}".format(distance, 
            azimuth, elevation, resolution))
    mlab.view(distance=distance, azimuth=azimuth, elevation=elevation,
              focalpoint=p.focalpoint)

    # control the position of the camera
    # f = mlab.gcf()
    # camera = f.scene.camera
    # camera.zoom(1)

    # save the figure
    path1, path0 = os.path.split(os.path.abspath(dataPath))
    figDir = path1 + '_fig'
    if not os.path.isdir(figDir):
        os.system("mkdir {}".format(figDir))
    if index is None:
        fName = os.path.join(figDir, path0[:-4] + '.jpg')
    else:
        fName = os.path.join(figDir, path0[:-4] + "{:04d}.jpg".format(index))
    if not p.hold:
        mlab.savefig(fName, magnification=True, quality=100, progressive=True)
        print(fName, "saved")
    else:
        mlab.show()


def main(dataPath):

    if dataPath[-1] == '/':
        dataPath = dataPath[:-1]

    if os.path.isfile(dataPath):
        if not p.singleview:
            single_plot(dataPath, azimuth=p.phi, elevation=p.theta, scale=p.scale,
                        distance=p.distance, resolution=p.resolution)
            return
        else:
            datafiles = [dataPath] * p.totalframes
    elif os.path.isdir(dataPath):
        datafiles = sorted(glob.glob(dataPath + "/*"))
    else:
        print("file or directory {} not found".format(dataPath))
        return

    # make the plots and save them into a folder named (dataPath)_fig

    N = len(datafiles)
    if N >= 2:
        p.hold = False
        if p.phiChange is not None:
            dphi = p.phiChange / (N - 1)
        else:
            dphi = 0
        if p.thetaChange is not None:
            dtheta = p.thetaChange / (N - 1)
        else:
            dtheta = 0
        if p.distanceChange is not None:
            ddistance = p.distanceChange / (N - 1)
        else:
            ddistance = 0
    else:
        dphi = 0
        dtheta = 0
        ddistance = 0

    ##### MPI stuff #####
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    workPerRank = int(np.ceil(N / size))

    startID = workPerRank * rank
    
    p.phi += startID * dphi
    p.theta += startID * dtheta
    p.distance += startID * ddistance

    endID = min(startID + workPerRank, N)
    datafiles = datafiles[startID:endID]

    index = 0
    for fn in datafiles:
        _index = index if p.singleview else None
        single_plot(fn, azimuth=p.phi, elevation=p.theta, scale=p.scale,
                    distance=p.distance, resolution=p.resolution, index=_index)
        p.phi += dphi
        p.theta += dtheta
        p.distance += ddistance
        if p.singleview:
            index += 1
        # single_plot(fn, scale=scale, distance=distance)
        

if __name__ == "__main__":

    if len(sys.argv) < 3:
        raise SystemExit("usage: python visual.py path/to/data parameters.ini")

    dPath = sys.argv[1]
    p = Parameters(sys.argv[2])
    
    mlab.figure(size=[800, 800], bgcolor=(0, 0, 0))
    main(dPath)
