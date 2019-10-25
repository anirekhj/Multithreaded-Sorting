import random
n = 500 # mil
out_file = "bigintlist.txt"
afile = open(out_file, "w")
for i in range(n):
    line = str(random.randint(1,n)) + ','
    afile.write(line)

afile.close()
