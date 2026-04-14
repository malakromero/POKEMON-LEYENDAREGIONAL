# Vault de notas — Pokemon Leyenda Regional

Este espacio funciona como un vault estilo Obsidian para llevar registro del proyecto **Pokemon Leyenda Regional**.

- Creador: Angel Romero (MalakLegens / Malak)
- Repositorio: malakromero/Pokemon-Leyenda-Regional

## Estructura
- `Indice.md`: mapa principal de notas
- `Plantilla.md`: base para nuevas notas
- `Diario/`: registro diario
- `Modulos/`: notas por area tecnica
- `ADR/`: decisiones tecnicas de arquitectura
- `scripts/`: utilidades del vault

## Uso rapido
1. Crea una nota nueva con `Plantilla.md`.
2. Enlaza notas usando formato wiki: `[[NombreDeNota]]`.
3. Actualiza `Indice.md` cuando agregues temas nuevos.
4. Crea el registro diario con `./scripts/crear_diario.sh`.
5. Crea ADRs con `./scripts/crear_adr.sh "Titulo ADR"`.
