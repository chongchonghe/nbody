#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
makemovie.py

A script to use ffmpeg to make a movie from a folder containing all the figures

@author: chongchonghe
"""

from __future__ import division, print_function
import sys
import os


def make_movie(dataPath):

    topDir, dataBasePath = os.path.split(os.path.abspath(dataPath))
    movieDir = os.path.join(topDir, "movie")
    if not os.path.isdir(movieDir):
        os.system("mkdir {}".format(movieDir))
    movieName = "{}/{}.mp4".format(movieDir, dataBasePath)
    output = os.system("ffmpeg -i {}/data_%04d.jpg -y -pix_fmt yuv420p {}".
        format(dataPath, movieName))
    # else:
    #     os.system("ffmpeg -i {}_fig/{}%04d.jpg -y -pix_fmt yuv420p {}".
    #         format(topDir, dataBasePath, movieName))
    if output == 0:
        print("\n{} saved\n".format(movieName))


if __name__ == "__main__":

    if len(sys.argv) < 2:
        raise SystemExit("usage: python makemovie.py path/to/data")

    dPath = sys.argv[1]
    make_movie(dPath)
