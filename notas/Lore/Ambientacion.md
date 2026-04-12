# Ambientacion general

## Regiones jugables
| Region | Juegos originales | Estado en PLR |
|--------|------------------|---------------|
| Kanto  | Rojo/Azul/Amarillo / FR/LG | Jugable |
| Johto  | Oro/Plata/Cristal / HG/SS  | Jugable |
| Hoenn  | Rubi/Zafiro/Esmeralda / OR/AS | Jugable |

## Linea de tiempo
- Los eventos de las 3 regiones ya ocurrieron.
- El mundo ya conoce a los Campeones de cada region.
- Los legendarios de cada region forman parte de la historia y el contexto del juego.

## Implicaciones de diseño
- Se necesitan mapas, layouts y eventos para las 3 regiones.
- Los NPCs, musica y graficos deben respetar el estilo de cada region.
- El orden de acceso a las regiones debe definirse narrativamente.

## Inicio del juego
- El jugador elige libremente en cual de las 3 regiones comenzar.
- La eleccion afecta el Pokemon inicial disponible en esa region y el contexto narrativo de arranque.

## Desbloqueo entre regiones
- Con 8 medallas de la region actual, se abre la opcion de viajar a otra region.
- Con 8 medallas de la segunda region, se desbloquea la ultima region.

## Alto Mando
| Alto Mando | Regiones | Medallas requeridas |
|---|---|---|
| Kanto + Johto | Kanto y Johto | 16 (8+8) |
| Hoenn | Hoenn | 8 |

## Campeones y final
- Hay 1 Campeon por Alto Mando.
- Al completar ambos Alto Mandos se desbloquea una zona final especial.
- El encuentro final del juego es contra Red en version adulta.

## Lideres de gimnasio
- Algunos lideres clasicos regresan en versiones mas adultas.
- Algunos gimnasios estaran a cargo de hijos o subordinados de lideres clasicos.
- Esta combinacion busca equilibrio entre continuidad, nostalgia y renovacion.

## Sistema de seguimiento Pokemon
- El Pokemon seguidor se activa desde el menu de Pokemon.
- La accion se ejecuta con el boton SELECT sobre el Pokemon elegido.
- SELECT permite activar, cambiar o desactivar el seguidor.
- Excepciones de uso y comportamiento tecnico definidos en [[ADR/ADR-007-sistema-de-seguimiento-pokemon-con-select-en-menu-pokemon]].

## Pendientes de definir
- [ ] Pokemon iniciales disponibles por region
- [ ] Tutorial o prologo comun vs. prologo por region
- [ ] Nexo narrativo que justifica el viaje entre regiones al obtener medallas
- [ ] Estado actual de cada Liga Pokemon
- [ ] Balanceo de dificultad entre regiones (ver [[ADR/ADR-003-mundo-parcialmente-abierto-con-gyms-libres-por-region-y-escalado-de-nivel]])
- [ ] Lista final de gimnasios con lider original adulto vs. reemplazo
- [ ] Lista de mapas concretos con seguidor deshabilitado

## Enlaces
- [[Indice]]
- [[Lore/Mecanicas-Core]]
- [[Lore/Mecanicas-Adicionales]]
