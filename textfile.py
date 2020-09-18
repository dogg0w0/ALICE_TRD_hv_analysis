import csv
champermap_dict = {}
history_dict = {}
with open('chambermap.csv', newline='') as csvfile:
    df = csv.reader(csvfile, delimiter=",")
    for row in df:
        champermap_dict[row[0]] = row[1]
with open('delimited_history.csv', newline='') as csvfile:
    df = csv.reader(csvfile, delimiter=",")
    for row in df:
        history_dict[row[2]] = [row[2][:-13], row[0], row[1],
                                      row[2][-5:], champermap_dict[row[2][:-13]]]

with open('outfile.txt', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter=' ',
                        quotechar='|', quoting=csv.QUOTE_MINIMAL)
    for k, v in history_dict.items():
        writer.writerow(v)
