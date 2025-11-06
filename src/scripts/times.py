import numpy as np
import pandas as pd

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
                std = np.std(times)
                average = np.mean(times)
                row.append(f"{average:.2f} ± {std:.2f}")
            
            for thread in threads:
                file = f"results/time{thread}_{input_size}_{word_size}.txt"
                with open(file, 'r') as f:
                    times = [float(line.strip()) for line in f]
                    std = np.std(times)
                    average = np.mean(times)
                    row.append(f"{average:.2f} ± {std:.2f}")
            data[row_key] = row

    df = pd.DataFrame(data).T
    thread_list = [str(t) for t in threads]
    thread_list.insert(0, "Sequencial")
    df.columns = thread_list

    n_threads = len(thread_list)
    column_format = "|l|" + "c|" * n_threads

    rows_str = "\n".join(
        f"\\textbf{{{index}}} & " + " & ".join(df.loc[index]) + " \\\\"
        for index in df.index
    )

    latex_table = f"""
\\begin{{table*}}[htbp]
\\centering
\\caption{{Tempos de execução para diferentes entradas e tamanhos de palavra}}
\\label{{tab:tempos}}
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