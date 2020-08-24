import matplotlib.pyplot as plt
import numpy as np
import csv
import scipy.stats as st
import re
from datetime import datetime
from sklearn import datasets, linear_model


monday = "2017-12-11"

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




def plot_trans_time_old():
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
		


	best_fit = np.polyfit(items_1, times_1, 1)
	# Train the model using the training set

	# Make predictions using the testing set

	#for i in range(0, len(items)):
	#	print(times[i], items[i])

	plt.scatter(items_1, times_1, s=5, color="r")
	#plt.scatter(items_8, times_8, s=5, color="b")

	print(best_fit)
	pred_1 = [best_fit[0]*item + best_fit[1] for item in items_1]

	plt.plot(items_1, pred_1 , color="b")
	plt.xlabel("Number of items")
	plt.ylabel("Duration of transaction")
	plt.show()

def plot_trans_time(day_filter):
	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')

	times_1 = []
	items_1 = []

	times_8 = []
	items_8 = []
	for row in trans_time:

		if row[0] == 'WorkstationGroupID':
			continue
		day = row[2].split("T")[0]

		if days_between(day, monday) % 7 == day_filter:
			if row[0] == '1':
				times_1.append(int(row[5]))
				items_1.append(int(row[7]))
			else:
				times_8.append(int(row[5]))
				items_8.append(int(row[7]))
			


	best_fit = np.polyfit(items_1, times_1, 1)
	# Train the model using the training set

	# Make predictions using the testing set

	#for i in range(0, len(items)):
	#	print(times[i], items[i])

	plt.scatter(items_1, times_1, s=5, color="r")
	#plt.scatter(items_8, times_8, s=5, color="b")

	pred_1 = [best_fit[0]*item + best_fit[1] for item in items_1]

	plt.plot(items_1, pred_1 , color="b")
	plt.xlabel("Number of items for day " + str(day_filter))
	plt.ylabel("Duration of transaction")
	#plt.show()
	plt.savefig("serve_speed_" + str(day_filter) + ".png")
	plt.close()

def plot_item_quantity_old():
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
	
	plt.xlabel("Number of items")
	plt.ylabel("Number of transactions")

	plt.show()

	#get_best_distribution(quantity)

def plot_item_quantity(day_filter):
	fd = open('trans_time.csv', newline='')
	item_quantity = csv.reader(fd, delimiter=',', quotechar='"')

	items = [ i for i in range(1, 300)]
	quantity = [0]*299

	for row in item_quantity:

		if row[0] == 'WorkstationGroupID':
			continue

		day = row[2].split("T")[0]
		
		#print(day, monday)
		if day_filter == None or days_between(day, monday) % 7 == day_filter:
			#print(int(row[7])-1)
			if int(row[7])-1 < 299:
				quantity[int(row[7])-1] += 1

	plt.plot(items, quantity)
	
	plt.xlabel("Number of items for day " + str(day_filter))
	plt.ylabel("Number of transactions")

	#plt.show()
	plt.savefig('item_quantity_'+str(day_filter)+'.png')
	plt.close()
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

def plot_hour_transaction_old():
	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')

	full_days = find_full_days()
	hours = [i for i in range(0, 24)]
	trans = [0]*24
	curr_hour = 0
	total_hours = [0]*24

	for row in trans_time:
		if row[0] == 'WorkstationGroupID':
			continue
		day = row[2].split("T")[0]
		hour = row[2].split("T")[1]

		if curr_hour == 0:
			curr_hour = int(hour.split(":")[0])

		if day in full_days:
			hour = int(hour.split(":")[0])

			if curr_hour != hour:
				total_hours[curr_hour] += 1
				curr_hour = hour

			trans[hour] += 1

	
	ratio = [0]*24
	for i in range(0, len(total_hours)):
		if total_hours[i] == 0:
			ratio[i] = 0
		else:
			ratio[i] = trans[i]/total_hours[i]

	print(ratio)
	plt.bar(hours, ratio)
	

	plt.xlabel("Hour of the day")
	plt.ylabel("Average number of transactions")

	plt.show()

def plot_hour_transaction(day_filter):
	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')


	monday = "2017-12-11"
	hours = [i for i in range(0, 24)]
	trans = [0]*24
	curr_hour = 0
	total_hours = [0]*24

	for row in trans_time:
		if row[0] == 'WorkstationGroupID':
			continue
		day = row[2].split("T")[0]
		hour = row[2].split("T")[1]

		if curr_hour == 0:
			curr_hour = int(hour.split(":")[0])


		if days_between(day, monday) % 7 == day_filter:
			hour = int(hour.split(":")[0])

			if curr_hour != hour:
				total_hours[curr_hour] += 1
				curr_hour = hour

			trans[hour] += 1

	
	days = ["lun", "mar", "mer", "gio", "ven", "sab", "dom"]
	ratio = [0]*24
	for i in range(0, len(total_hours)):
		if total_hours[i] == 0:
			ratio[i] = 0
		else:
			ratio[i] = trans[i]/total_hours[i]

	#print(day_filter)
