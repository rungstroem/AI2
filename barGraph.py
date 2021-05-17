import matplotlib.pyplot as plt;
import numpy as np
import statistics

# data to plot
n_groups = 10

# Agent vs agent
#agent1 = (143+155, 148+132, 117+148, 124+156, 140+131, 134+148, 129+139, 129+151, 129+153, 135+130)
#agent2 = (102+100, 97+123, 107+128, 102+118, 112+117, 110+108, 93+139, 104+116, 97+121, 103+132)

#print(statistics.mean(agent1));
#print(statistics.mean(agent2));
#print(statistics.stdev(agent1));
#print(statistics.stdev(agent2));


# For reward func 1
#agent1 = 	(115, 133, 97, 117, 121, 117, 122, 142, 129, 134)
#rd1 = 		(125, 140, 135, 120, 125, 144, 137, 133, 120, 136)
#rd2 = 		(143, 106, 142, 150, 116, 124, 112, 122, 119, 112)
#rd3 = 		(117, 121, 126, 113, 138, 115, 129, 103, 132, 118)

# For reward func 2
agent1 = 	(128, 132, 117, 132, 145, 134, 129, 131, 146, 142)
rd1 = 		(125, 125, 125, 117, 125, 112, 131, 123, 122, 116)
rd2 = 		(125, 121, 127, 137, 122, 122, 119, 141, 116, 124)
rd3 = 		(122, 122, 131, 114, 108, 132, 121, 105, 116, 118)

print(statistics.mean(agent1));
print(statistics.mean(rd1));
print(statistics.mean(rd2));
print(statistics.mean(rd3));
print(statistics.stdev(agent1));
print(statistics.stdev(rd1));
print(statistics.stdev(rd2));
print(statistics.stdev(rd3));

# create plot
fig, ax = plt.subplots()
index = np.arange(n_groups)
bar_width = 0.2
opacity = 1

#rects1 = plt.bar(index, agent1, bar_width,
#alpha=opacity,
#color="tab:blue",
#label='Agent 1')

#rects2 = plt.bar(index + bar_width, agent2, bar_width,
#alpha=opacity,
#color="tab:orange",
#label='Agent 2')

rects1 = plt.bar(index, agent1, bar_width,
alpha=opacity,
color="tab:blue",
label='Agent 1')

rects2 = plt.bar(index+bar_width, rd1, bar_width,
alpha=opacity,
color="tab:green",
label='Random player 1')

rects3 = plt.bar(index+(bar_width*2), rd2, bar_width,
alpha=opacity,
color="tab:red",
label='Random player 2')

rects4 = plt.bar(index+(bar_width*3), rd3, bar_width,
alpha=opacity,
color="tab:orange",
label='Random player 3')


plt.xlabel('Epoch')
plt.ylabel('Wins')
plt.title('Wins per epoch - Reward function 2')
plt.xticks(index + bar_width, ('1', '2', '3', '4', '5', '6','7','8', '9', '10'))
plt.legend()

plt.tight_layout()
plt.show()

