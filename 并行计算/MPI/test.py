import os
import matplotlib
from matplotlib import pyplot as plt
import numpy as  np


def run(num, core):
    l1 = []
    l2 = []
    l3 = []
    l4 = []
    r = []
    for i in range(core):
        result = os.popen('mpirun -np ' + str(i + 1) + ' --oversubscribe  ./er0 ' + str(num))
        text = result.read().split(' ')
        # print(text)
        r.append(text[0])
        l1.append(float(text[1]))
    for i in range(core):
        result = os.popen('mpirun -np ' + str(i + 1) + ' --oversubscribe  ./er1 ' + str(num))
        text = result.read().split(' ')
        # print(text)
        r.append(text[0])
        l2.append(float(text[1]))
    for i in range(core):
        result = os.popen('mpirun -np ' + str(i + 1) + ' --oversubscribe  ./er2 ' + str(num))
        text = result.read().split(' ')
        print(text)
        r.append(text[0])
        l3.append(float(text[1]))
    for i in range(core):
        result = os.popen('mpirun -np ' + str(i + 1) + ' --oversubscribe  ./er3 ' + str(num))
        text = result.read().split(' ')
        print(text)
        r.append(text[0])
        l4.append(float(text[1]))
    print(r)
    return [l1, l2, l3, l4]


def plot(r):
    plt.title("test 2")
    plt.xlabel("core num")
    plt.ylabel("time")
    x = range(1, len(r[0]) + 1)
    plt.plot(x, r[0], 'b', label='r0')
    plt.plot(x, r[1], 'g', label='r1')
    plt.plot(x, r[2], 'r', label='r2')
    plt.plot(x, r[3], 'y', label='r3')
    plt.show()


if __name__ == '__main__':
    plot(run(100000000, 8))
    # run(100000)
