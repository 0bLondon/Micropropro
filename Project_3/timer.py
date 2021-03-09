#!/usr/bin/python3
import sys,os
import subprocess


def time_exec(filename):
    _, e = subprocess.Popen(['bash','-c','time "$@" 2>&1', '_', './'+filename],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
    time = e.decode()
    print("Timing for",filename)
    print(time)

def main():
    
    execs = ['tise.py',
             'plain_tise',
             'O1_tise',
             'O2_tise',
             'O3_tise',
             'Ofast_tise',]

    for exec in execs:
        time_exec(exec)
        

if __name__ == '__main__':
    main()