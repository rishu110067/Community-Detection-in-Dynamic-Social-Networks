# code for running cpp file of CIDLPA algorithm
import subprocess

def run_cpp(cpp_file, exe_file):
    x = subprocess.getoutput('g++' + cpp_file + '-o' + exe_file)
    if x == "":
        subprocess.run(exe_file)
    else:
        print(x)
