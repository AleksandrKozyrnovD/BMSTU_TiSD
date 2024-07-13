import subprocess


FULLNESS = [ 10, 12, 20, 25, 30, 40, 50, 75, 85, 100 ]

SIZES = [ 10, 25, 50 ]

COUNT = 25
# FULLNESS = [ 10, 12, 20, 25, 30, 40, 50, 75, 85, 100 ]
#
# SIZES = [ 10, 25, 50, 100, 250, 500 ]
#
# COUNT = 25

for size in SIZES:
    for full in FULLNESS:
        for i in range(COUNT):
            # subprocess.run(f"'./app.exe ./data/{size}_{full}_1{1 + i}.txt ./data/{size}_{full}_2{1 + i}.txt ./data/{size}_{full}_1.txt ./data/{size}_{full}_2.txt'")
            subprocess.run(["./app.exe", f"./data/{size}_{full}_1_{i + 1}.txt", f"./data/{size}_{full}_2_{i + 1}.txt", f"./data/{size}_{full}_1.txt", f"./data/{size}_{full}_2.txt"])
        print(f"{size} with fullness {full}% done")

file = open("results.txt", "w")

file.write("Sparse vs normal\n")

for size in SIZES:
    for full in FULLNESS:
        file1 = open(f"./data/{size}_{full}_1.txt", "r")
        file2 = open(f"./data/{size}_{full}_2.txt", "r")
        file.write(f"{size} with {full}%\n")
        num1 = 0
        num2 = 0
        for i in range(COUNT):
            line1 = file1.readline().rstrip()
            line2 = file2.readline().rstrip()
            num1 += int(line1)
            num2 += int(line2)
            # file.write(line1 + " " + line2 + "\n")
        file.write(f"{num1 / COUNT:.2f} {num2 / COUNT:.2f}\n\n")
        file1.close()
        file2.close()
file.close()

