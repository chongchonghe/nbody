try:
    import configparser as cp
except ImportError:
    import ConfigParser as cp

config = cp.ConfigParser()
config.read("basic.ini")

if config.has_option("main", "resolution"):
	resolution = config.getfloat("main", "resolution")
print("resolution =", resolution)