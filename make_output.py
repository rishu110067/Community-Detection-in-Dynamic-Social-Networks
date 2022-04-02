# code for creating output file

def make_output(label):
    path1 = "test/output.txt"
    path2 = "outputs/output_" + label + ".txt"
    with open(path1) as f1:
        with open(path2, "w") as f2:
            for line in f1:
                f2.write(line)
