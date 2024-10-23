import json
import argparse
import ply.yacc as yacc


args = argparse.ArgumentParser()
args.add_argument("--grammar-path", default="./grammar.json", type=str, required=False)
params = args.parse_args()
grammar_path = params.grammar_path

with open(grammar_path, 'r') as f:
    grammar = json.load(f)

tokens = tuple(grammar["tokens"])

print(tokens)

productions = grammar["states"]
for state in productions.keys():
    for index, rule in enumerate(productions[state]):
        name_prod = f"p_{state}_{index}"
        func = (f"def {name_prod}(p):\n" + 
                f"    '{state} : {rule}'\n"+
                f"    pass")
        exec(func)

yacc.yacc()