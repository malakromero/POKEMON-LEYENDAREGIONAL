# Pokemon Leyenda Regional

Proyecto fan de ROM Hack centrado en una nueva experiencia regional conectada.

Pokemon Leyenda Regional propone una aventura conectada entre Kanto, Johto y Hoenn, ambientada varios años despues de los juegos clasicos, con progresion libre por gimnasios, escalado global de dificultad y contenido de post-game enfocado en retos.

## Estado del proyecto

- Fase actual: diseno + implementacion inicial
- Base tecnica: motor actual de Pokemon Leyenda Regional
- Compilacion: funcional
- ROM de salida: pokemon_leyenda_regional.gba

## Vision de juego

- Elegir region inicial: Kanto, Johto o Hoenn
- Orden libre de gimnasios dentro de cada region
- Escalado global de nivel segun el Pokemon mas fuerte del equipo
- Progresion entre regiones por medallas
- Doble ruta de Alto Mando
- Combate final post-game contra Red adulto

## Reglas principales

- Con 8 medallas se desbloquea el viaje a la siguiente region
- Con 16 medallas totales se desbloquea la tercera region
- Kanto + Johto comparten un Alto Mando
- Hoenn tiene su propio Alto Mando
- Al superar ambos Altos Mandos se habilita el encuentro final

## Mecanicas clave implementadas/disenadas

- Sistema de Pokemon seguidor (toggle con SELECT en menu Pokemon)
- Vuelo mejorado con visual del Pokemon volador
- Pokedex mejorado estilo Crystal Clear
- Misiones dinamicas de NPCs
- Bolsa expandida y compras en cantidad
- Sistema de logros y estadisticas
- Legendarios errantes por region
- Categoria de legendarios post-game por region

## Pantalla legal personalizada

Se agrego una pantalla negra legal despues del logo de Nintendo con mensaje de autoria y uso sin fines de lucro.

- Duracion total: ~6 segundos
- No saltable durante ~2 segundos
- Saltable luego con A/B/START

## Estructura util del repo

- src/: logica principal del juego
- data/: datos de mapas, scripts, texto, batallas
- graphics/: recursos graficos
- include/: headers
- notas/: documentacion del proyecto (ADR, lore, mecanicas)
- dev_scripts/: scripts auxiliares del proyecto

## Compilar

Requisitos minimos en Linux:

- make
- gcc/g++
- python3
- arm-none-eabi-gcc
- arm-none-eabi-binutils

Compilacion:

```bash
make -j"$(nproc)"
```

Salida esperada:

- pokemon_leyenda_regional.elf
- pokemon_leyenda_regional.gba

## Ejecutar para pruebas

En este entorno se usa extension de VS Code para emular GBA.

Flujo rapido:

```bash
./dev_scripts/build_and_open_rom.sh
```

Esto compila y abre pokemon_leyenda_regional.gba en VS Code para lanzarlo con la extension instalada.

## Documentacion de diseno

La documentacion viva del proyecto esta en:

- notas/Indice.md
- notas/Lore/Mecanicas-Core.md
- notas/Lore/Mecanicas-Adicionales.md
- notas/Lore/Legendarios-Errantes.md
- notas/ADR/

## Creditos

- Proyecto: MalakLegends
- Desarrollo y direccion creativa: equipo de Pokemon Leyenda Regional
- Agradecimientos: comunidad de ROM Hacking

## Aviso legal

Este es un proyecto de fans sin fines de lucro.
Pokemon y todo el material relacionado pertenecen a Nintendo, Game Freak y The Pokemon Company.
