import re
import sys

def should_skip_line(line):
    return len(line) == 0 or line[0] == '#'

def validate_str(stripped_line):
    pattern = r"^[.01]+$"
    return len(stripped_line) > 32 and stripped_line[32].isspace() and re.fullmatch(pattern, stripped_line[:32])

class Pattern:
    def __init__(self, mask, fb, name):
        self.mask = mask
        self.fb = fb
        self.name = name

class PatternGroup:
    def __init__(self, patterns):
        common_mask = 0xFFFFFFFF
        groups = {}
        self.groups = {}
        for pat in patterns:
            common_mask = common_mask & pat.mask
        self.common_mask = common_mask
        for pat in patterns:
            val = pat.fb & common_mask
            if val in groups:
                groups[val].append(pat)
            else:
                groups[val] = [pat]
        for (val, group_pats) in groups.items(): 
            if len(group_pats) > 1:
                self.groups[val] = PatternGroup(group_pats)
            else:
                self.groups[val] = group_pats[0]

def generate_decision_tree(out_file, group):
    out_file.write("switch (instruction & " + hex(group.common_mask) + ") {\n")
    for (val, subgroups) in group.groups.items():
        out_file.write("case " + hex(val) + ": {\n")
        if type(subgroups) is Pattern:
            out_file.write("return " + subgroups.name + "::process(instruction, state);\n")
        else:
            generate_decision_tree(out_file, subgroups)
        out_file.write("}\n")
    out.write("default: {\n")
    out.write("std::cerr << \"Instruction is not supported: \" << std::hex << instruction;\n")
    out.write("return false;\n}\n}\n")
global head_group 

with open("patterns.txt", "r") as file:
    patterns = []
    for line in file:
        stripped_line = line.lstrip()
        if should_skip_line(stripped_line):
            continue
        if not validate_str(stripped_line):
            print("validate: ", stripped_line)
            raise RuntimeError("line is not correct")
        fixed_bits = int("0b" + stripped_line[:32].replace('.', '0'), 2)
        mask = int("0b" + stripped_line[:32].replace('0', '1').replace('.', '0'), 2)
        patterns.append(Pattern(mask, fixed_bits, stripped_line[32:].replace(" ", "").rstrip()))

    head_group = PatternGroup(patterns)

with open(sys.argv[1], "w") as out:
    out.write("#include <core/state.h>\n")
    out.write("#include <ops/ops.h>\n\n")
    out.write("namespace cheeky::ops {\n")
    out.write("bool handle_instruction(uint32_t instruction, core::State& state) {\n")
    generate_decision_tree(out, head_group)
    out.write("}\n}\n")
