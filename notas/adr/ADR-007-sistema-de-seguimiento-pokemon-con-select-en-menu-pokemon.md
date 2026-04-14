# ADR-007: Sistema de seguimiento Pokemon con SELECT en menu Pokemon

## Estado
Aceptado

## Fecha
2026-04-11

## Contexto
El juego necesita una forma rapida y clara de activar o desactivar el Pokemon seguidor sin agregar friccion en exploracion ni depender de NPCs.

## Decision
- El sistema de seguimiento Pokemon se controla desde el menu de Pokemon.
- Al presionar SELECT sobre un Pokemon del equipo, se activa como seguidor.
- Si ya hay un seguidor activo y se presiona SELECT sobre otro Pokemon, se reemplaza el seguidor.
- Si se presiona SELECT sobre el Pokemon que ya esta siguiendo, el seguimiento se desactiva.

## Tabla tecnica de excepciones
| Contexto | Permitir seguidor | Comportamiento |
|---|---|---|
| Exploracion normal (exterior/interior amplio) | Si | El seguidor aparece detras del jugador. |
| Bicicleta activa | No | Se oculta temporalmente; reaparece al bajar de la bici. |
| Surf/Buceo/Cascada activos | No | Se oculta temporalmente; reaparece al volver a tierra. |
| Escaleras/teletransporte/cambio de mapa | Si | Se re-sincroniza posicion al cargar el nuevo mapa. |
| Mapas pequenos o con pasillos de 1 tile | No | Se desactiva solo en ese mapa para evitar bloqueos. |
| Escenas de historia (cutscenes) | No | Se oculta mientras dure el evento y luego reaparece. |
| Batalla (entrenador/salvaje) | No en campo | El seguidor no se renderiza en mapa durante la batalla. |
| Centros/tiendas | Si | Permitido salvo mapas marcados como restringidos. |

## UX y mensajes
- Activar: "<Pokemon> ahora te sigue." 
- Cambiar: "<Pokemon> tomo el lugar del seguidor actual." 
- Desactivar: "<Pokemon> dejo de seguirte." 
- Bloqueado por contexto: "Aqui no puedes usar un Pokemon seguidor." 

## Reglas base de implementacion
- Prioridad: estado de mapa/evento > eleccion del jugador.
- Si un contexto prohibe seguidor, no se pierde la seleccion: queda en memoria y se restaura automaticamente cuando el contexto vuelva a permitirlo.
- Cada mapa puede tener un flag `allowFollower` para forzar habilitado/deshabilitado.

## Integracion tecnica validada en el repo
- Punto de entrada del menu de equipo:
	- `Task_HandleChooseMonInput` en `src/party_menu.c`
	- `PartyMenuButtonHandler` en `src/party_menu.c`
- Hook recomendado:
	- Agregar retorno para `SELECT_BUTTON` en `PartyMenuButtonHandler`.
	- Manejar ese retorno en `Task_HandleChooseMonInput` solo en `PARTY_MENU_TYPE_FIELD`.
- API follower ya existente:
	- `PlayerHasFollowerNPC`, `CreateFollowerNPC`, `DestroyFollowerNPC`, `HideNPCFollower` en `src/follower_npc.c` y `include/follower_npc.h`
	- Soporte de ocultar/reaparecer por efectos de campo ya existe en `src/field_effect.c` y `src/field_screen_effect.c`.

## Colisiones de input revisadas
- En `src/party_menu.c`, el flujo principal de seleccion usa A, B y START en `PartyMenuButtonHandler`.
- No hay uso actual de SELECT en ese flujo principal, por lo que SELECT es un candidato limpio para el toggle de seguidor.
- Recomendacion: limitar a `PARTY_MENU_TYPE_FIELD` para no afectar menus de batalla, daycare o modos especiales.

## Lista inicial de restricciones por contexto
- Prohibido temporalmente:
	- Surf, buceo y cascada.
	- Bicicleta activa.
	- Cutscenes y movimientos forzados.
- Prohibido por mapa (fase 1):
	- Interiores de ancho estrecho y pasillos de 1 tile.
	- Mapas de puzzle donde el follower pueda bloquear pathing.
- Permitido por defecto:
	- Rutas, ciudades, edificios amplios, centros Pokemon y tiendas (salvo excepcion explicita).

## Consecuencias
- Positivas:
	- Flujo rapido y natural para el jugador.
	- Reutiliza una interaccion comun sin agregar menus nuevos.
	- Facilita cambiar seguidor segun preferencia estetica o narrativa.
- Negativas:
	- Puede entrar en conflicto con atajos existentes del boton SELECT en ciertos contextos.
	- Requiere mensajes claros para confirmar activacion/desactivacion.
- Riesgos:
	- En mapas o estados especiales (bicicleta, surf, interiores restringidos) puede haber casos limite.

## Alternativas consideradas
1. Activar seguimiento desde un NPC dedicado.
2. Agregar un submenu propio de seguimiento en opciones.

## Pendientes tecnicos
- [x] Revisar colisiones con comportamiento actual de SELECT en menu Pokemon
- [ ] Implementar soporte SELECT en `PartyMenuButtonHandler` + `Task_HandleChooseMonInput`
- [ ] Definir lista de mapas con `allowFollower = false`
- [ ] Definir mecanismo de persistencia de Pokemon seleccionado como seguidor
- [ ] Definir exclusiones por especie/tamano de Pokemon (si aplica)

## Enlaces
- [[Indice]]
