#%%
# import pandas as pd
from uncertain_panda import pandas as pd
from uncertainties import *

strategies = [
    'weight',
    # 'MCD',  # morse based cellular decomposition
    # 'BCD',  # boustrophedon based cellular decomposition
    'onBCD',  # on-line boustrophedon based cellular decomposition
    'BCD-GVD',  # boustrophedon based cellular decomposition with Generalized Voronoi Diagram
    'topBCD',  # landmark-based topological coverage with boustrophedon based cellular decomposition
    # 'SD',  # slice decomposition
    # 'SD2',  # slice decomposition II
    'onSD2',  # online slice decomposition with a topological map
    'onSD2NN',  # # online slice decomposition with a topological map landmarks recognized with neural netwerk
    # 'GB',  # grid based
    'STC',  # grid based with spanning trees
    'SSTC',  # grid based spiral spanning trees
    'NN',  # grid based neural network
]

#%% add online criterium
online      = [ # Is the algorithm suitable for online usage (Leave this out since I allready filter on it?)
                3,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0),
               ufloat(5, 0),
               ufloat(5, 0),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0),
               ufloat(5, 0),
               # ufloat(5, 0.2),
               ufloat(5, 0),
               ufloat(5, 0),
               ufloat(5, 0)]
energy_eff  = [ # How energy efficient is the algorithm in, taking in to account accelerations, decelirations, turns, revisits, drag etc
               1,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(4, 1),
               ufloat(4, 1),
               ufloat(4, 0.5),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 1),
               ufloat(1, 0.5),
               # ufloat(5, 0.2),,
               ufloat(3, 1),
               ufloat(3, 1),
               ufloat(2, 2)]
time  = [ # how much time
                2,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 2),
               ufloat(5, 2),
               ufloat(5, 1),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 2.5),
               ufloat(1, 2),
               # ufloat(5, 0.2),
               ufloat(3, 2),
               ufloat(3, 2),
               ufloat(2, 1)]
revisits    = [ # How many revisits on certain locations
                3,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(1, 0.2),
               ufloat(1, 0.2),
               ufloat(1, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(1, 0.2),
               ufloat(1, 0.2),
               # ufloat(5, 0.2),
               ufloat(1, 0.2),
               ufloat(1, 0.2),
               ufloat(1, 0.2),
               ufloat(1, 0.2),
               ufloat(3, 0.2)]
complexity  = [ # Implementation complexity
                3,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 1),
               ufloat(5, 1),
               ufloat(4, 0.5),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(1, 1),
               ufloat(1, 2),
               # ufloat(5, 0.2),
               ufloat(1, 1),
               ufloat(1, 2),
               ufloat(2, 0.5)]
adaptivity  = [ # Performance in a changing environment
                1,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(2, 1),
               ufloat(3, 1),
               ufloat(3, 1),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0.5),
               ufloat(5, 0.5),
               # ufloat(5, 0.2),
               ufloat(3, 2),
               ufloat(3, 2),
               ufloat(4, 1)]
maintainability = [ # Bug fixes regression in between commits
                2,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 2),
               ufloat(5, 2),
               ufloat(4, 1),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(3, 1),
               ufloat(3, 1),
               # ufloat(5, 0.2),
               ufloat(2, 1),
               ufloat(2, 1),
               ufloat(2, 2)]
TRL          = [ # Technological Readiness Level (novel --> proven)
                3,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 1),
               ufloat(5, 1),
               ufloat(5, 0.5),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(2, 2),
               ufloat(2, 2),
               # ufloat(5, 0.2),
               ufloat(3, 1),
               ufloat(3, 1),
               ufloat(1, 3)]
integration  = [  # how wel it behaves with the rest of the crawler components and controllers
                1,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(4, 0.5),
               ufloat(4, 0.5),
               ufloat(4, 0.5),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(2, 1),
               ufloat(2, 1),
               # ufloat(5, 0.2),
               ufloat(4, 2),
               ufloat(4, 2),
               ufloat(4, 2)]
rulesregulation  = [ # Is it allowed by law
                1,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(3, 2),
               ufloat(3, 2),
               ufloat(3, 2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(2, 3),
               ufloat(2, 3),
               # ufloat(5, 0.2),
               ufloat(3, 2),
               ufloat(3, 2),
               ufloat(2, 2)]
environment  = [ # Environmental impact (sound,marine life)
                1,
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(3, 1),
               ufloat(3, 1),
               ufloat(3, 1),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(2, 1),
               ufloat(2, 1),
               # ufloat(5, 0.2),
               ufloat(3, 1),
               ufloat(3, 1),
               ufloat(2, 1)]
#%%
data = [
    pd.Series(data=dict(zip(strategies, online)), name='online'),
    pd.Series(data=dict(zip(strategies, time)), name='time'),
    pd.Series(data=dict(zip(strategies, energy_eff)), name='energy eff.'),
    pd.Series(data=dict(zip(strategies, revisits)), name='revisits'),
    pd.Series(data=dict(zip(strategies, complexity)), name='complexity'),
    pd.Series(data=dict(zip(strategies, adaptivity)), name='adaptivity'),
    pd.Series(data=dict(zip(strategies, TRL)), name='TRL'),
    pd.Series(data=dict(zip(strategies, maintainability)), name='maintainability'),
    pd.Series(data=dict(zip(strategies, integration)), name='integration'),
    pd.Series(data=dict(zip(strategies, rulesregulation)), name='legislation'),
    pd.Series(data=dict(zip(strategies, environment)), name='environment')
]

#%%
pugh = pd.DataFrame(data, columns=strategies)
df_1 = pugh[pugh['weight'] == 1][pugh.columns[1:]].apply(lambda x: x)
df_2 = pugh[pugh['weight'] == 2][pugh.columns[1:]].apply(lambda x: x * 2)
df_3 = pugh[pugh['weight'] == 3][pugh.columns[1:]].apply(lambda x: x * 3)
pg = pd.concat([df_1, df_2, df_3])
sigma = pg.sum()

#%%
c = [' X[3,c,m]'] * (len(strategies) - 1)
out = '{\n\\noindent\n\\taburowcolors[2] 2{RoyalWhite .. RoyalLightGrey}\n\\begin{longtabu}{X[7,r,m] X[1,cm]' + ''.join(c) + '}\n\\rowcolor{RoyalRed} \\rowfont{\\sffamily\\bfseries\\color{RoyalWhite}}\n  \\color{RoyalWhite}'
for col in pugh.columns:
    out += ' & \\color{RoyalWhite} \\rotatebox[origin=l]{90}{'
    out += '{}'.format(col)
    out += '}'
out += '\\\\\n'
for name, row in pugh.iterrows():
    out += '  \\textbf{' + str(name) + '}'
    for i in row.values:
        if isinstance(i, int):
            val = ' & \\({:.1f}\\)'.format(i)
        else:
            val = ' & \\({:.1fL}\\)'.format(i)
        val = val.replace('.0', '')
        out += val
    out += '\\\\\n'
out += '\\rowcolor{RoyalRed} \\rowfont{\\sffamily\\bfseries\\color{RoyalWhite}}\n  \\color{RoyalWhite} \\(\\Sigma\\) & '
for _, col in sigma.items():
    out += '&  \\color{{RoyalWhite}} {:.1fL}'.format(col)
out += '\n\\end{longtabu}\n}\n'

with open('../strategy_decision_matrix.tex', 'w') as f:
    f.write(out)
