
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


if __name__ == "__main__":

    RandomParticle(1000)

    demo()