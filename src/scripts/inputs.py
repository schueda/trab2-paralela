from os import makedirs
import random

valid_chars = ['A','C','G','T']
input_sizes = [250, 500, 750, 1000]
word_sizes = [10, 128, 256]

def main():
    makedirs('inputs', exist_ok=True)
    
    for size in input_sizes:
        for word_size in word_sizes:
            with open(f'inputs/input{size}_{word_size}.txt', 'w') as f:
                print(f"{size}", file=f)
                for _ in range(size): 
                    print("".join(random.choices(valid_chars, k=word_size)), file=f)

if __name__ == "__main__":
    main()
