#!/usr/bin/env python3
import glob
import subprocess
import os
import sys

os.chdir(os.path.dirname(sys.argv[0]) + "/..")

binutils_size = "arm-none-eabi-size"
build = "flash"

path_expr = "builds/target_{}/monitor/**/*.obj".format(build)
objs = glob.glob(path_expr, recursive=True)

if len(objs) == 0:
    raise Exception("No object files could be found for build '{}'".format(build))

def callsize(objpath):
    result = subprocess.run([binutils_size, objpath], capture_output=True, check=True)
    output = result.stdout.decode().strip()

    lines = output.split('\n')

    if len(lines) != 2:
        raise Exception("Only expected 2 lines from {} output, but got {}".format(binutils_size, len(lines)))

    keys = lines[0].replace('\t', ' ').split()
    values = lines[1].replace('\t', ' ').split()
    d = dict(zip(keys, values))
    print(d['filename'], d['dec'])

for obj in objs:
    callsize(obj)

