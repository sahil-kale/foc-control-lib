import os
import argparse

TESTS_EXE_NAME = "foc-control-lib_tests"

C_SOURCE_TESTS_DIR = "foc-control-lib"


def main(debug, clean):
    # chdir to "application"
    os.chdir(C_SOURCE_TESTS_DIR)
    os.chdir("test")

    if clean:
        os.system("rm -rf build")

    # make a build directory
    os.system("mkdir -p build")
    os.chdir("build")

    # run "cmake ."
    cmd = r'cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"'
    os.system(cmd)
    # make the application, check that the ret code is 0
    ret = os.system("make all")
    if ret != 0:
        print("Error building the application")
        exit(1)

    if debug:
        os.system(f"gdb ./{TESTS_EXE_NAME}")
    else:
        os.system(f"./{TESTS_EXE_NAME}")


if __name__ == "__main__":
    # create an arg parser for whether to debug or not
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", help="Debug mode", action="store_true")
    parser.add_argument("--clean", help="Clean build files", action="store_true")
    args = parser.parse_args()

    main(args.debug, args.clean)
