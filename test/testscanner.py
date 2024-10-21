import os
import rstr
import argparse
import random
import subprocess
import tqdm


def do_test(is_arg_required=True, **kwargs):
    params = define_header(is_arg_required, kwargs)

    id_relop, id_alg, id_brackets, id_number, id_id = define_id()

    (id_def, integer_constant_def, 
     operators_relop_def, operator_alg_def, 
     brackets_def, comments_def) = define_regex()

    # generate lists like (data, id), data is the tokens and id is the way of compiler identifies.
    (id_list, number_list, 
     opalg_list, oprelop_list, brackets_list, 
     ignorable_list) = define_lists(params, id_relop, 
                                    id_alg, id_brackets, 
                                    id_number, id_id, 
                                    id_def, integer_constant_def, 
                                    operators_relop_def, operator_alg_def, 
                                    brackets_def, comments_def)
    
    values = id_list + number_list + opalg_list + oprelop_list + brackets_list + ignorable_list
    random.shuffle(values)

    strs_values, ids = zip(*values)
    ids = list(ids)
    std_input = " ".join(strs_values)
    file = open("test.txt", "w+")
    file.write(std_input)
    file.flush()
    file.seek(0)
    
    result = subprocess.run(
        ['./testScanner.out'],
        stdin=file,
        stdout=subprocess.PIPE)
    file.close()
    
    out = result.stdout.decode().splitlines()
    ignored = 0
    miss = 0
    ignored_input = open("ignored_list.txt","+a")
    wrong_answer = open("wrong_list.txt","+a")
    print("AVISO: UMA ENTRADA NATURALMENTE IGNORADA PELO TESTE, DEVE SER IGNORADA PELO COMPILADOR, CASO NAO SEJA, UM ERRO DE INDEX OCORRERA")
    gui = tqdm.tqdm(enumerate(out))
    for i, output in enumerate(out[:-1]):
        id1, id2 = output.split(" ")
        
        while ids[i + ignored] == '-1':
            ignored_input.write(strs_values[i+ignored])
            ignored += 1
            gui.set_description(f"ignore {ignored} miss {miss}")
            
        index = i + ignored
        
        if id1 != ids[index][0] or id2 != ids[index][1]:
            wrong_answer.writelines(
            ["============\n",
            f"compiler answer: {(id1, id2)}\n",
            f"correct: {ids[index]}\n",
            f"input: {strs_values[index]}\n",
            f"strs ignored: {ignored}\n",
            f"index: {index}\n",
            "============\n"])
            miss += 1
    gui.close()

    if out[-1].split(" ")[0] != '18':
        miss += 1
    
    print(f"Numero de entradas ignoradas pelo teste: {ignored}")
    print(f"Numero de entradas erradas pelo compilador: {miss}")

    wrong_answer.close()
    ignored_input.close()
    

def define_lists(params, id_relop, id_alg, id_brackets, id_number, id_id, id_def, integer_constant_def, operators_relop_def, operator_alg_def, brackets_def, comments_def):
    integer_constant = [rstr.xeger(integer_constant_def) for _ in range(params.num_itkns)]
    id = [rstr.xeger(id_def) for _ in range(params.num_itkns)]
    bl = ['\n' for _ in range(30)] if params.rand_bl else []
    comments = [rstr.xeger(comments_def) for _ in range(params.num_itkns)]
    operators_relop = [rstr.xeger(operators_relop_def) for _ in range(params.num_itkns)]
    operator_alg = [rstr.xeger(operator_alg_def) for _ in range(params.num_itkns)]
    brackets = [rstr.xeger(brackets_def) for _ in range(params.num_itkns)]
    
    id_list = [(c, id_id) for c in id]
    number_list = [(c, id_number) for c in integer_constant]
    opalg_list = [(c, id_alg[c]) for c in operator_alg]
    oprelop_list = [(c, ('15', id_relop[c])) for c in operators_relop]
    brackets_list = [(c, id_brackets[c]) for c in brackets]
    ignorable_list = [(c, '-1') for c in bl + comments]
    return (id_list, number_list,
            opalg_list, oprelop_list,
            brackets_list, ignorable_list)

def define_regex():
    letter_def = r'[A-Za-z]'
    digit_def = r'[0-9]'
    id_def = fr'{letter_def}(({digit_def}|{letter_def}|_)*)'
    integer_constant_def = fr'{digit_def}+'
    operators_relop_def = r"==|!=|<=|<|>=|>|&&|\|\||!"
    operator_alg_def = r"=|\+|-|\*|/"
    brackets_def = r"\(|\)|{|}|\[|\]|,|;"
    anything = fr"({letter_def} | {digit_def})*"
    comments_def = fr"(//{anything}\n)|/\*{anything}\*/"

    return (id_def, integer_constant_def,
            operators_relop_def, operator_alg_def,
            brackets_def, comments_def)

def define_id():
    id_relop = {
        "<=":'0', "<":'1',
        "==":'2', "!=":'3',
        ">":'4', ">=":'5',
        "!":'6', "&&":'7',
        "||":'8'
        }
    id_alg = {
        "+":('1', '-1'), "-":('2', '-1'),
        "*":('3', '-1'), "/":('4', '-1'),
        "=":('14', '-1')
        }
    id_brackets = {
        "(":('6', '-1'), ")":('7', '-1'),
        "[":('8', '-1'), "]":('9', '-1'),
        "{":('10', '-1'), "}":('11', '-1'),
        ",":('12', '-1'), ";":('13', '-1')
        }
    id_number = ('5', '-1')
    id_id = ('0', '-1')
    return id_relop, id_alg, id_brackets, id_number, id_id

def define_header(is_arg_required, kwargs):
    if is_arg_required:
        args = argparse.ArgumentParser()
        args.add_argument("--num-itkns", default=1000, type=int, required=False)
        args.add_argument("--rand-bl", default=True, action="store_true", required=False)
        params = args.parse_args()
    else:
        if not "num_itkns" in kwargs:
            kwargs["nuum_itkns"] = 1000
        if not "rand_bl" in kwargs:
            kwargs["rand_bl"] = True
        params = argparse.Namespace(**kwargs)
    return params

if __name__ == "__main__":
    do_test(True)