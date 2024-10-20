import os
import rstr
import argparse

def do_test(is_arg_required=True, **kwargs):
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

    id_relop = {
        "<=":0, "<":1,
        "==":2, "!=":3,
        ">":4, ">=":5,
        "!":6, "&&":8,
        "||":9
        }
    id_alg = {
        "+":(1, 0), "-":(2, 0),
        "*":(3, 0), "/":(4, 0),
        "=":(14, 0)
        }
    id_brackets = {
        "(":(6, 0), ")":(7, 0),
        "[":(8, 0), "]":(9, 0),
        "{":(10, 0), "}":(11, 0)
        }
    id_number = (5, 0)
    id_id = (0, 0)

    letter_def = r'[A-Za-z]'
    digit_def = r'[0-9]'
    id_def = fr'{letter_def}(({digit_def}|{letter_def}|_)*)'

    integer_constant_def = fr'{digit_def}+'
    operators_relop_def = r"==|!=|<=|<|>=|>|&&|\|\||!"
    operator_alg_def = r"=|\+|-|\*|/"
    brackets_def = r"\(|\)|{|}|\[|\]|,|;"
    comments_def = r"(//[^\n]*\n)|/\*.*\*/"

    # generate lists like (data, id), data is the tokens and id is the way of compiler identifies.
    integer_constant = [(rstr.xeger(integer_constant_def), id_number) for _ in range(params.num_itkns)]
    id = [(rstr.xeger(id_def), id_id) for _ in range(params.num_itkns)]
    bl = ['\n' for _ in range(30)] if params.rand_bl else []
    comments = [rstr.xeger(comments_def) for _ in range(params.num_itkns)]
    operators_relop = [rstr.xeger(operators_relop_def) for _ in range(params.num_itkns)]
    operator_alg = [rstr.xeger(operator_alg_def) for _ in range(params.num_itkns)]
    brackets = [rstr.xeger(brackets_def) for _ in range(params.num_itkns)]
    
    id_list = [(c, id_id) for c in id]
    number_list = [(c, id_number) for c in integer_constant]
    opalg_list = [(c, id_alg[c]) for c in operator_alg]
    oprelop_list = [(c, (15, id_relop[c])) for c in operators_relop]
    brackets_list = [(c, id_brackets[c]) for c in brackets]
    ignorable_list = [(c, -1) for c in bl + comments]
    print(brackets)

if __name__ == "__main__":
    do_test(True)