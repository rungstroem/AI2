import matplotlib.pyplot as plt;
import csv

x = [];
y = [];

with open("../AccumulatedReward.csv", 'r') as csvfile:
	plots = csv.reader(csvfile, delimiter=',');
	i = 0;
	for col in plots:
			i += 1;
			x.append(i);
			y.append(int(col[0]));

plt.plot(x,y, label="Reward over time");
plt.xlabel("Iteration");
plt.ylabel("Reward");
plt.title("Accumulated reward");
plt.legend();
plt.show();
