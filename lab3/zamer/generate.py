import random as rand

FULLNESS = [ 10, 12, 20, 25, 30, 40, 50, 75, 85, 100 ]

SIZES = [ 10, 25, 50 ]

COUNT = 25

for size in SIZES:
    for full in FULLNESS:
        file = open(f"./data/{size}_{full}_1.txt", "w")
        file.close()
        file = open(f"./data/{size}_{full}_2.txt", "w")
        file.close()
        for i in range(COUNT):
            file1 = open(f"./data/{size}_{full}_1_{i + 1}.txt", "w")
            file2 = open(f"./data/{size}_{full}_2_{i + 1}.txt", "w")
            file1.write(f"{size} {size} {int((size * size) * (full / 100))}\n")
            file2.write(f"{size} {size} {int((size * size) * (full / 100))}\n")
            for _ in range(int((size * size) * (full / 100))):
                file1.write(f"{rand.randint(0, size - 1)} {rand.randint(0, size - 1)} {rand.randint(-size * size, size * size)}\n")
                file2.write(f"{rand.randint(0, size - 1)} {rand.randint(0, size - 1)} {rand.randint(-size * size, size * size)}\n")
            file1.close()
            file2.close()
        print(f"{size} with fullness {full}% done")

