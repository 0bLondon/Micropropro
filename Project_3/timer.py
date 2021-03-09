#!/usr/bin/python3
import sys,os
import subprocess
import matplotlib.pyplot as plt
import numpy as np

def time_exec(filename, steps):
    x, e = subprocess.Popen(['zsh','-c','./timer "$@" 2>&1', '_', filename, str(steps)],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
    time = x.decode()
    print(float(time))
    return float(time)

def main():
    
    execs = [#'tise.py',
             'plain_tise',
             'O1_tise',
             'O2_tise',
             'O3_tise',
             'Ofast_tise',
             'opt_tise',
             'O1_opt_tise',
             'O2__opt_tise',
             'O3_opt_tise',
             'Ofast_opt_tise',]

             
    step_list = np.logspace(1,6, num = 25)
    
    plt.xscale("log")
    plt.ylim([0,.025])
    print(step_list)
    for ex in execs:
        times = []
        print("Timing for",ex)
        for steps in step_list:
            print("# Steps =", str(steps))
            times.append(time_exec(ex,steps))
        plt.plot(step_list, times, label=ex)
    plt.legend()
    
    plt.xlabel("Num Steps")
    plt.ylabel("Times (s)")
    plt.show()

if __name__ == '__main__':
    main()