# Mecanicas core (v0.1)

## Vision jugable
Pokemon Leyenda Regional combina 3 regiones jugables (Kanto, Johto, Hoenn) con progresion parcialmente abierta, escalado global y doble ruta de Alto Mando.

## Bucle principal de juego
1. Elegir region inicial (Kanto, Johto o Hoenn).
2. Explorar region activa en orden libre de gimnasios.
3. Obtener medallas para desbloquear viaje a otras regiones.
4. Completar Alto Mando de Hoenn (8) y Alto Mando Kanto+Johto (16).
5. Desbloquear zona final y enfrentar a Red adulto.

## Reglas globales ya definidas
- Escalado global segun el Pokemon mas fuerte del equipo.
- Orden libre de gimnasios dentro de cada region.
- Cambio de region condicionado por medallas.
- 1 Campeon por Alto Mando.
- Al completar ambos Alto Mandos se desbloquea encuentro final con Red adulto.
- Sistema seguidor activable con SELECT desde menu Pokemon.

## Progresion regional (cerrada)
### Regla oficial de desbloqueo
- Tras obtener las 8 medallas de la region inicial, se abre la opcion de viajar a una de las otras 2 regiones.
- Tras obtener las 8 medallas de esa segunda region, se desbloquea automaticamente la ultima region.

### Flujo de regiones
- Fase 1: region inicial (eleccion libre al empezar partida).
- Fase 2: segunda region (eleccion entre las 2 restantes al completar 8 medallas).
- Fase 3: ultima region (desbloqueo al completar 8 medallas de la segunda region).

## Escalado v1 (propuesta tecnica)
Referencia base:
- Lmax = nivel del Pokemon mas fuerte del jugador.

Bandas sugeridas:
- Entrenadores de ruta: Lmax - 4 a Lmax - 1
- Gimnasio (trainers): Lmax - 2 a Lmax
- Lider de gimnasio (ace): Lmax + 1 a Lmax + 3
- Rival/jefe de historia: Lmax + 2 a Lmax + 4
- Elite 4: Lmax + 4 a Lmax + 6
- Campeon: Lmax + 6 a Lmax + 8
- Red adulto (encuentro final): Lmax + 8 a Lmax + 12

Reglas de seguridad:
- Piso minimo por etapa para evitar combates triviales.
- Techo maximo por etapa para evitar picos injustos.
- Recalculo al entrar al combate (no durante el combate).

## Lineamientos de lore por paso del tiempo
- Parte de los lideres clasicos aparecen en version adulta.
- Parte de los gimnasios los lideran hijos o subordinados.
- Debe haber justificacion narrativa por cada relevo.

## Proximo bloque de definicion
- Lista completa de gimnasios por region con estado: original adulto / hijo / subordinado.
- Formula exacta de escalado por tipo de combate.
- Recompensa canonica por derrotar a Red adulto.

## Enlaces
- [[Lore/Ambientacion]]
- [[ADR/ADR-003-mundo-parcialmente-abierto-con-gyms-libres-por-region-y-escalado-de-nivel]]
- [[ADR/ADR-004-alto-mando-kanto-johto-compartido-y-alto-mando-hoenn-independiente]]
- [[ADR/ADR-005-red-adulto-como-encuentro-final-al-completar-los-2-alto-mandos]]
- [[ADR/ADR-006-lideres-reemplazados-por-hijos-o-subordinados-y-versiones-adultas]]
- [[ADR/ADR-007-sistema-de-seguimiento-pokemon-con-select-en-menu-pokemon]]
