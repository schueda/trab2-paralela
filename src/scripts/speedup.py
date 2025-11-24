import numpy as np
import pandas as pd
from uncertainties import ufloat

from inputs import input_sizes, word_sizes

methods = ["GPU", "GPUpart"]

def read_times(path):
    with open(path, 'r') as f:
        times = [float(line.strip()) for line in f]
    return ufloat(np.mean(times), np.std(times))

def main():
    columns = pd.MultiIndex.from_product(
        [word_sizes, methods],
        names=["word_size", "method"]
    )

    df = pd.DataFrame(index=input_sizes, columns=columns, dtype=str)

    for input_size in input_sizes:
        for word_size in word_sizes:
            seq_file = f"results/timeSEQ{input_size}_{word_size}.txt"
            seq_val = read_times(seq_file)
            for method in methods:
                file = f"results/time{method}{input_size}_{word_size}.txt"
                meth_val = read_times(file)
                speed = seq_val / meth_val
                df.loc[input_size, (word_size, method)] = f"{speed.nominal_value:.2f} ± {speed.std_dev:.2f}"

    header1 = ["entrada"] + [f"\\multicolumn{{2}}{{c|}}{{{w}}}" for w in word_sizes]
    header_row1 = " & ".join(header1) + " \\\\"

    header2 = "entrada & " + " & ".join(m for w in word_sizes for m in methods) + " \\\\"

    rows = "\n".join(
        f"\\textbf{{{inp}}} & " +
        " & ".join(df.loc[inp, (w, m)] for w in word_sizes for m in methods) +
        " \\\\"
        for inp in df.index
    )

    total_cols = 1 + 2 * len(word_sizes)
    column_format = "|l|" + "c|" * (total_cols - 1)

    latex = f"""
\\begin{{table*}}[htbp]
\\centering
\\caption{{Speedup para GPU e GPUpart para diferentes entradas e tamanhos de palavra}}
\\label{{tab:speedup_gpu}}
\\resizebox{{\\textwidth}}{{!}}{{%
\\begin{{tabular}}{{{column_format}}}
\\toprule
{header_row1}
\\midrule
{header2}
\\midrule
{rows}
\\bottomrule
\\end{{tabular}}%
}}
\\end{{table*}}
""".strip()

    print(latex)

if __name__ == "__main__":
    main()
