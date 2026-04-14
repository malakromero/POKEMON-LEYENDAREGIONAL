import os

log_file = "/home/angel/POKEMON-LEYENDAREGIONAL/notas/diario/2026-04-13.md"

with open(log_file, "r", encoding="utf-8") as f:
    content = f.read()

new_log_entry = "- Reestructuración de la carpeta notas (Regla de Oro a raíz, separación de mecánicas en `mecanicas/`, guion centralizado en `guiones/`, todo en minúsculas, y actualización automática de los enlaces internos de Markdown)."

content = content.replace("## Proximos pasos", f"{new_log_entry}\n\n## Proximos pasos")

with open(log_file, "w", encoding="utf-8") as f:
    f.write(content)

print("Bitácora actualizada correctamente.")
