import matplotlib.pyplot as plt;
import csv
from scipy.signal import butter, filtfilt

T = 10.0;
fs = 60.0;
cutoff = 10;
nyQ = 0.5*fs;
order  = 2;
n = int(T*fs);

def low_pass(data, cutoff, fs, order):
	normal_cutoff = cutoff/nyQ;
	b,a = butter(order, normal_cutoff, btype='low', analog=False);
	y = filtfilt(b,a,data);
	return y;

x = [];
y = [];
with open("./reward/AccumulatedReward8.csv", 'r') as csvfile:
	plots = csv.reader(csvfile, delimiter=',');
	i = 0;
	for col in plots:
			i += 1;
			x.append(i);
			y.append(float(col[0]));

yV2 = [];
with open("./reward/AccumulatedRewardV28.csv", 'r') as csvfile:
	plots = csv.reader(csvfile, delimiter=',');
	i = 0;
	for col in plots:
			i += 1;
			yV2.append(float(col[0]));


y = low_pass(y, 16, 1000, 2);
yV2 = low_pass(yV2, 16,1000,2);


plt.plot(x,y, label="Agent 1");
plt.plot(x,yV2, label="Agent 2");

plt.xlabel("Iteration");
plt.ylabel("Reward");
plt.title("Accumulated reward");
plt.legend();
plt.show();
