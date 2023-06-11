import sys, os
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("Usage: python3 plot.py <filename>")
    sys.exit(1)

filename = sys.argv[1]
if os.path.isfile(filename) == False:
    print("File does not exist")
    sys.exit(1)

data = []
colors = {1 : 'red', 2 : 'blue'}

# In this case, we are reading in each row instead of each column
# This is because we are not concerned about using each individual feature
# We are only concerned about iterating through each row (record) and plotting the data
with open(filename, 'r') as f:
    for line in f:
        line = line.split()
        # print(line)
        for i in range(len(line)):
            line[i] = float(line[i])
        # print(line)
        data.append(line)

features = input("Enter the features you want to plot (separated by a space): ").split()
while len(features) != 2:
    print("Please enter only 2 features")
    features = input("Enter the features you want to plot (separated by a space): ").split()

for i in range(len(features)):
    features[i] = int(features[i])
    if features[i] < 0 or features[i] >= len(data[0]):
        print(f"Feature {features[i]} does not exist")
        sys.exit(1)

if 0 in features:
    print(f"Column 0 is label! Please enter a feature from 1 to {len(data[0]) - 1}")
    sys.exit(1)

for i in data:
    plt.scatter(i[features[0]], i[features[1]], color=colors[i[0]])

plt.xlabel("Feature " + str(features[0]))
plt.ylabel("Feature " + str(features[1]))
plt.title("Plot of Features " + str(features[0]) + " and " + str(features[1]) + " of3\n" + filename)
plt.show()