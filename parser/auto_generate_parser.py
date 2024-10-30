import json
import argparse
from numpy import NAN
import ply.yacc as yacc
import math
import pandas as pd

args = argparse.ArgumentParser()
args.add_argument("--grammar-path", default="./grammar.json", type=str, required=False)
params = args.parse_args()
grammar_path = params.grammar_path

with open(grammar_path, 'r') as f:
    grammar = json.load(f)

tokens = tuple(grammar["tokens"])

productions = grammar["states"]
states = list(productions.keys())
n_prod = 0

t_len = int(
    math.log10(
        sum([
            len(productions[state]) for state in productions.keys()
            ]))) + 1

format_mode = f"0{t_len}d"
rules = []

for state in productions.keys():
    for index, rule in enumerate(productions[state]):
        name_prod = f"p_n{n_prod:0{t_len}d}_{state}_{index}"
        n_prod += 1
        rules.append(f'{state} -> {rule}')
        func = (f"def {name_prod} (p):\n"  + 
                f"    '{state} : {rule}'\n"+
                f"    pass")
        exec(func)



parser = yacc.yacc("LALR")
action = parser.action
goto = parser.goto

if "precedence" in grammar:
    precedence = tuple([tuple(i) for i in grammar["precedence"]])

def join_action(index, action, rule_row):
    if index in action:
        row = action[index]
        for symbols in row.keys():
            if symbols == "$end":
                s = "$"
            else:
                s = grammar["Symbols"][tokens.index(symbols)]
            rule_row[s] = f"s{abs(row[symbols])}"
            if row[symbols] < 0:
                rule_row[s] = f"r{abs(row[symbols])}"

def join_goto(index, goto, rule_row):
    if index in goto:
        rule_row |= goto[index]

rule_action_table = []
rule_goto_table = []

for index in range(max(list(action.keys()) + list(action.keys()))):
    rule_row_goto = {"prod_name":index}
    rule_row_action = {"prod_name":index}
    join_action(index, action, rule_row_action)
    join_goto(index, goto, rule_row_goto)
    rule_action_table.append(rule_row_action)
    rule_goto_table.append(rule_row_goto)

table_action = pd.DataFrame(rule_action_table)
table_action.to_csv("./action.csv")
table_goto = pd.DataFrame(rule_goto_table).fillna(-1).astype(int)
table_goto.to_csv("./goto.csv")