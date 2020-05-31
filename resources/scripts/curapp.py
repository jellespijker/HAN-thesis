#%%
import numpy as np
import pickle
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.interpolate import interp1d, interpolate
from shapely.geometry import LineString, Point, Polygon
from shapely.affinity import skew, scale, rotate
from sklearn.metrics import mean_squared_error

#%%T
data = None
with open('UM3_test1.pickle', 'rb') as f:
    data = pickle.load(f)
    
#%%
cover = []
cover_x = []
cover_y = []
travel = []
travel_x = []
travel_y = []


for r in data:
    if r['line type'] == 'InfillType':
        cover_x.append(r['x'])
        cover_y.append(r['y'])
        cover.append([r['x'], r['y']])
    if r['line type'] == 'MoveCombingType':
        travel_x.append(r['x'])
        travel_y.append(r['y'])
        travel.append([r['x'], r['y']])
        
#%%
cover_x = np.array(cover_x)
cover_y = np.array(cover_y)

#%% new sequence is

cx_1 = np.flip(cover_x[36:152])
tx_1 = np.flip(cover_x[35:36])
cx_2 = np.flip(cover_x[:35])
cx = np.append(cx_1, cx_2)

cy_1 = np.flip(cover_y[36:152])
ty_1 = np.flip(cover_y[35:36])
cy_2 = np.flip(cover_y[:35])
cy = np.append(cy_1, cy_2)

#%%

points = [(x, y) for x, y in zip(cx, cy)]
mut_points = [points[0]]
line = LineString(points)

for i in range(len(points) - 1):
    line = rotate(line, 0.05, origin=points[i])
    mut_points.append(line.coords[i + 1])
    
cover_orig = LineString(points)
cover = LineString(mut_points)

#%%

def discretize(line: LineString, n):
    deltap = np.linspace(0, line.length, n)
    points = []
    for delta in deltap:
        p = line.interpolate(delta)
        points.append(p)
    return points
    
cover_d = LineString(discretize(cover, 50000))
cover_d_orig = LineString(discretize(cover_orig, 50000))

#%%
cover_s = skew(cover_d, xs=0.5, ys=-7, origin=points[0])
ocover_s = skew(cover_d_orig, xs=0.5, ys=0.5, origin=points[0])


#%%
rc = np.array(cover_s.coords[0]) - np.array(cover_s.coords[1])
orc = np.array(cover_s.coords[0]) - np.array(cover_s.coords[1])

ocover_sc = scale(ocover_s, 0.987, 0.987, origin=cover_s.coords[0])
cover_sc = scale(cover_s, 0.97, 0.95 * np.max(rc)/np.min(rc), origin=cover_s.coords[0])

#%%
rg = np.random.default_rng(12345)
points_diff = []
opoints_diff = []
for idx, p in enumerate(cover_sc.coords):
    rx = p[0] + (2.6 + idx / 20000) * rg.random()
    ry = p[1] + (2.6 + idx / 20000) * rg.random()
    points_diff.append(Point(rx, ry))

for p in ocover_sc.coords:
    rx = p[0] + 2 * rg.random()
    ry = p[1] + 2 * rg.random()
    opoints_diff.append(Point(rx, ry))

mutated_cover = LineString(points_diff)
omutated_cover = LineString(opoints_diff)
#%%
inset = [Point(d['x'], d['y']) for d in data if d['line type'] == 'Inset0Type']
inset = inset[1:]
inset.append(Point(123.879, -108.12))
ip = Polygon(inset)

#%%
np.savetxt('coverage_mutated.dat', np.array(mutated_cover.coords), delimiter=',')
np.savetxt('border.dat', np.array(ip.exterior.coords), delimiter=',')
np.savetxt('coverage_original.dat', np.array(cover_d.coords), delimiter=',')
np.savetxt('coverage_optimal.dat', np.array(omutated_cover.coords), delimiter=',')

#%%
nees_mutated = []
nees_optimal = []
for p, m, o in zip(cover_d_orig.coords, mutated_cover.coords, omutated_cover.coords):
    nees_mutated.append(mean_squared_error(p, m))
    nees_optimal.append(mean_squared_error(p, o))
np.savetxt('NEES_mutated.dat', np.array(nees_mutated), delimiter=',')
np.savetxt('NEES_optimal.dat', np.array(nees_optimal), delimiter=',')

#%%

import pandas as pd

idx = []
for i in range(int(len(nees_optimal)/50)):
    for j in range(50):
        idx.append(i)

#%%
df_mut = pd.DataFrame({'distance': idx, 'error': nees_optimal, 'sim': ['optimal'] * len(nees_optimal)})
df_opt = pd.DataFrame({'distance': idx, 'error': nees_mutated, 'sim': ['normal'] * len(nees_optimal)})
df = pd.concat([df_opt, df_mut])

