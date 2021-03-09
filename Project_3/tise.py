#!/usr/bin/python3
import matplotlib.pyplot as plt
import numpy as np
import time

import warnings

L = 10
dx = .001
n_steps = int(L/dx)
x = np.linspace(0,L,n_steps)

# semi-infinite triangular potential
def V(x): 
    if x < 0: 
        return np.Inf
    else :
        return x

# second derivative
def dpsi_pdx(x, psi, psi_p,E):
    return -2*(E - V(x))*psi

#first derivative
def dpsidx(x, psi, psi_p,E):
    return psi_p

# Main algorithm
def rungeKutta(x, psi, psi_p, dx, f1, f2,E):

    k1 = dx * f1(x, psi,psi_p,E) 
    l1 = dx * f2(x, psi,psi_p,E) 
    
    k2 = dx * f1(x + .5*dx, psi+.5*l1, psi_p+.5*k1,E) 
    l2 = dx * f2(x + .5*dx, psi+.5*l1, psi_p+.5*k1,E)
    
    k3 = dx * f1(x + .5*dx, psi+.5*l2, psi_p+.5*k2,E) 
    l3 = dx * f2(x + .5*dx, psi+.5*l2, psi_p+.5*k2,E) 
    
    k4 = dx * f1(x + dx, psi+l3, psi_p + k3,E) 
    l4 = dx * f2(x + dx, psi+l3, psi_p + k3,E) 
  
    psi_p = psi_p + (1.0 / 6.0)*(k1 + 2 * k2 + 2 * k3 + k4)
    psi = psi + (1.0 / 6.0)*(l1 + 2 * l2 + 2 * l3 + l4)
    
    return [psi,psi_p] 

# Allowed energy levels:
# 1.8558807589337636
# 3.2448238992006146
# 4.381963325045131
# 5.386972869457468
# 6.305683553878785
root1_rg = 1.8558807589337636
root2_rg = 3.2448238992006146
root3_rg = 4.381963325045131
root4_rg = 5.386972869457468
root5_rg = 6.305683553878785

for E in [root1_rg, root2_rg, root3_rg, root4_rg, root5_rg]:
    # Set initial values
    psi_p = np.zeros([int(n_steps)])
    psi = np.zeros([int(n_steps)])
    psi[0]=0
    psi_p[0]=1
    
    # Perform integration
    for i in range(1,len(x)):
        [psi[i],psi_p[i]] = rungeKutta(x[i-1],psi[i-1],psi_p[i-1],dx,dpsi_pdx,dpsidx,E)

    # print(psi[:10])