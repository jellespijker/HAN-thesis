# %%
import os

import pandas as pd

# %% parsing the csv
gls_dfs = []
path = '../glossary/'
for filename in os.listdir(path):
    if filename.endswith("english.csv") or filename.endswith('organizations.csv') or filename.endswith('ships.csv'):
        gls_dfs.append(pd.read_csv('{}{}'.format(path, filename), delimiter='|'))

# %%
df = pd.concat(gls_dfs)

# %% template symbols
file_temp = '% !TeX root = ../../report.tex\n% !TeX spellcheck = en-US\n% !TeX encoding = UTF-8\n\\ProvidesFile{{glossary.tex}}\n\glsnoexpandfields\n{}\\endinput\n'
# templ_quant = """\\newglossaryentry{{{label}}}%
# {{%
#   name={{{mathalias}}},%
#   symbol={{\ensuremath{{{symbol}}}}},%
#   sort={{{sortlabel}}},%
#   type={{symbolslist}},%
#   category={{mathtooltip}},%
#   description={{{desc}}},%
#   unit={{\ensuremath{{{unit}}}}}%
# }}\n"""
templ_quant = """\\glsxtrnewsymbol[description = {{{desc}}},unit={{{unit}}}]{{{label}}}{{\ensuremath{{{symbol}}}}}\n"""

# %% template acro

templ_acr = """\\newacronym{{{short}}}{{{shrt}}}{{{long}}}\n"""

# %% template gls
templ_gls = """\\newglossaryentry{{{label}}}%
{{%
  name={{{name}}},%
  sort={{{sortlabel}}},%
  category={{regular}},%
  description={{{desc}}}%
}}\n"""
# %%

symb = {}
for _, r in df[df['symbol'].notnull()].iterrows():
    label = 'sym-{}'.format(r.key)
    mathalias = r.key.replace('_', ' ').replace('\\', '').replace('{', '').replace('}', '')
    symbol =  r.symbol
    sortlabel = r.key.replace(r'_', '').replace(r'\\', '').replace(r'{', '').replace(r'}', '').replace(r'#', '')
    desc = r.description.replace(r'_', '').replace(r'\\vv', '').replace(r' - ', '-')
    unit = r.units
    symb['sym-{}'.format(r.key)] = templ_quant.format(label=label, mathalias=mathalias, symbol=symbol, sortlabel=sortlabel,
                                                      desc=desc, unit=unit)

# %%

for _, r in df[df['short'].notnull()].iterrows():
    symb['acr-{}'.format(r.short)] = templ_acr.format(short='acr-{}'.format(r.short), shrt=r.short, long=r.long.replace('\\&', 'and'))

# %%

for _, r in df[(df.key.notna() & df.units.isna())].iterrows():
    label = 'gls-{}'.format(r.key)
    name = r['name']
    sortlabel = r.key.replace(r'_', '').replace(r'\\', '').replace(r'{', '').replace(r'}', '').replace(r'#', '')
    desc = r.description.replace(r'_', '').replace(r'\\vv', '').replace(r' - ', '-').replace(r'_', '').replace('{', '').replace('}', '')
    symb['gls-{}'.format(r.key)] = templ_gls.format(label=label, name=name, sortlabel=sortlabel, desc=desc)

# %%
symb_gls = ''.join([symb[k] for k in sorted(symb)]).replace('\r', '').replace('\n', '\r\n').replace('ü', 'u').replace('–', '-').expandtabs(tabsize=4)  # .encode(encoding="utf-8",errors="replace")

out = file_temp.format(symb_gls)
with open('{}glossary.tex'.format(path), 'wt') as f:
    f.write(out)
