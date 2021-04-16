from matplotlib import pyplot as plt
import numpy as np
import os, sys
import matplotlib as mpl
mpl.use('tkagg')
content = []
with open('value.txt') as f:
    content = f.readlines()
content = [int(x.strip()) for x in content] 

# print(content)
plt.plot(content)
plt.show()