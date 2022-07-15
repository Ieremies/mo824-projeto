from multiprocessing import Pool
from subprocess import Popen
from os import popen
import time
from os import listdir
from os.path import isfile, join
from itertools import product
from sys import argv

# instances = ["./inst/" + f for f in listdir("./inst/") if isfile(join("./inst/", f))]
instances = ["./inst/grid-08-10"]
alphas = ["0.10", "0.20", "0.05"]
local_search = ["1", "0"]
heuristics = ["0 0 ","1 20", "1 40", "1 80", "2 2 ", "2 3 " , "2 4 "]
target = [argv[2] for _ in range(100) ]

param = [alphas, instances, local_search, heuristics, target]
comb = product(*param)

def exe (args):
    args = ["./src/grasp_roman"] + list(args)
    print("; ".join(args), end="; ")
    stream = popen(" ".join(args) + "; ")
    print(stream.read(), end="")


if __name__ == "__main__":
    with Pool(int(argv[1])) as p:
        p.map(exe, product(alphas, instances, local_search[:1], heuristics[:1], target))
        p.map(exe, product(alphas[:1], instances, local_search[1:], heuristics[:1], target))
        p.map(exe, product(alphas[:1], instances, local_search[:1], heuristics[1:], target))
