import json
import argparse

args = argparse.ArgumentParser()
args.add_argument("--grammar-path", default="./grammar.json", type=str, required=False)
params = args.parse_args()
grammar_path = params.grammar_path

with open(grammar_path, 'r') as f:
    grammar = json.load(f)

tokens = tuple(grammar["tokens"])
productions = grammar["states"]
states = list(productions.keys())
first = {}
follow = {states[0]:["$"]}

def first_calc():
    for state in states:
        if state in first:
            continue
        first_state(state)

def first_state(state, index=0):
    for rule in productions[state]:
        symbols = rule.split(" ")
        first_procced(symbols, state)
    return [i for i in first[state] if i != ""]

def first_procced(symbols, state, index=0):
    if symbols[index] not in tokens and symbols[index] != '': # quando for um estado
        if "" in productions[symbols[index]]: # se o estado tiver epsilon
            if len(symbols) == 1: # caso ele seja unico, adiciona o epsilon e paciencia 
                join_m(state, [""], first)
            else: # caso nao seja, vai pro proximo
                join_m(state, first_procced(symbols, state, index + 1), first)

        if symbols[index] == state: # caso o estado seja o proprio, n eh necessario fazer o calculo, pois A.join(A) == A.
            return

        elif symbols[index] not in first: # caso o estado nao esteja calculado, calcula e atribui: B.join(first(A))
            join_m(state, first_state(symbols[index]), first)
        
        elif symbols[index] != state: # caso o estado ja tenha calculado o first, B.join(A)
            join_m(state, first[symbols[index]], first)

    else: # quando for terminal, trivial
        join_m(state, [symbols[index]], first)

    return [i for i in first[state] if i != ""]

visited = set()

def join_m(state, value, dict_m):
    if state not in dict_m:
        dict_m[state] = value
    else:
        dict_m[state] += [out for out in value if out not in dict_m[state]]

def find_all(lst, value):
    return [index for index, element in enumerate(lst) if element == value]

def follow_calc():
    visited.clear() 
    for state in states:
        follow_state(state)

def follow_state(state):
    if state in visited:
        return follow.get(state, [])
    visited.add(state) 

    for s in states:
        for rule in productions[s]:
            symbols = rule.split(" ")
            if state in symbols:
                for i in find_all(symbols, state):
                    follow_proccede(state, s, symbols, i)
    return follow.get(state, [])

def follow_proccede(state, s, symbols, i):
    if i == len(symbols) - 1:
        if s != state:
            join_m(state, follow_state(s), follow)
    else:
        if symbols[i + 1] in tokens:  # Caso trivial
            join_m(state, [symbols[i + 1]], follow)
        else:
            if "" in productions[symbols[i + 1]]:
                join_m(state, follow_state(symbols[i + 1]), follow)
            else:
                join_m(state, first[symbols[i + 1]], follow)

def first_p():
    rules = {}
    for s in states:
        for rule in productions[s]:
            symbols = rule.split(" ")
            if s in rules:
                rules[s].append(firstp_proccede(s, symbols))
            else:
                rules[s] = [firstp_proccede(s, symbols)]
    rules[states[0]][0] += ["$"]
    return rules

def firstp_proccede(s, symbols, i=0):
    rules = []
    if symbols[i] in tokens or symbols[i] == "": #se for terminal
        if symbols[i] != "":
            rules = [symbols[i]]
        else:
            rules = follow[s]
    else:
        rules = [i for i in first[symbols[i]] if i != ""]
        if "" in productions[symbols[i]]:
            if i + 1 < len(symbols):
                rules += firstp_proccede(s, symbols, i+1)
            else:
                rules += follow[s]
    return rules

first_calc()
follow_calc()
print(str(first_p()).replace('\'', '\"').replace("[", "{").replace("]", "}"))