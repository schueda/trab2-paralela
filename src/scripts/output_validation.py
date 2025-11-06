from glob import glob
from os import makedirs

def main():
    makedirs("validations", exist_ok=True)
    
    for outfile in glob("outputs/*.txt"):
        valfile = outfile.replace("outputs/output", "validations/validation")
        with open(outfile, 'r') as outf, \
        open(valfile, 'w') as valf:
            input_size = outfile.split("_")[1]
            word_size = outfile.split("_")[2].split(".")[0]
            infile = f"inputs/input{input_size}_{word_size}.txt"
            with open(infile, 'r') as inf:
                infile = inf.read()
                for outline in outf:
                    outline = outline.strip()
                    line_size = len(outline)
                            
                    ok = True         
                    for inline in inf:
                        inline = inline.strip()
                        if inline not in outline:
                            ok = False
                            break
                            
                    print(line_size, ok, file=valf)

if __name__ == "__main__":
    main()