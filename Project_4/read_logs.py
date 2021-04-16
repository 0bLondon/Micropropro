import os,sys

with open("/home/pi/mini.log") as logs:
    content = logs.readlines()

content = [x.strip() for x in content]
# content = [int(x) for x in content]
# print(sum(content)/len(content))
s = "".join(content)
for i in range(0, len(s)):
    print(s[i],end='')
    if (i % 24 == 0 and i != 0):
        print('')
# print(s)
# 0001010001100100001100111