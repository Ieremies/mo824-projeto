from multiprocessing import Pool
from subprocess import Popen
from os import popen
import time
from os import listdir
from os.path import isfile, join
from itertools import product
from sys import argv

instances = ["./inst/" + f for f in listdir("./inst/") if isfile(join("./inst/", f))]
alphas = ["0.20", "0.10", "0.05"]
local_search = ["0", "1"]
heuristics = ["0 0", "1 10", "1 20", "1 40"]


def exe (args):
    args = ["./src/grasp_roman"] + list(args)
    stream = popen(" ".join(args))
    print("; ".join(args),end="")
    print(stream.read())

comb = product(alphas, instances, local_search, heuristics)
if __name__ == "__main__":
    with Pool(int(argv[1])) as p:
        print(p.map(exe, comb))
