import os

from inputs import input_sizes, word_sizes


def main():
    _ = os.system("make -C ../algorithms")
    _ = os.system('echo "1" | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo')
    _ = os.system("sudo service bluetooth stop")
    _ = os.system("sudo ip link set enxacde48001122 down")
    _ = os.system("sudo ip link set wlp5s0 down")

    os.makedirs("results", exist_ok=True)
    os.makedirs("outputs", exist_ok=True)

    for input_size in input_sizes:
        for word_size in word_sizes:
            inputfile = f"inputs/input{input_size}_{word_size}.txt"

            timefile = f"results/timeGPU{input_size}_{word_size}.txt"
            outfile = f"outputs/outputGPU{input_size}_{word_size}.txt"
            for _ in range(20):
                _ = os.system(
                    f"../algorithms/gpu_greedy_ssp < {inputfile} >> {outfile} 2>> {timefile}"
                )

            timefile = f"results/timeGPUpart{input_size}_{word_size}.txt"
            outfile = f"outputs/outputGPUpart{input_size}_{word_size}.txt"
            for _ in range(20):
                _ = os.system(
                    f"../algorithms/partial_gpu_ssp < {inputfile} >> {outfile} 2>> {timefile}"
                )

            timefile = f"results/timeSEQ{input_size}_{word_size}.txt"
            outfile = f"outputs/outputSEQ{input_size}_{word_size}.txt"
            for _ in range(20):
                _ = os.system(
                    f"../algorithms/greedy_ssp < {inputfile} >> {outfile} 2>> {timefile}"
                )

    _ = os.system('echo "0" | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo')
    _ = os.system("sudo service bluetooth start")
    _ = os.system("sudo ip link set enxacde48001122 up")
    _ = os.system("sudo ip link set wlp5s0 up")


if __name__ == "__main__":
    main()
