import matplotlib.pyplot as plt
import numpy as np
import csv
import scipy.stats as st
import re
from datetime import datetime
from sklearn import datasets, linear_model
from matplotlib.font_manager import FontProperties


# Funzione per creare boxplot
#@ tags: lista di nomi per la x ["sperimentale", "condivisa", ....]
#@ ic: lista di intervalli di confidenza [ (lower, upper), (lower, upper), ..... ]
#@ filename: nome del file dove salvare il grafico "pippo.png"
def create_box_plot(tags, ics, filename):

	for ic,tag in zip(ics,tags):
	    plt.plot((tag,tag),(ic[0],ic[1]),'r_-',color='b')

	plt.savefig(filename)
	plt.close()
	return

create_box_plot(["ciao", "miao", "miao1", "miao2", "miao3"], [(1,3),(2,4),(2,4),(2,4),(2,4)], "mario.png")

exit()

def hour_ic_to_list(ic):
	ic = ic[1:-1].split(",")
	splitted = ic[0].split(":")
	ic[0] = int(splitted[0])*60*60 + int(splitted[1])*60 + int(splitted[2])
	splitted = ic[1].split(":")
	ic[1] = int(splitted[0])*60*60 + int(splitted[1])*60 + int(splitted[2])
	return ic

def get_average(ic):
	return int(ic[0]) + (int(ic[1]) - int(ic[0]))/2

# fd = open('../simulation_results_ezio.csv', newline='')
# results = csv.reader(fd, delimiter=',', quotechar='"')

# num_cassieri = [i for i in range(4, 11)]


# wait_times = []
# abbandoni = []

# wait_temp = []
# abbandoni_temp = []

# tags = ["sperimentale_60_20_20"]

# average_wait = 0
# average_abbandoni = 0

# curr = "sperimentale_60_20_20"
# c = 0
# for row in results:
# 	if row[0] == "Tipo Test":
# 		continue

# 	if c != 0 and c%7 == 0:
# 		#print(c, len(wait_temp) + 1)
# 		if curr == row[1]:
# 			wait_temp.append(average_wait / 7)
# 			abbandoni_temp.append(average_abbandoni / 7)
# 			average_abbandoni = 0
# 			average_wait = 0
# 		else:
# 			#print(row)
# 			wait_temp.append(average_wait / 7)
# 			abbandoni_temp.append(average_abbandoni / 7)
# 			average_abbandoni = 0
# 			average_wait = 0

# 			wait_times.append(list(wait_temp))
# 			abbandoni.append(list(abbandoni_temp))
# 			tags.append(row[1])
# 			wait_temp = []
# 			abbandoni_temp = []
# 			curr = row[1]

# 	average_wait += get_average(hour_ic_to_list(row[4]))
# 	average_abbandoni += get_average(eval(row[7]))
# 	c += 1


# wait_temp.append(average_wait / 7)
# abbandoni_temp.append(average_abbandoni / 7)

# wait_times.append(list(wait_temp))
# abbandoni.append(list(abbandoni_temp))

# print(wait_times)
# print(tags)

# plt.xlabel("Numero di cassieri")
# plt.ylabel("Tempo medio di attesa")
# for i in range(0, len(wait_times)):

# 	plt.plot( num_cassieri , wait_times[i], label=tags[i])


# fontP = FontProperties()
# fontP.set_size('xx-small')
# plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', prop=fontP)
# plt.show()


# plt.xlabel("Numero di cassieri")
# plt.ylabel("Numero medio di abbandoni")
# for i in range(0, len(wait_times)):

# 	plt.plot( num_cassieri , abbandoni[i], label=tags[i])


# fontP = FontProperties()
# fontP.set_size('xx-small')
# plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', prop=fontP)
# plt.show()