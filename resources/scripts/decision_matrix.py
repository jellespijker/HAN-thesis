#%%
from uncertain_panda import pandas as pd
# import pandas as pd
from uncertainties import *

strategies = [
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
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0.2),
               ufloat(1, 0.2),
               # ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2)]
energy_eff  = [ # How energy efficient is the algorithm in, taking in to account accelerations, decelirations, turns, revisits, drag etc
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0.2),
               ufloat(1, 0.4),
               # ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(2, 0.1)]
time  = [ # how much time
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(5, 0.2),
               ufloat(1, 0.4),
               # ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(5, 0.2),
               ufloat(2, 0.1)]
revisits    = [ # How many revisits on certain locations
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(3, 0.2)]
complexity  = [ # Implementation complexity
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(2, 0.5)]
adaptivity  = [ # Performance in a changing environment
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(4, 0.2)]
maintainability = [ # Bug fixes regression in between commits
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(2, 0.5)]
TRL          = [ # Technological Readiness Level (novel --> proven)
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(1, 0.2)]
integration  = [  # how wel it behaves with the rest of the crawler components and controllers
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(3, 0.2)]
rulesregulation  = [ # Is it allowed by law
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(2, 0.5)]
environment  = [ # Environmental impact (sound,marine life)
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(1, 0.2),
               # ufloat(1, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               # ufloat(5, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(0, 0.2),
               ufloat(2, 0.5)]
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

pugh = pd.DataFrame(data, columns=strategies)

#%%
c = [' X[2,c,m]'] * len(strategies)
out = '{\n\\noindent\n\\taburowcolors[2] 2{RoyalWhite .. RoyalLightGrey}\n\\begin{longtabu}{X[3,r,m]' + ''.join(c) + '}\n\\rowcolor{RoyalRed} \\rowfont{\\sffamily\\bfseries\\color{RoyalWhite}}\n  \\color{RoyalWhite}'
for col in pugh.columns:
    out += ' & \\color{RoyalWhite} \\rotatebox[origin=l]{90}{'
    out += '{}'.format(col)
    out += '}'
out += '\\\\\n'
for name, row in pugh.iterrows():
    out += '  \\textbf{' + str(name) + '}'
    for i in row.values:
        val = ' & \\({:.1fL}\\)'.format(i)
        val = val.replace('.0', '')
        out += val
    out += '\\\\\n'
out += '\\end{longtabu}\n}\n'

with open('../strategy_decision_matrix.tex', 'w') as f:
    f.write(out)
