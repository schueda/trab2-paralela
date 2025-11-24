import numpy as np
import pandas as pd

from inputs import input_sizes, word_sizes

methods = ["SEQ", "GPU", "GPUpart"]

def read_timefile(path):
    with open(path, 'r') as f:
        times = [float(line.strip()) for line in f]
    return np.mean(times), np.std(times)

def main():
    columns = pd.MultiIndex.from_product(
        [word_sizes, methods],
        names=["word_size", "method"]
    )

    df = pd.DataFrame(index=input_sizes, columns=columns, dtype=str)

    for input_size in input_sizes:
        for word_size in word_sizes:
            for method in methods:
                filename = f"results/time{method}{input_size}_{word_size}.txt"
                avg, std = read_timefile(filename)
                df.loc[input_size, (word_size, method)] = f"{avg:.2f} ± {std:.2f}"

    latex_header = []
    latex_header.append("entrada")

    for w in word_sizes:
        latex_header.append(f"\\multicolumn{{3}}{{c|}}{{{w}}}")

    header_row1 = " & ".join(latex_header) + " \\\\"
    
    header_row2 = " & " + " & ".join(m for w in word_sizes for m in methods) + " \\\\"

    rows_str = "\n".join(
        f"\\textbf{{{inp}}} & " +
        " & ".join(df.loc[inp, (w, m)] for w in word_sizes for m in methods) +
        " \\\\"
        for inp in df.index
    )

    total_cols = 1 + 3 * len(word_sizes)
    column_format = "|l|" + "c|" * (total_cols - 1)

    latex_table = f"""
\\begin{{table*}}[htbp]
\\centering
\\caption{{Tempos de execução para diferentes tamanhos de entrada e palavra}}
\\label{{tab:tempos}}
\\resizebox{{\\textwidth}}{{!}}{{%
\\begin{{tabular}}{{{column_format}}}
\\toprule
{header_row1}
\\midrule
{header_row2}
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
