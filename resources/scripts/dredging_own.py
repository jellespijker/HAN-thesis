#%%
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from pint import UnitRegistry

u = UnitRegistry()
u.setup_matplotlib()

#%% constants
p_atm = (1. * u.atmosphere).to('Pa')

L_p = 30. * u.m
L_s = 3. * u.m
L_a = 2.5 * u.m
theta = 10. * u.deg
theta_p = 80. * u.deg
S = 20. * u.m
H = 20. * u.m
a = S - L_a * np.sin(theta.to('rad').m)

D_s = 150. * u.mm
D_p = 150. * u.mm

epsilon_b_s = 0.1
epsilon_s_s = 0.3
epsilon_b_p = 0.1
epsilon_s_p = 0.3

rho_w = 999. * u.kg / u.m ** 3
rho_m = 1200. * u.kg / u.m ** 3
rho_s = 1400. * u.kg / u.m ** 3
mu = 1.17e-6 * u.m ** 2 / u.s

C = 1.
F_rxd = 0.501
c_t = (rho_m - rho_w) / (rho_s - rho_w)
c_t.ito('dimensionless')

g = (1. * u.g0).to_base_units()

Q = np.linspace(20, 400, 200) * u.m ** 3 / u.hr
#Q = np.linspace(0.02, 0.16, 200) * u.m ** 3 / u.s
#%% velocities

v_s = Q / (np.pi/4 * D_s**2)
v_s.ito('m/s')
v_p = Q / (np.pi/4 * D_p**2)
v_p.ito('m/s')

#%% pressureloss suction side
p_v_s = 1/2 * rho_m * v_s**2
p_v_s.ito('Pa')

p_i_s = epsilon_s_s * 1/2 * rho_m * v_s**2  # inlet
p_i_s.ito('Pa')

p_ro_s = epsilon_s_s * 1/2 * rho_m * v_s**2  # bend, valves etc
p_ro_s.ito('Pa')

p_sm_s = rho_m * g * (S-a) * C  # static head
p_sm_s.ito('Pa')

Re_s = v_s * D_s / mu
Re_s.ito('dimensionless')

labda_s = 0.0032 + 0.221/Re_s**0.237
#labda_s = 0.31 * (np.log(Re_s) - 1)**-2

F_r_s = v_s / (g * D_s)**(1/2)
F_r_s.ito('dimensionless')

psi_s = 1 + 1 * c_t * F_r_s**(-3) * F_rxd**(1.5) * np.cos(theta)

p_rp_s = labda_s * L_s/D_s * rho_w * v_s**2 #* psi_s  # straigh length of pipe
p_rp_s.ito('Pa')

p_ss = rho_w * g * S
p_ss.ito('Pa')

p_s = p_atm + p_ss - p_rp_s - p_sm_s - p_ro_s - p_i_s - p_v_s
#p_s = p_rp_s + p_sm_s + p_ro_s + p_i_s + p_v_s
#%% Pressureloss press side

p_v_p = 1/2 * rho_m * (v_p**2 - v_s**2)
#p_v_p = 1/2 * rho_m * v_p**2
p_v_p.ito('Pa')

p_ro_p = epsilon_b_p * 1/2 * rho_m * v_s **2
p_ro_p.ito('Pa')

p_sm_p = rho_m * g * (H + a)
p_sm_p.ito('Pa')

Re_p = v_p * D_p / mu
Re_p.ito('dimensionless')

labda_p = 0.0032 + 0.221 / Re_p ** 0.237
#labda_p = 0.31 * (np.log(Re_p) - 1) ** -2

F_r_p = v_p / (g * D_p) ** (1/2)
F_r_p.ito('dimensionless')

psi_p = 1 + 1 * c_t * F_r_p ** (-3) * F_rxd ** 1.5 * np.cos(theta_p)

p_rp_p = labda_p * L_p / D_p * rho_w * v_p ** 2 #* psi_p  # straigh length of pipe
p_rp_p.ito('Pa')

p_p = p_v_p + p_rp_p + p_ro_p + p_sm_p + p_atm

#%% total pressure

p_man = p_s + p_p
#p_man /= 1.7
#p_man = p_s + p_p + p_atm - p_ss

#%% plot
df_l = pd.DataFrame(dict(zip(['Q [m^3/hr]', 'p [bar]', 'system'], [Q.to('m**3/hr').m, p_man.to('bar').m, ['pipe'] * len(p_s)])))
df_p = pd.DataFrame({'Q [m^3/hr]': ([0.03, 0.085, 0.165] * u.m**3 / u.s).to('m**3/hr').m,
                     'p [bar]': ([518, 475, 270] * u.kPa).to('bar').m,
                     'system': ['TT15-55 pump'] * 3})

df = pd.concat([df_p, df_l])
sns.set()
sns.set(style="whitegrid")
sns.set_context("paper")
pal = sns.color_palette("Reds", n_colors=2)
sns.lineplot(x='Q [m^3/hr]', y='p [bar]', data=df, hue='system', palette=pal)

