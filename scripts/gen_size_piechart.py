#!/usr/bin/env python3
import argparse
import glob
import matplotlib.pyplot as plt
import os
import subprocess
import sys

binutils_size = "arm-none-eabi-size"

def get_objs(base, build):
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
        raise Exception("Only expected 2 lines from {} output, but got {}"
                        .format(binutils_size, len(lines)))

    keys = lines[0].replace('\t', ' ').split()
    values = lines[1].replace('\t', ' ').split()

    return dict(zip(keys, values))

def get_obj_file_sizes(base_path, build, objs):
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

def process_commandline_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--save", action="store_true", help="Save to size_${build}.png")
    parser.add_argument("build", choices=["sram", "flash"], help="Which build to get sizes from")

    args = parser.parse_args()

    return args.build, args.save

def main():
    # Change directory to the root of the repo
    os.chdir(os.path.dirname(sys.argv[0]) + "/..")

    build, save = process_commandline_args()

    base_path = "builds/target_{}/monitor".format(build)
    objs = get_objs(base_path, build)
    dont_show_objs_under_percentage = 2.00

    files, sizes = remove_files_less_than_x_percent(*get_obj_file_sizes(base_path, build, objs),
                                                    dont_show_objs_under_percentage)

    binary_size = callsize(base_path + f"/monitor_{build}")['dec']

    fig_size = 12
    fig, ax = plt.subplots(figsize=(fig_size, fig_size))
    pie = ax.pie(sizes)
    ax.pie_label(pie, files, distance=1.2)
    ax.pie_label(pie, '{absval:d}\n{frac:.1%}', distance=0.9)
    ax.set_title(f"Total: {binary_size} ({build})", fontdict={"fontsize": 20})

    if save:
        fig_filename = "size_{}.png".format(build)
        print("Saving to '" + fig_filename + "'...", end=" ")
        plt.savefig(fig_filename)
        print("Saved.")
    else:
        plt.show()

if __name__ == "__main__":
    main()
