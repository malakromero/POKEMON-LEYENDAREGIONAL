# Cómo convertir el PNG del minimapa de Kanto a C array para GBA

1. Asegúrate de tener instalado `grit` (https://www.coranac.com/projects/grit/)

2. Ejecuta este comando desde la raíz del proyecto:

   grit graphics/pokenav/region_map/map_kanto.png -ftc -gB8 -gTFF00FF -p -pS -o graphics/kanto_minimap/kanto_minimap_gfx

   - Esto generará `kanto_minimap_gfx.h` y `kanto_minimap_gfx.c` en `graphics/kanto_minimap/`.
   - El array de gráficos será `gKantoMinimapGfx` y la paleta `gKantoMinimapPal`.

3. Copia el contenido de `kanto_minimap_gfx.c` dentro de `kanto_minimap_gfx.h` (o incluye el .c en tu build).

4. Compila el proyecto normalmente.

5. El minimapa aparecerá en el menú de selección de región.

Si necesitas ajustar el tamaño, edita el PNG antes de convertirlo.
