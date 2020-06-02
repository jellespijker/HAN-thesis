# %%
import pandas as pd
from sympy import init_printing, symbols, latex, pi, Rational, log, cos, sqrt

init_printing()

# %% symbols

sym = pd.read_csv('../glossary/symbols-english.csv', delimiter='|')
sym.index = sym.key


def prep_key(s):
    return str(s).replace(r'{', '').replace('}', '').replace('\\', '').format(' ', '_')


def add_symbol(df, key, units, description, smb=None):
    if any(df.index.isin([prep_key(key)])):
        print("key: {} is allready in glossary as {}\n not added\n".format(prep_key(key),
                                                                           df[df.key.isin([prep_key(key)])]))
        return
    if smb is None:
        smb = latex(key)
    s = pd.Series({'key': prep_key(key),
                   'symbol': smb,
                   'units': '\\si{{{}}}'.format(units),
                   'description': description})
    sym = df.loc[prep_key(key)] = s


def store_glossary(df: pd.DataFrame):
    df = df.sort_index()
    df.to_csv('../glossary/symbols-english.csv', sep='|', index=False)


Q, D_s, D_p, p_s, p_atm, p_ss, p_v, p_i, p_ro, p_sm, p_rp, v_s, v_p = symbols(
    'Q d_ps d_pp p_s p_atm p_ss p_h p_i p_ro p_sm p_rp v_s v_p')
rho_w, g, S, rho_m, epsilon_s, epsilon_b, a, C = symbols('rho_w g D_s rho_m epsilon_s epsilon_b h_a C_vb')
labda, L, psi, Re, mu, c_t, F_r, F_rxd, theta, rho_s = symbols('lambda L_s psi_m Re mu c_t F_r F_rxd theta_p rho_ds')
H, p_p, p_man = symbols('H p_p p_man')

add_symbol(sym, Q, '\\meter\\cubed\\per\\second', 'volumetric flow')
add_symbol(sym, v_p, '\\meter\\per\\second', 'mixture velocity in discharge line')
add_symbol(sym, v_s, '\\meter\\per\\second', 'mixture velocity in suction line')
add_symbol(sym, D_s, '\\meter', 'volumetric flow')
add_symbol(sym, D_p, '\\meter', 'volumetric flow')
add_symbol(sym, p_s, '\\pascal', 'absolute pressure at the entrance of impeller')
add_symbol(sym, p_p, '\\pascal', 'absolute pressure at the outlet of the dredge pump')
add_symbol(sym, p_man, '\\pascal', 'manometric head of the dredge pump')
add_symbol(sym, p_atm, '\\pascal', 'atmospheric pressure')
add_symbol(sym, p_ss, '\\pascal', 'static pressure generated by surrounding water')
add_symbol(sym, p_v, '\\pascal', 'head, i.e. the velocity required to accelerate the mixture in the pipe')
add_symbol(sym, p_i, '\\pascal', 'pressure loss at the suction inlet')
add_symbol(sym, p_ro, '\\pascal', 'resistance offered by bends, valves, hoses and other obstructions')
add_symbol(sym, p_sm, '\\pascal', 'static head in the suction pipe')
add_symbol(sym, p_rp, '\\pascal', 'resistance of the straight line pipe')
add_symbol(sym, rho_w, '\\kilo\\gram\\per\\cubic\\meter', 'density of water')
add_symbol(sym, rho_m, '\\kilo\\gram\\per\\cubic\\meter', 'density of mixture')
add_symbol(sym, rho_s, '\\kilo\\gram\\per\\cubic\\meter', 'density of soil')
add_symbol(sym, g, '\\meter\\per\\squared\\second', 'standard gravity model')
add_symbol(sym, S, '\\meter', 'suction depth')
add_symbol(sym, epsilon_s, '-', 'resistance coefficient of the suction inlet')
add_symbol(sym, epsilon_b, '-', 'resistance coefficient of the bends, valves, hoses and other obstructions')
add_symbol(sym, a, '\\meter', 'height of water level above center line of a pump')
add_symbol(sym, C, '-', 'transportation concentration')
add_symbol(sym, labda, '-', 'friction factor of a straight pipe')
add_symbol(sym, L, '\\meter', 'length of suction line')
add_symbol(sym, psi, '-', 'Durand_Condolios mixture correction')
add_symbol(sym, Re, '-', 'Reynolds Number')
add_symbol(sym, mu, '\\pascal\\second', 'dynamic viscosity')
add_symbol(sym, c_t, '-', 'volumetric concentration')
add_symbol(sym, F_r, '-', 'Froude number for a pipeline')
add_symbol(sym, F_rxd, '-', 'Froude number of the grains')
add_symbol(sym, theta, '\\degree', 'inclination of the pipeline')
add_symbol(sym, H, '\\meter', 'vertical distance between center of the pump and the elevation of the pipeline')