#            }else if(ore_7 < ora && ora <= ore_8){
#                media_arr = arrivi_lun_7_8;

	print("case " + days[day_filter] + ":")
	for i in range(6, len(ratio)-1):
		if i == 6:	
			print("\tif ( ore_"+str(i)+" < ora && ora <= ore_"+str(i+1)+"){")
		else:
			print("\t} else if ( ore_"+str(i)+" < ora && ora <= ore_"+str(i+1)+"){")
		
		print("\t\tmedia_arr = arrivi_"+days[day_filter]+"_"+str(i)+"_"+str(i+1)+";")
	print("\t}")
	print("\tbreak;")
		#print(float(ratio[i]), float(ratio[i+1]), (float(ratio[i])+float(ratio[i+1])))
		#print("#define arrivi_"+days[day_filter]+"_"+str(i)+"_"+str(i+1)+" %.6f" % ratio[i])
	print("")
	plt.bar(hours, ratio)
	

	plt.xlabel("Hour of the day " + str(day_filter))
	plt.ylabel("Average number of transactions")

	#plt.show()

	plt.savefig('transtaction_per_hour_normalized_'+str(day_filter)+'.png')

	plt.close()

def plot_day_transaction():

	fd = open('trans_time.csv', newline='')
	trans_time = csv.reader(fd, delimiter=',', quotechar='"')

	monday = "2017-12-11"

	days = ["lunedì", "martedì", "mercoledì", "giovedì", "venerdì", "sabato", "domenica"]
	trans = [0]*7
	total_hours = [0]*7
	curr_hour = 0

	for row in trans_time:
		if row[0] == 'WorkstationGroupID':
			continue
		day = row[2].split("T")[0]
		hour = row[2].split("T")[1]

		if curr_hour == 0:
			curr_hour = int(hour.split(":")[0])

		hour = int(hour.split(":")[0])

		if curr_hour != hour:
			total_hours[days_between(day, monday) % 7] += 1
			curr_hour = hour

		trans[days_between(day, monday) % 7] += 1

	ratio = [0]*7
	for i in range(0, len(total_hours)):
		if total_hours[i] == 0:
			ratio[i] = 0
		else:
			ratio[i] = trans[i]/total_hours[i]

	print(ratio)
	plt.bar(days, ratio)
	
	plt.xlabel("Day of the week")
	plt.ylabel("Average number of transactions per hour")
	plt.show()	

for i in range(6, 23):
	print(str(i)+":00 &", end=' ')
print("")
for i in range(0,7):
	plot_hour_transaction(i)

from math import exp,log10
from scipy.stats import norm
import random

mu = 24/60/60

#print(mu)

#values = [-mu*log10(1-random.random()) for i in range(0, 1000)]
values = [random.expovariate(mu) for i in range(0, 10000)]

#print(values)

count = 0
average = 0
time_passed = 0
total = 0
for i in range(0, len(values)):
	time_passed += values[i]
	count += 1
	
	if time_passed > 3600:
		time_passed = time_passed - 3600
		average += count
		count = 0
		total += 1

print(average / total)

	


def plot_exponential(x_range, mu=0, sigma=1, cdf=False, **kwargs):
	'''
    Plots the exponential distribution function for a given x range
    If mu and sigma are not provided, standard exponential is plotted
    If cdf=True cumulative distribution is plotted
    Passes any keyword arguments to matplotlib plot function
    '''
	x = x_range
	if cdf:
		y = st.expon.cdf(x, mu, sigma)
	else:
		y = st.expon.pdf(x, mu, sigma)
	plt.plot(x, y, **kwargs)


def compare_exponential(max_num_acquisti, samples):
	# @samples: campione di numero di acquisti non normalizzato (con zero numero di acquisti incluso)
	# @max_num_acquisti: usato pre graficare.

	num_transactions = len(samples)
	Xn = np.mean(samples)
	print("Media NON normalizzata: ", np.mean(samples))

	#normalizzo
	norm =[]
	for i in range(0, len(samples)):
		norm.append(samples[i]/float(num_transactions))

	print("Media normalizzata: ", np.mean(norm))

	print("Grafico normalizzato")
	sns.distplot(samples)
	plt.show()

	print("Esponenziale")
	x = np.linspace(0, 300, 500)

	plot_exponential(x, 0, Xn, color='red', lw=2, alpha=0.5, label="Densità reale esponenziale con media {}".format(Xn))
	plt.legend()
	plt.show()

	print("Entrambe")
	plot_exponential(x, 0, Xn, color='red', lw=3, alpha=0.5, label="Densità reale esponenziale con media {}".format(Xn))
	sns.distplot(samples, label="Distribuzione campioni prelevati (istogramma+kernel density)")
	plt.savefig('expon_proof.png')
	plt.close()


# risultati etratti da una settimana di transazioni
def plot_item_quantity_big():
	fd = open('transactions_200607.csv', newline='')
	lines = csv.reader(fd, delimiter=',', quotechar='"')

	# [gg][hh][quantita]
	transactions = []

	# 0 = lunedi, 6 = domenica
	for i in range(0, 7):
		transactions.append({})

	items = [0 for i in range(0, 100)]
	all = [0 for i in range(1,150)]
	num_elem = 0
	for line in lines:

		try:
			day = int(line[2]) - 1
			hour = line[3]
			quantity = int(line[4])
			all[quantity] += 1
			num_elem +=1
			if transactions[day].get(hour, None) is None:
				transactions[day][hour] = items.copy()
			else:
				transactions[day][hour][quantity] += 1

		except:
			pass


	#calcolo media campionaria
	compare_exponential(150, all, num_elem)



def get_item_quantity():
	fd = open('trans_time.csv', newline='')
	item_quantity = csv.reader(fd, delimiter=',', quotechar='"')

	all = []
	for row in item_quantity:

		if row[0] == 'WorkstationGroupID':
			continue

		if int(row[7])-1 < 299:
			all.append(int(row[7])-1)

	return all


q = get_item_quantity()
compare_exponential(299, q)