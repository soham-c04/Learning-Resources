import subprocess
import tempfile
import re
import sys
from pathlib import Path

SAT_SOLVER = r"C:\Users\SOHAM\Downloads\cryptominisat5.exe"

def varnum(r, c, d):
    return 9 * (r - 1) + 3 * (c - 1) + d

def inv_varnum(v):
    v -= 1
    r = v // 9 + 1
    c = (v % 9) // 3 + 1
    d = v % 3 + 1
    return r, c, d

def build_cnf(grid):
    clauses = []

    for r in range(1, 4):
        for c in range(1, 4):
            clause = [str(varnum(r, c, d)) for d in range(1, 4)]
            clauses.append(clause)

    for r in range(1, 4):
        for c in range(1, 4):
            for d1 in range(1, 4):
                for d2 in range(d1 + 1, 4):
                    clauses.append([f"-{varnum(r,c,d1)}", f"-{varnum(r,c,d2)}"])

    for r in range(1, 4):
        for d in range(1, 4):
            for c1 in range(1, 4):
                for c2 in range(c1 + 1, 4):
                    clauses.append([f"-{varnum(r,c1,d)}", f"-{varnum(r,c2,d)}"])

    for c in range(1, 4):
        for d in range(1, 4):
            for r1 in range(1, 4):
                for r2 in range(r1 + 1, 4):
                    clauses.append([f"-{varnum(r1,c,d)}", f"-{varnum(r2,c,d)}"])

    for r in range(1, 4):
        for c in range(1, 4):
            v = grid[r - 1][c - 1]
            if v != 0:
                clauses.append([str(varnum(r, c, v))])

    num_vars = 27
    num_clauses = len(clauses)
    
    dimacs_lines = [f"p cnf {num_vars} {num_clauses}"]
    for cl in clauses:
        dimacs_lines.append(" ".join(cl) + " 0")
    dimacs = "\n".join(dimacs_lines) + "\n"
    return dimacs

def call_solver_with_dimacs(dimacs_text, solver_path):
    proc = subprocess.run([solver_path, '-'],
                          input=dimacs_text,
                          capture_output=True,
                          text=True,
                          timeout=30)
    if proc.returncode == 0 or ("SATISFIABLE" in proc.stdout.upper() or "UNSAT" in proc.stdout.upper()):
        return proc.stdout, proc.stderr

    with tempfile.NamedTemporaryFile(mode="w", suffix=".cnf", delete=False) as tf:
        tf.write(dimacs_text)
        temp_path = tf.name

    proc = subprocess.run([solver_path, temp_path],
                          capture_output=True,
                          text=True,
                          timeout=30)
    return proc.stdout, proc.stderr

def parse_solution(solver_stdout):
    stdout = solver_stdout.strip()
    
    if re.search(r"\bUNSATISFIABLE\b", stdout, re.IGNORECASE) or re.search(r"\bUNSAT\b", stdout, re.IGNORECASE):
        return None 

    lits = []
    for line in stdout.splitlines():
        line = line.strip()
        if not line:
            continue
        if line.startswith("v") or re.match(r"^[-\d\s]+0\s*$", line):
            for token in re.findall(r"-?\d+", line):
                val = int(token)
                if val == 0:
                    continue
                lits.append(val)
        else:
            for token in re.findall(r"-?\d+", line):
                val = int(token)
                if val == 0:
                    continue
                lits.append(val)

    positive_vars = [v for v in lits if v > 0]
    pos_set = set(positive_vars)

    grid = [[0] * 3 for _ in range(3)]
    for v in sorted(pos_set):
        if v < 1 or v > 27:
            continue
        r, c, d = inv_varnum(v)
        grid[r - 1][c - 1] = d
    if any(grid[r][c] == 0 for r in range(3) for c in range(3)):
        return grid
    return grid

def read_grid_from_stdin():
    print("Enter the 3x3 Sudoku rows. Use 0 for blanks. Example line: 0 2 0")
    grid = []
    for i in range(3):
        while True:
            line = input(f"Row {i+1}: ").strip()
            
            if not line:
                print("Empty line — please enter three numbers (0..3) separated by spaces.")
                continue
            parts = re.split(r"\s+", line)
            if len(parts) != 3:
                print("Please enter exactly 3 numbers.")
                continue
            
            nums = [int(x) for x in parts]
            
            if any(n < 0 or n > 3 for n in nums):
                print("Numbers must be in range 0..3.")
                continue
            grid.append(nums)
            break
    return grid

def pretty_print_grid(grid):
    print("\nSolved grid:")
    for r in range(3):
        print(" ".join(str(grid[r][c]) for c in range(3)))

def main():
    grid = read_grid_from_stdin()
    print("\nInput puzzle:")
    for row in grid:
        print(" ".join(str(x) for x in row))

    dimacs = build_cnf(grid)

    stdout, stderr = call_solver_with_dimacs(dimacs, SAT_SOLVER)

    solution = parse_solution(stdout)
    if solution is None:
        print("\nUNSATISFIABLE (no solution).")
    else:
        pretty_print_grid(solution)

if __name__ == "__main__":
    main()
