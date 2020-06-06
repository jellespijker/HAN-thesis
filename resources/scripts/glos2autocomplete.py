#%%
import pandas as pd

sym = pd.read_csv('../glossary/symbols-english.csv', delimiter='|')
acr = pd.read_csv('../glossary/acronyms-english.csv', delimiter='|')
glos = pd.read_csv('../glossary/glossary-english.csv', delimiter='|')
sym.index = sym.key

#%%

def store_glossary(df: pd.DataFrame):
    df = df.sort_index()
    df.to_csv('../glossary/symbols-english.csv', sep='|', index=False)

template = '\tsnippet: [\n\t\t{{\n\t\t\tdisplayText: "\\\\sym-{}"\n\t\t\tsnippet: "\\\\\\\\gls{{sym-{}}}\\\\n"\n\t\t\tdescription: "$2"\n\t\t}}\n\t]\n'
#%%

snip = '".text.tex.latex": {\n\tsnippet: [\n'
for _, r in sym.iterrows():
    snip += '\t\t{{\n\t\t\tdisplayText: "\\\\sym_{}"\n\t\t\tsnippet: "\\\\\\\\gls{{sym-{}}}"\n\t\t\tdescription: "{}"\n\t\t}}\n'.format(r['key'].replace(
        '-', '_'), r['key'], r['description'].replace('\\', '\\\\'))
for _, r in glos.iterrows():
    snip += '\t\t{{\n\t\t\tdisplayText: "\\\\gls_{}"\n\t\t\tsnippet: "\\\\\\\\gls{{sym-{}}}"\n\t\t\tdescription: "{}"\n\t\t}}\n'.format(
        r['key'].replace('-', '_'), r['key'], r['description'].replace('\\', '\\\\').replace('"',"'"))
for _, r in acr.iterrows():
    snip += '\t\t{{\n\t\t\tdisplayText: "\\\\acr_{}"\n\t\t\tsnippet: "\\\\\\\\gls{{acr-{}}}"\n\t\t\tdescription: "{}"\n\t\t}}\n'.format(
        r['short'].replace('-','_'), r['short'], r['long'].replace('\\', '\\\\'))

snip += '\t]\n}\n'
print(snip)

