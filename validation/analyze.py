import matplotlib.pyplot as plt
import numpy as np
import csv
import scipy.stats as st
import re
from datetime import datetime
from sklearn import datasets, linear_model

fd = open('migliori.csv', newline='')
results = csv.reader(fd, delimiter=',', quotechar='"')

def create_bar_plot(tags, values):
	fig, ax = plt.subplots()

	for value,tag in zip(values,tags):
	    ax.bar(tag,abs(value),color='b')


	plt.ylabel("Valore della funzione")
	#ax.xaxis.tick_top()
	fig.autofmt_xdate()
	plt.show()
	#plt.savefig(filename)
	plt.close()
	return


def create_box_plot(tags, ics, filename):

	fig, ax = plt.subplots()

	ax.set_prop_cycle

	for ic,tag,i in zip(ics,tags, range(len(tags))):
	    ax.plot((tag,tag),(ic[0],ic[1]),'_-')


	plt.ylabel("Tempo di attesa (s)")
	fig.autofmt_xdate()
	plt.show()
	#plt.savefig(filename)
	plt.close()
	return

def hour_ic_to_list(ic):
	ic = ic[1:-1].split(";")
	splitted = ic[0].split(":")
	ic[0] = int(splitted[0])*60*60 + int(splitted[1])*60 + int(splitted[2])
	splitted = ic[1].split(":")
	ic[1] = int(splitted[0])*60*60 + int(splitted[1])*60 + int(splitted[2])
	return ic

def abbandoni_ic_to_list(ic):
	ic = ic[2:-2].split("; ")
	return ic

tags = []
ics_attesa = []
formula = []
ics_abbandoni = []

c = 0
for row in results:

	if row[0] == "Tipo Cinfig. Cassa":
		continue

	if c%7 == 0:
		tags.append(row[0])
		ics_attesa.append(hour_ic_to_list(row[3][3:-1]))
		ics_abbandoni.append(abbandoni_ic_to_list(row[6].strip()))
		formula.append(float(row[9].strip()))
	c +=  1

print(tags)
print(ics_attesa)
for i in range(0, len(ics_abbandoni)):
	ics_abbandoni[i][0] = float(ics_abbandoni[i][0])
	ics_abbandoni[i][1] = float(ics_abbandoni[i][1])
	if ics_abbandoni[i][0] < 0:
		ics_abbandoni[i][0] = 0

print(ics_abbandoni)


#create_box_plot(tags, ics_abbandoni, "pippo")
create_box_plot(tags, ics_attesa, "wait_box_plot.png")

#create_bar_plot(tags, formula)
