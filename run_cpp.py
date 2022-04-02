# code for running cpp file of CIDLPA algorithm
import os
import subprocess

def run_cpp(cpp_file, exe_file):
    cwd = os.getcwd()
    cwd1 = cwd + "\\test"
    os.chdir(cwd1)
    x = subprocess.getoutput('g++ ' + cpp_file + ' -o ' + exe_file)
    if x == "":
        subprocess.run(exe_file)
    else:
        print(x)
    os.chdir(cwd)
