# ADR-004: Alto Mando Kanto-Johto compartido y Alto Mando Hoenn independiente

## Estado
Aceptado

## Fecha
2026-04-11

## Contexto
El juego tiene 3 regiones jugables. Kanto y Johto estan historicamente ligadas (como en los juegos originales). Hoenn es una region independiente con su propio final.

## Decision
| Alto Mando | Regiones involucradas | Medallas requeridas |
|---|---|---|
| Alto Mando Kanto + Johto | Kanto y Johto | 16 (8 de Kanto + 8 de Johto) |
| Alto Mando Hoenn | Hoenn | 8 |

- Kanto y Johto **comparten** un unico Alto Mando, accesible solo con las 16 medallas de ambas regiones.
- Hoenn tiene su propio Alto Mando, accesible con las 8 medallas de la region.
- El jugador puede enfrentar los dos Alto Mandos sin orden fijo entre ellos.

## Consecuencias
- Positivas:
  - Refuerza el vinculo narrativo entre Kanto y Johto, coherente con el lore original.
  - El Alto Mando compartido actua como gran clímax de ambas regiones.
  - Hoenn mantiene identidad propia con su propio final.
- Negativas:
  - El jugador que empieza en Kanto o Johto tarda mas en llegar al Alto Mando.
  - Mayor complejidad en los flags de desbloqueo (verificar medallas de 2 regiones).
- Riesgos:
  - Si el jugador empieza en Hoenn, puede derrotar ese Alto Mando antes que el de Kanto+Johto, lo que puede sentirse desequilibrado narrativamente.

## Alternativas consideradas
1. Tres Alto Mandos independientes (uno por region).
2. Un unico Alto Mando final con las 24 medallas totales.

## Pendientes
- [ ] Definir si existe un campeon unico final o uno por Alto Mando
- [ ] Definir la narrativa que justifica el Alto Mando compartido Kanto+Johto
- [ ] Definir si completar los 2 Alto Mandos desbloquea algo adicional

## Enlaces
- [[Indice]]
- [[ADR/ADR-003-mundo-parcialmente-abierto-con-gyms-libres-por-region-y-escalado-de-nivel]]

## Enlaces
- [[Indice]]
