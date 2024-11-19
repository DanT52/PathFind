import subprocess
import os

def compile_cpp():
    compile_command = ["g++", "-std=c++11", "-o", "main", "main.cpp"]
    result = subprocess.run(compile_command, capture_output=True, text=True)
    if result.returncode != 0:
        print("Compilation failed:")
        print(result.stderr)
        return False
    return True

def run_test(input_file, expected_output_file):
    with open(input_file, 'r') as infile, open(expected_output_file, 'r') as expected_file:
        expected_output = expected_file.read().strip()
        result = subprocess.run(["./main"], stdin=infile, capture_output=True, text=True)
        actual_output = result.stdout.strip()
        return actual_output == expected_output

def main():
    if not compile_cpp():
        return

    input_dir = "tests/inputs"
    output_dir = "tests/outputs"
    input_files = sorted([f for f in os.listdir(input_dir) if f.startswith("in")])
    output_files = sorted([f for f in os.listdir(output_dir) if f.startswith("out")])

    for input_file, output_file in zip(input_files, output_files):
        input_path = os.path.join(input_dir, input_file)
        output_path = os.path.join(output_dir, output_file)
        if run_test(input_path, output_path):
            print(f"Test {input_file} passed.")
        else:
            print(f"Test {input_file} failed.")

if __name__ == "__main__":
    main()