# sns.set()
# sns.set(style="whitegrid")
# sns.set_context("paper")
# f, ax = plt.subplots(figsize=(15, 6.5))
# sns.despine(f, left=True, bottom=True)
# pal = sns.color_palette("Reds", n_colors=2)
# 
# sns.lineplot(x='distance', y='error', data=df, hue='sim', palette=pal, ax=ax)

#%%
pal = sns.color_palette("Reds", n_colors=2)

pdf_orig = pd.DataFrame({'x': cover_d_orig.coords.xy[0], 'y': cover_d_orig.coords.xy[1], 'sim': ['analytic'] * len(
    cover_d_orig.coords.xy[1])})

#ox, oy, mx, my = [], [], [], []
# ox, oy, mx, my = omutated_cover.coords.xy[0][::100], omutated_cover.coords.xy[1][::100], mutated_cover.coords.xy[0][::100], \
#                  mutated_cover.coords.xy[1][::100]
ox, oy, mx, my = omutated_cover.coords.xy[0][::20], omutated_cover.coords.xy[1][::20], mutated_cover.coords.xy[0][::20], \
                 mutated_cover.coords.xy[1][::20]
# ox, oy, mx, my = ocover_sc.coords.xy[0][::100], ocover_sc.coords.xy[1][::100], cover_sc.coords.xy[0][::100], \
#                  cover_sc.coords.xy[1][::100]


#%%
from scipy import signal

b, a = signal.butter(3, 0.05)
zi = signal.lfilter_zi(b, a)
z, _ = signal.lfilter(b, a, omutated_cover.coords.xy[0], zi=zi * omutated_cover.coords.xy[0][0])
z2, _ = signal.lfilter(b, a, z, zi=zi * z[0])
fox = signal.filtfilt(b, a, omutated_cover.coords.xy[0])
foy = signal.filtfilt(b, a, omutated_cover.coords.xy[1])
fmx = signal.filtfilt(b, a, mutated_cover.coords.xy[0])
fmy = signal.filtfilt(b, a, mutated_cover.coords.xy[1])

pdf_opt = pd.DataFrame({'x': fox, 'y': foy, 'sim': ['optimal'] * len(fox)})
pdf_mut = pd.DataFrame({'x': fmx, 'y': fmy, 'sim': ['normal'] * len(fox)})
pdf = pd.concat([pdf_orig, pdf_opt, pdf_mut])
boundary = pd.DataFrame({'x': fmx, 'y': fmy, 'sim': ['normal'] * len(fox)})
sns.set()
sns.set(style="whitegrid")
sns.set_context("paper")
pal = sns.color_palette("Reds", n_colors=2)
sns.lineplot(x='distance', y='error', data=df, hue='sim', palette=pal)

sns.set()
sns.set(style="whitegrid")
sns.set_context("paper")
pal = sns.color_palette("Reds", n_colors=1)
f, ax = plt.subplots(figsize=(6.5, 6.5))
sns.despine(f, left=True, bottom=True)
pal = sns.color_palette("Reds", n_colors=1)
ax.fill(*ip.exterior.xy, color=(1., 0., 0., 0.15))
sns.lineplot(x='x', y='y', data=pdf_opt, hue='sim', sort=False, palette=pal, ax=ax)
f, ax = plt.subplots(figsize=(6.5, 6.5))
sns.despine(f, left=True, bottom=True)
pal = sns.color_palette("Reds", n_colors=1)
ax.fill(*ip.exterior.xy, color=(1., 0., 0., 0.15))
sns.lineplot(x='x', y='y', data=pdf_mut, hue='sim', sort=False, palette=pal, ax=ax)
f, ax = plt.subplots(figsize=(6.5, 6.5))
sns.despine(f, left=True, bottom=True)
pal = sns.color_palette("Reds", n_colors=1)
ax.fill(*ip.exterior.xy, color=(1., 0., 0., 0.15))
sns.lineplot(x='x', y='y', data=pdf_orig, hue='sim', sort=False, palette=pal, ax=ax)
# g = sns.FacetGrid(pdf, col="sim", palette=[pal]*3)
# g.map(sns.lineplot, 'x', 'y', size=2, linewidth=1, sort=False, color=pal[0])

#%%
# pal = sns.color_palette("Reds", n_colors=3)
# g = sns.FacetGrid(pdf, col="sim", palette=pal)
# g.map(sns.lineplot, 'x', 'y', size=2, linewidth=1, sort=False, palette=pal)

#%%
# sns.set()
# sns.set_context("paper")
# pal = sns.color_palette("Reds", n_colors=2)
# fig, ax = plt.subplots()
# sns.lineplot(x='x', y='y', data=pdf, sort=False, hue='sim', palette=pal, ax=ax)
# 
# #sns.scatterplot(x='x', y='y', data=pdf, hue='sim', ci='sd', palette=pal, ax=ax)
# sns.lineplot(x='x', y='y', data=pdf_orig, sort=False, hue='sim', palette=[(0.25, 0.25, 0.25)], size=1, ax=ax)
