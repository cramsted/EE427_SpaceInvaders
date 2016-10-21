# from __future__ import print_function
from scipy.io.wavfile import read
import numpy
import argparse

text_file = "output.c"

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name', nargs='?')
    args = parser.parse_args()

    a = read(args.file_name)
    soundRate = a[0]
    bitStream = a[1]

    with open("Output.txt", "w") as text_file:
        print("#include <stdint.h>\n\r", file=text_file)
        print("// %s \n\r" % args.file_name, file=text_file)
        print("int32_t soundRate = %d" % soundRate, file=text_file)
        print("int32_t soundFrames = %d" % bitStream.size, file=text_file)
        print("int32_t sound[] = {", file=text_file)
        for i in range(0, bitStream.size):
            # for j in range(0, 10):
            if(i != bitStream.size - 1):
                print("%d, " % bitStream[i], file=text_file, end="")
            else:
                print("%d\n};" % bitStream[i], file=text_file, end="")
