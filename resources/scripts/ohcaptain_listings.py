#%%
import os

base_dir = r'/home/peer23peer/projects/HAN-thesis/thesis/resources/ohCaptain/ohCaptain'

hdrs = {}
srcs = {}

for subdir, dirs, files in os.walk(base_dir):
    for filename in files:
        filepath = subdir + os.sep + filename
        
        if filepath.endswith(".h"):
            key = filename.split('.')[0]
            hdrs[key] = filepath
        if filepath.endswith(".cpp"):
            key = filename.split('.')[0]
            srcs[key] = filepath

#%%

out = '% !TeX root = ../report.tex\n% !TeX spellcheck = en - US\n% !TeX encoding = UTF - 8\n\n\\chapter{OHCAPTAIN}\\label{app:ohcaptain source code}\n\n\\vspace{5mm}\n'

for k, v in hdrs.items():
    out += '\\RoyalFileListing{{c++}}{{{}}}{{{}.h}}\n'.format(v, k)

for k, v in srcs.items():
    out += '\\RoyalFileListing{{c++}}{{{}}}{{{}.cpp}}\n'.format(v, k)

with open('../../appendices/appendix-ohCaptain.tex', 'w') as f:
    f.write(out)
# r = '\RoyalFileListing{c++}{/home/peer23peer/projects/HAN-thesis/thesis/resources/simulation/crawlersim.cpp}{crawlersim.cpp}'