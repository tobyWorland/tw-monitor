#!/usr/bin/env python3
import glob
import subprocess
import os
import sys
import matplotlib.pyplot as plt

os.chdir(os.path.dirname(sys.argv[0]) + "/..")

binutils_size = "arm-none-eabi-size"
build = "flash"
base_path = "builds/target_{}/monitor".format(build)

def get_objs(base):
    path_expr = base + "/**/*.obj"
    objs = glob.glob(path_expr, recursive=True)

    if len(objs) == 0:
        raise Exception("No object files could be found for build '{}'".format(build))

    return objs


def callsize(objpath):
    result = subprocess.run([binutils_size, objpath], capture_output=True, check=True)
    output = result.stdout.decode().strip()

    lines = output.split('\n')

    if len(lines) != 2:
        raise Exception("Only expected 2 lines from {} output, but got {}".format(binutils_size, len(lines)))

    keys = lines[0].replace('\t', ' ').split()
    values = lines[1].replace('\t', ' ').split()

    return dict(zip(keys, values))

def get_obj_file_sizes(objs):
    files = []
    sizes = []
    size_sum = 0

    for obj in objs:
        d = callsize(obj)

        filename = d['filename']
        filename = filename.removeprefix(base_path + "/CMakeFiles/monitor_{}.dir/".format(build))
        filename = filename.removesuffix(".obj")
        size = int(d['dec'])

        #print(filename, d['dec'])

        files.append(filename)
        sizes.append(size)

        size_sum += size

    return (files, sizes, size_sum)

def remove_files_less_than_x_percent(files, sizes, size_sum, percent_threshold):
    output_files = []
    output_sizes = []

    dropped_size = 0

    # Calculate percentage
    for (i, f) in enumerate(files):
        size = sizes[i]
        percent = 100 / size_sum * size

        if percent < percent_threshold:
            # print("Dropping", f, "at", percent, "%")
            dropped_size += size
        else:
            # print(f, percent)
            output_files.append(f)
            output_sizes.append(size)

    if (dropped_size > 0):
        output_files.append("Other")
        output_sizes.append(dropped_size)

    return (output_files, output_sizes)

objs = get_objs(base_path)
dont_show_objs_under_percentage = 2.00

files, sizes = remove_files_less_than_x_percent(*get_obj_file_sizes(objs), dont_show_objs_under_percentage)

fsize = 11
fig, ax = plt.subplots(figsize=(fsize, fsize))
pie = ax.pie(sizes, labels=files, autopct='%1.1f%%', rotatelabels=False)
plt.show()
