import matplotlib.pyplot as plt
import numpy as np
import csv
import scipy.stats as st
import re
from datetime import datetime


def days_between(d1, d2):
    d1 = datetime.strptime(d1, "%Y-%m-%d")
    d2 = datetime.strptime(d2, "%Y-%m-%d")
    return abs((d2 - d1).days)

def get_best_distribution(data):
    dist_names = ["norm", "exponweib", "weibull_max", "weibull_min", "pareto", "genextreme"]
    dist_results = []
    params = {}
    for dist_name in dist_names:
        dist = getattr(st, dist_name)
        param = dist.fit(data)

        params[dist_name] = param
        # Applying the Kolmogorov-Smirnov test
        D, p = st.kstest(data, dist_name, args=param)
        print("p value for "+dist_name+" = "+str(p))
        dist_results.append((dist_name, p))

    # select the best fitted distribution
    best_dist, best_p = (max(dist_results, key=lambda item: item[1]))
    # store the name of the best fit and its p value

    print("Best fitting distribution: "+str(best_dist))
    print("Best p value: "+ str(best_p))
    print("Parameters for the best fit: "+ str(params[best_dist]))

    return best_dist, best_p, params[best_dist]



def plot_trans_time():
	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')

	times_1 = []
	items_1 = []

	times_8 = []
	items_8 = []
	for row in trans_time:	
		if row[0] == '1':
			try:
				times_1.append(int(row[5]))
				items_1.append(int(row[7]))
			except:
				pass
		else:
			try:
				times_8.append(int(row[5]))
				items_8.append(int(row[7]))
			except:
				pass
		

	#for i in range(0, len(items)):
	#	print(times[i], items[i])

	plt.scatter(items_1, times_1, s=5, color="r")
	plt.scatter(items_8, times_8, s=5, color="b")

	plt.show()

def plot_item_quantity():
	fd = open('item_quantity.csv', newline='')
	item_quantity = csv.reader(fd, delimiter=',', quotechar='"')

	items = [ i for i in range(1, 100)]
	quantity = [0]*99

	for row in item_quantity:
		try:
			quantity[int(row[3])-1] += 1
		except:
			pass
	plt.plot(items, quantity)
	
	plt.show()

	#get_best_distribution(quantity)


def find_full_days():
	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')

	curr_day = 0
	full_days = []
	full_check = False
	pattern = re.compile("08:\d\d:\d\d")


	for row in trans_time:
		if row[0] == 'WorkstationGroupID':
			continue
		day = row[2].split("T")[0]
		hour = row[2].split("T")[1]

		if curr_day == 0:
			curr_day = day

		if curr_day != day:
			if full_check:
				full_days.append(curr_day)
			curr_day = day
			full_check = False

		if pattern.match(hour):
			#print(hour, day)
			full_check = True

	return full_days

def plot_hour_transaction():
	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')

	full_days = find_full_days()
	hours = [i for i in range(0, 24)]
	trans = [0]*24

	for row in trans_time:
		if row[0] == 'WorkstationGroupID':
			continue
		day = row[2].split("T")[0]
		hour = row[2].split("T")[1]

		if day in full_days:
			hour = int(hour.split(":")[0])

			trans[hour] += 1

	plt.bar(hours, trans)
	
	plt.show()

	get_best_distribution(trans)

def plot_day_transaction():

	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')

	monday = "2017-12-11"

	days = ["lunedì", "martedì", "mercoledì", "giovedì", "venerdì", "sabato", "domenica"]
	trans = [0]*7

	for row in trans_time:
		if row[0] == 'WorkstationGroupID':
			continue
		day = row[2].split("T")[0]

		trans[days_between(day, monday) % 7] += 1

	plt.bar(days, trans)
	
	plt.show()	


plot_hour_transaction()