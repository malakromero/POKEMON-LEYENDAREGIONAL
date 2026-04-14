import os
import re

notas_dir = "/home/angel/POKEMON-LEYENDAREGIONAL/notas"

def format_markdown_tables(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()

    out_lines = []
    in_table = False
    table_lines = []

    def process_table(t_lines):
        # Extract rows
        rows = []
        for line in t_lines:
            # removing trailing newlines and surrounding spaces
            parts = [p.strip() for p in line.strip("\n").strip().split("|")]
            # remove empty first and last if they exist (caused by | at start and end)
            if len(parts) > 0 and parts[0] == "":
                parts.pop(0)
            if len(parts) > 0 and parts[-1] == "":
                parts.pop(-1)
            rows.append(parts)

        # find max widths
        num_cols = max((len(r) for r in rows), default=0)
        widths = [0] * num_cols
        for row in rows:
            for i, col in enumerate(row):
                # if separator row, ignore width
                if set(col) == {'-'} or set(col) == {'-', ':'} or set(col) == {':'}:
                    continue
                # For length calculation we must account for emojis like ✅ visually but len() is ok for rough padding
                # Actually len() treats ✅ as 1 char, which is fine for ASCII alignment padding.
                widths[i] = max(widths[i], len(col))

        # Format rows
        formatted = []
        for r_idx, row in enumerate(rows):
            new_row = []
            for i, col in enumerate(row):
                if r_idx == 1 and all(c in "-:" for c in col):
                    # Separator row
                    # Preserve alignment if there are colons
                    left_align = col.startswith(":")
                    right_align = col.endswith(":")
                    w = max(3, widths[i])
                    if left_align and right_align:
                        new_row.append(":" + "-" * (w) + ":")
                    elif right_align:
                        new_row.append("-" * (w + 1) + ":")
                    elif left_align:
                        new_row.append(":" + "-" * (w + 1))
                    else:
                        new_row.append("-" * (w + 2))
                else:
                    # Content row
                    w = widths[i]
                    # To cleanly align we just ljust
                    # Wait logic to rjust if right aligned? Just ljust for simplicity
                    new_row.append(" " + col.ljust(w) + " ")
            formatted.append("|" + "|".join(new_row) + "|\n")
        return formatted

    changed = False
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.strip().startswith("|") and line.strip().endswith("|"):
            if not in_table:
                in_table = True
                table_lines = []
            table_lines.append(line)
        else:
            if in_table:
                # Flush table
                formatted_table = process_table(table_lines)
                if formatted_table != table_lines:
                    changed = True
                out_lines.extend(formatted_table)
                in_table = False
            out_lines.append(line)
        i += 1
    
    if in_table:
        formatted_table = process_table(table_lines)
        if formatted_table != table_lines:
            changed = True
        out_lines.extend(formatted_table)

    if changed:
        with open(filepath, "w", encoding="utf-8") as f:
            f.writelines(out_lines)
        return True
    return False

# Procesar todos
for root, dirs, files in os.walk(notas_dir):
    for filename in files:
        if filename.endswith(".md"):
            format_markdown_tables(os.path.join(root, filename))
