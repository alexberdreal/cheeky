import re

def should_skip_line(line):
    return len(line) == 0 or line[0] == '#'

def validate_str(stripped_line):
    pattern = r"^[.01]+$"
    return len(stripped_line) > 32 and stripped_line[32].isspace() and re.fullmatch(pattern, stripped_line[:32])

class Pattern:
    def __init__(self, mask, fb):
        self.mask = mask
        self.fb = fb

class PatternGroup:
    def __init__(self, patterns):
        common_mask = 0xFFFFFFFF
        groups = {}
        self.groups = {}
        for pat in patterns:
            common_mask = common_mask & pat.mask
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

with open("patterns.txt", "r") as file:
    patterns = []
    for line in file:
        stripped_line = line.lstrip()
        if should_skip_line(stripped_line):
            continue
        if not validate_str(stripped_line):
            raise RuntimeError("line is not correct: " + stripped_line)
        fixed_bits = int("0b" + stripped_line[:32].replace('.', '0'), 2)
        mask = int("0b" + stripped_line[:32].replace('0', '1').replace('.', '0'), 2)
        patterns.append(Pattern(mask, fixed_bits))

    group = PatternGroup(patterns) 
