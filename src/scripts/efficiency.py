import numpy as np
import pandas as pd

from uncertainties import ufloat

from tests import threads
from inputs import input_sizes, word_sizes

def main():
    data = {}

    for input_size in input_sizes:
        for word_size in word_sizes:
            row_key = f"{input_size}/{word_size}"
            row = []
            
            file = f"results/timeSEQ_{input_size}_{word_size}.txt"
            with open(file, 'r') as f:
                times = [float(line.strip()) for line in f]
                seq_ufloat = ufloat(np.mean(times), np.std(times))
            
                for thread in threads:
                    file = f"results/time{thread}_{input_size}_{word_size}.txt"
                    with open(file, 'r') as f:
                        times = [float(line.strip()) for line in f]
                        threads_ufloat = ufloat(np.mean(times), np.std(times))
                        efficiency = (seq_ufloat / threads_ufloat) / thread
                        row.append(f"{efficiency.nominal_value:.2f} ± {efficiency.std_dev:.2f}")
                data[row_key] = row

    df = pd.DataFrame(data).T
    df.columns = [str(t) for t in threads]

    n_threads = len(threads)
    column_format = "|l|" + "c|" * n_threads

    rows_str = "\n".join(
        f"\\textbf{{{index}}} & " + " & ".join(df.loc[index]) + " \\\\"
        for index in df.index
    )

    latex_table = f"""
\\begin{{table*}}[htbp]
\\centering
\\caption{{Eficiência para diferentes números de threads e diferentes entradas e tamanhos de palavra}}
\\label{{tab:eficiencia}}
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