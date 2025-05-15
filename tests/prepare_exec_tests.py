import subprocess
from pathlib import Path

executable = Path("data/branching")
source = Path("data/branching.cpp")

print(f"Compiling test executables...")

# Ensure the output directory exists
executable.parent.mkdir(parents=True, exist_ok=True)

# Compile the source file
result = subprocess.run(
    ["g++", "-o", str(executable), str(source)],
    capture_output=True,
    text=True
)

if result.returncode == 0:
    print("Compilation successful.")
else:
    print("Compilation failed.")
    print("Error output:")
    print(result.stderr)
    exit(1)