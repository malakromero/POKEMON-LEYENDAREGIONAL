import os
import shutil
import re
import glob

base_dir = "/home/angel/POKEMON-LEYENDAREGIONAL/notas"

# Crear nuevas carpetas en minusculas
new_dirs = ["mecanicas", "guiones", "adr", "diario", "lore", "modulos"]
for nd in new_dirs:
    os.makedirs(os.path.join(base_dir, nd), exist_ok=True)

# Mover archivos especificos primero
moves = [
    ("Intro-Presentacion.md", "guiones/intro-presentacion.md"),
    ("Lore/Mecanicas-Core.md", "mecanicas/mecanicas-core.md"),
    ("Lore/Mecanicas-Adicionales.md", "mecanicas/mecanicas-adicionales.md"),
    ("Diario/Regla de Oro.md", "regla-de-oro.md"),
    ("Ideas.md", "ideas.md"),
    ("Indice.md", "indice.md"),
    ("Pendientes.md", "pendientes.md"),
    ("Plantilla.md", "plantilla.md"),
    ("README.md", "readme.md")
]

for src, dst in moves:
    src_path = os.path.join(base_dir, src)
    dst_path = os.path.join(base_dir, dst)
    if os.path.exists(src_path):
        os.rename(src_path, dst_path)

# Mover todo lo que quedo en las carpetas con mayuscula hacia sus equivalentes en minuscula
for cap_dir in ["ADR", "Diario", "Lore", "Modulos"]:
    src_dir = os.path.join(base_dir, cap_dir)
    dst_dir = os.path.join(base_dir, cap_dir.lower())
    if os.path.exists(src_dir):
        for root, dirs, files in os.walk(src_dir):
            for file in files:
                old_file_path = os.path.join(root, file)
                rel_path = os.path.relpath(old_file_path, src_dir)
                # lowercase the filename too
                new_rel_path = rel_path.lower()
                new_file_path = os.path.join(dst_dir, new_rel_path)
                
                os.makedirs(os.path.dirname(new_file_path), exist_ok=True)
                os.rename(old_file_path, new_file_path)
        # remover carpeta vieja si esta vacia
        shutil.rmtree(src_dir)

# Renombrar scripts/ a minuscula ? ya esta en minuscula, pero los archivos adentro?
if os.path.exists(os.path.join(base_dir, "scripts")):
    for f in os.listdir(os.path.join(base_dir, "scripts")):
        os.rename(os.path.join(base_dir, "scripts", f), os.path.join(base_dir, "scripts", f.lower()))

# Ahora actualizar los enlaces en todos los .md
link_replacements = {
    r"\[\[Intro-Presentacion\]\]": "[[guiones/intro-presentacion]]",
    r"\[\[Lore/Mecanicas-Core\]\]": "[[mecanicas/mecanicas-core]]",
    r"\[\[Lore/Mecanicas-Adicionales\]\]": "[[mecanicas/mecanicas-adicionales]]",
    r"\[\[Diario/Regla de Oro\]\]": "[[regla-de-oro]]",
    r"\[\[Regla de Oro\]\]": "[[regla-de-oro]]",
    r"\[\[Lore/": "[[lore/",
    r"\[\[ADR/": "[[adr/",
    r"\[\[Diario/": "[[diario/",
    r"\[\[Modulos/": "[[modulos/",
    r"\[\[Indice\]\]": "[[indice]]",
    r"\[\[README\]\]": "[[readme]]",
    r"\[\[Plantilla\]\]": "[[plantilla]]",
    r"\[\[Pendientes\]\]": "[[pendientes]]",
    r"\[\[Ideas\]\]": "[[ideas]]"
}

md_files = []
for root, dirs, files in os.walk(base_dir):
    for f in files:
        if f.endswith(".md"):
            md_files.append(os.path.join(root, f))

for md_file in md_files:
    with open(md_file, "r", encoding="utf-8") as f:
        content = f.read()
    
    nuevo_content = content
    # Aplicar los regex
    for viejo, nuevo in link_replacements.items():
        nuevo_content = re.sub(viejo, nuevo, nuevo_content)
    
    # Adicionalmente, necesitamos asegurar que cualquier cosa dentro de los corchetes [[ ... ]] este en minusculas para reflejar los archivos
    # Una busqueda con expresion regular para encontrar [[ ... ]] y dentro pasarlo a lower.
    def lower_match(match):
        inner_text = match.group(1)
        # Si es un enlace especifico que ya fue parseado y esta en la base, no arruinar la capitalizacion visual si quieren, pero el archivo fisico si es minuscula.
        # Obsidian usualmente es case insensitive pero en WSL/Linux es sensitive.
        return f"[[{inner_text.lower()}]]"
        
    nuevo_content = re.sub(r"\[\[(.*?)\]\]", lower_match, nuevo_content)

    if nuevo_content != content:
        with open(md_file, "w", encoding="utf-8") as f:
            f.write(nuevo_content)

print("TODO LISTO. ARCHIVOS RENOMBRADOS Y ENLACES ACTUALIZADOS.")
