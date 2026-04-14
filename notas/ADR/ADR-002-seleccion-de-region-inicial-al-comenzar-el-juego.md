# ADR-002: Seleccion de region inicial al comenzar el juego

## Estado
Aceptado

## Fecha
2026-04-11

## Contexto
El juego cuenta con 3 regiones jugables: Kanto, Johto y Hoenn. Se necesita definir como el jugador accede a ellas y si hay un orden fijo o libre.

## Decision
El jugador puede **elegir libremente** en cual de las 3 regiones iniciar su aventura desde el principio del juego.

## Consecuencias
- Positivas:
  - Mayor rejugabilidad, cada run puede ser diferente.
  - El jugador se involucra con la region que le es mas familiar o preferida.
  - Refuerza el concepto de "Leyenda Regional" en el titulo.
- Negativas:
  - El tutorial / introduccion debe adaptarse a las 3 regiones o ser generico.
  - La narrativa no puede depender de un orden fijo entre regiones.
- Riesgos:
  - Desbalance de dificultad si las regiones tienen niveles distintos.
  - Mayor complejidad en la logica de guardado de progreso por region.

## Alternativas consideradas
1. Comenzar siempre en una region fija y desbloquear las demas.
2. Elegir region despues de un prologo comun a las 3.

## Enlaces
- [[Indice]]
