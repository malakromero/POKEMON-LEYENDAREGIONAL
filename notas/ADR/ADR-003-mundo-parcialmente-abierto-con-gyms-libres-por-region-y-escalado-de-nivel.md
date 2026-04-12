# ADR-003: Mundo parcialmente abierto con gyms libres por region y escalado de nivel

## Estado
Aceptado

## Fecha
2026-04-11

## Contexto
El juego tiene 3 regiones jugables con 8 gimnasios cada una. Se necesita definir si el jugador puede abordarlos en cualquier orden y como se maneja la progresion de niveles.

## Decision
Mundo **parcialmente abierto**:
- El jugador puede hacer los gimnasios de **cada region en el orden que prefiera**.
- La libertad de orden esta **limitada a la region activa**: no se mezclan gimnasios entre regiones.
- El escalado de nivel se adapta automaticamente a los gimnasios completados, no a un orden fijo.

## Consecuencias
- Positivas:
  - Mas libertad y rejugabilidad sin perder estructura narrativa por region.
  - El escalado de nivel evita que el juego sea trivial o imposible segun el orden elegido.
  - Cada region mantiene coherencia de dificultad propia.
- Negativas:
  - El escalado de nivel requiere logica adicional en el codigo de batalla.
  - Los eventos de historia atados a gymnasios deben funcionar en cualquier orden.
- Riesgos:
  - Posibles conflictos de flags de evento si se asume un orden fijo en el codigo base de Emerald.
  - El equilibrio de equipos de lideres de gimnasio se vuelve mas complejo.

## Alternativas consideradas
1. Orden fijo de gimnasios por region (clasico).
2. Mundo totalmente abierto sin restriccion entre regiones.

## Escalado de nivel — detalle tecnico
El escalado es **global**: aplica a todo el contenido del juego sin excepcion.
- Gimnasios y lideres
- Entrenadores del camino
- Pokemon salvajes
- Elite 4 y Campeon
- Jefes de historia y eventos especiales

La referencia siempre es el **nivel del Pokemon mas alto del equipo** del jugador.
Evita que el jugador pueda trivializar el juego grindando mucho o que quede bloqueado en cualquier parte.

## Desbloqueo de regiones
- Con **8 medallas** de la region actual, se habilita la opcion de viajar a otra region.
- Con **8 medallas** de la segunda region elegida, se desbloquea la ultima region.
- La progresion queda en 3 fases: region inicial (libre) -> segunda region (eleccion entre 2) -> ultima region (desbloqueada al completar 8 de la segunda).

## Pendientes tecnicos
- [ ] Definir la formula exacta de proporcion del escalado (ej: lider = nivel maximo del jugador + X)
- [ ] Definir techo y suelo de escalado por gimnasio para mantener coherencia narrativa
- [ ] Revisar flags de evento en el codigo base que asuman orden fijo

## Enlaces
- [[Indice]]
- [[ADR/ADR-002-seleccion-de-region-inicial-al-comenzar-el-juego]]

## Enlaces
- [[Indice]]
