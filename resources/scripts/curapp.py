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
# lbl1 = range(0, len(cx))
# fi1, ax1 = plt.subplots()
# ax1.plot(cx, cy)
# for i, txt in enumerate(lbl1):
#     ax1.annotate(txt, (cx[i], cy[i]))

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
# plt.plot(*cover_orig.xy, label='orig')
# plt.plot(*cover.xy, label='tw')
# plt.show()

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
cover_s = skew(cover_d, xs=0.5, ys=-5, origin=points[0])
ocover_s = skew(cover_d_orig, xs=0.5, ys=0.5, origin=points[0])


#%%
rc = np.array(cover_s.coords[0]) - np.array(cover_s.coords[1])
orc = np.array(cover_s.coords[0]) - np.array(cover_s.coords[1])

ocover_sc = scale(ocover_s, 0.99, 0.99, origin=cover_s.coords[0])
cover_sc = scale(cover_s, 0.96, 0.96 * np.max(rc)/np.min(rc), origin=cover_s.coords[0])

#%%
rg = np.random.default_rng(12345)
points_diff = []
opoints_diff = []
for p in cover_sc.coords:
    rx = p[0] + 1.1*rg.random()
    ry = p[1] + 1.1*rg.random()
    points_diff.append(Point(rx, ry))

for p in ocover_sc.coords:
    rx = p[0] + 0.3 * rg.random()
    ry = p[1] + 0.3 * rg.random()
    opoints_diff.append(Point(rx, ry))

mutated_cover = LineString(points_diff)
omutated_cover = LineString(opoints_diff)
#%%
inset = [Point(d['x'], d['y']) for d in data if d['line type'] == 'Inset0Type']
inset = inset[1:]
ip = Polygon(inset)

plt.plot(*mutated_cover.xy)
plt.plot(*omutated_cover.xy)
plt.plot(*ip.exterior.xy)

plt.show()

#%%
np.savetxt('coverage_mutated.dat', np.array(mutated_cover.coords), delimiter=',')
np.savetxt('border.dat', np.array(ip.exterior.coords), delimiter=',')
np.savetxt('coverage_original.dat', np.array(cover_d.coords), delimiter=',')
np.savetxt('coverage_optimal.dat', np.array(omutated_cover.coords), delimiter=',')

#%%
nees_mutated = []
nees_optimal = []
for p, m, o in zip(cover_d.coords, mutated_cover.coords, omutated_cover.coords):
    nees_mutated.append(mean_squared_error(p, m))
    nees_optimal.append(mean_squared_error(p, o))
np.savetxt('NEES_mutated.dat', np.array(nees_mutated), delimiter=',')
np.savetxt('NEES_optimal.dat', np.array(nees_optimal), delimiter=',')