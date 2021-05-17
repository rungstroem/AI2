import matplotlib.pyplot as plt;
import numpy as np

# data to plot
n_groups = 6
agent1 = (273, 277, 262, 249, 284, 269)
agent2 = (227, 223, 238, 251, 216, 231)

# create plot
fig, ax = plt.subplots()
index = np.arange(n_groups)
bar_width = 0.35
opacity = 1

rects1 = plt.bar(index, agent1, bar_width,
alpha=opacity,
color="tab:blue",
label='Agent 1')

rects2 = plt.bar(index + bar_width, agent2, bar_width,
alpha=opacity,
color="tab:orange",
label='Agent 2')
i = 1.0
j = 2000
for i in range(len(rects1)):
    plt.annotate(agent1[i], (-0.1 + i, agent1[i] + j))

plt.xlabel('Configuration')
plt.ylabel('Reward')
plt.title('Reward by configuration')
plt.xticks(index + bar_width, ('Gamma = 0.1', 'Gamma = 0.5', 'Gamma = 0.9', 'Alpha = 0.1', 'Alpha = 0.5', 'Alpha = 0.9'))
plt.legend()

plt.tight_layout()
plt.show()

