import matplotlib.pyplot as plt
import numpy as np
import csv
import scipy.stats as st
import re
from datetime import datetime
from sklearn import datasets, linear_model

fd = open('trans_time.csv', newline='')
trans_time = csv.reader(fd, delimiter=',', quotechar='"')

num_cassieri = [i for i in range(4, 11)]

for 