store_glossary(sym)

# %% equations

eq = {}


def save_to_tex(eq):
    for k, v in eq.items():
        with open('../equations/{}.tex'.format(k), 'w') as f:
            f.write(v)


# %% conversion


def export_latex(expr, name):
    smbls = list(expr.free_symbols)
    for s in smbls:
        s.name = '<{}>'.format(s)
        
    lstr = latex(expr)
    for s in smbls:
        ss = prep_key(s)
        glss = r'\gls{{sym-{}}}'.format(ss)
        lstr = lstr.replace(latex(s), glss)
    hdr = '% !TeX root = ../../report.tex\n% !TeX spellcheck = en - US\n% !TeX encoding = UTF - 8\n\n'
    return '{}\\begin{{equation}}\\label{{eq:{}}}\n  \\gls{{sym-{}}} = {}\n\\end{{equation}}'.format(hdr,
                                                                                                     prep_key(name),
                                                                                                     prep_key(name),
                                                                                                     lstr).replace('<',
                                                                                                                   '').replace(
        '>', '')


# %% mixture velocity


def _v(Q, D):
    return Q / (pi / 4 * D ** 2)


eq['v_s'] = export_latex(_v(Q, D_s), v_s)
eq['v_p'] = export_latex(_v(Q, D_p), v_p)


# %% pressure


def _sum_p(*args):
    return sum(args)


def _p_ss(rho_w, g, S):
    return rho_w * g * S  # / 1000


def _p_v(rho_m, v_s):
    return Rational(1, 2) * rho_m * v_s ** 2  # / 1000


def _p_i_ro(eps, rho, v_s):
    return eps * Rational(1, 2) * rho * v_s ** 2  # / 1000


def _p_sm(rho_m, g, S, a, C):
    return rho_m * g * (S - a) * C  # / 1000


def _p_rp(labda, L, D_s, rho_w, v_s, psi):
    return labda * L / D_s * Rational(1, 2) * rho_w * v_s ** 2 * psi  # / 1000 


def _labda(Re):
    return 0.31 * (log(Re) - 1) ** -2


def _Re(v_s, D_s, mu):
    return v_s * D_s / mu


def _psi(c_t, F_r, F_rxd, theta):
    return 1 + 180 * c_t * F_r**-3 * F_rxd**1.5 * cos(theta)


def _c_t(rho_m, rho_w, rho_s):
    return (rho_m - rho_w) / (rho_s - rho_w)


def _F_r(v_s, g, D_s):
    return v_s / sqrt(g * D_s)


def _p_p(rho_m, v_s, v_p):
    return Rational(1, 2) * rho_m * v_s ** 2  # / 1000


def _p_man(p_p, p_s):
    return p_p - p_s


eq['p_s'] = export_latex(_sum_p(p_atm, p_ss, -p_v, -p_i, -p_ro, -p_sm, -p_rp), p_s)

eq['p_ss'] = export_latex(_p_ss(rho_w, g, S), p_ss)

eq['p_v'] = export_latex(_p_v(rho_m, v_s), p_v)

eq['p_i'] = export_latex(_p_i_ro(epsilon_s, rho_m, v_s), p_i)

eq['p_ro'] = export_latex(_p_i_ro(epsilon_b, rho_m, v_s), p_ro)

eq['p_sm'] = export_latex(_p_sm(rho_m, g, S, a, C), p_sm)

eq['p_rp'] = export_latex(_p_rp(labda, L, D_s, rho_w, v_s, psi), p_rp)

eq['labda'] = export_latex(_labda(Re), labda)

eq['Re'] = export_latex(_Re(v_s, D_s, mu), Re)

eq['psi'] = export_latex(_psi(c_t, F_r, F_rxd, theta), psi)

eq['c_t'] = export_latex(_c_t(rho_m, rho_w, rho_s), c_t)

eq['F_r'] = export_latex(_F_r(v_s, g, D_s), F_r)

eq['p_p'] = export_latex(_p_p(rho_m, v_s, v_p), p_p)

eq['p_man'] = export_latex(_p_man(p_p, p_s), p_man)

save_to_tex(eq)
