import numpy as np
import pandas as pd
import math

from uncertainties import ufloat

processes = [2, 4, 8, 12, math.inf]
from inputs import input_sizes, word_sizes

def main():
    data = {}

    for input_size in input_sizes:
        for word_size in word_sizes:
            row_key = f"{input_size}/{word_size}"
            row = []
            
            file = f"results/amdahl{input_size}_{word_size}.txt"
            with open(file, 'r') as f:
                times = [float(line.strip()) for line in f]
                parallel_time = ufloat(np.mean(times), np.std(times))
            
                for num_of_processes in processes:
                    amdahl = 1/((1 - parallel_time) + parallel_time/num_of_processes)
                    row.append(f"{amdahl.nominal_value:.2f} ± {amdahl.std_dev:.2f}")
                data[row_key] = row

    df = pd.DataFrame(data).T
    df.columns = [str(t) for t in processes]

    n_threads = len(processes)
    column_format = "|l|" + "c|" * n_threads

    rows_str = "\n".join(
        f"\\textbf{{{index}}} & " + " & ".join(df.loc[index]) + " \\\\"
        for index in df.index
    )

    latex_table = f"""
\\begin{{table*}}[htbp]
\\centering
\\caption{{Lei de amdahl para diferentes números de threads e diferentes entradas e tamanhos de palavra}}
\\label{{tab:amdahl}}
\\resizebox{{\\textwidth}}{{!}}{{%
\\begin{{tabular}}{{{column_format}}}
\\toprule
entrada/palavra & {' & '.join(df.columns)} \\\\
\\midrule
{rows_str}
\\bottomrule
\\end{{tabular}}%
}}
\\end{{table*}}
""".strip()
   
    print(latex_table)

if __name__ == "__main__":
    main()