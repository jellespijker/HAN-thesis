#%%
from sympy import symbols, cos, sin
from sympy.vector import Vector
from sympy.matrices import Matrix
import numpy as np
from sympy.solvers import solve

def omega(v_r, v_l, L):
    return (v_r - v_l)/L

def R_ICR(v_r, v_l, L):
    return L/2 * (v_r + v_l) / (v_r - v_l)

def v_t(omega, R_ICR):
    return omega * R_ICR

v_r, v_l, L = symbols('v_r v_l L')
#%%

o = omega(v_r, v_l, L)
r = R_ICR(v_r, v_l, L)

#%%
u = np.array([[v_l], [v_r]])
B = np.array([[1 / 2, 1 / 2], [0, 0], [-1 / L, 1 / L]])
B.dot(u)

v = v_t(o, r)
