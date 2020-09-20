numb = [[(j, i) for i in range(0, 6)] for j in range(0, 6)]


index = 0
for i in range(0, 5):
    for j in range(0, 6):
        index+=1
        print(index, "\t", i, "\t", j)
    print("-------------------")