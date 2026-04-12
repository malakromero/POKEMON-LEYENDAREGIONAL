#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
  echo "Uso: ./scripts/crear_adr.sh \"Titulo ADR\""
  exit 1
fi

VAULT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
ADR_DIR="$VAULT_DIR/ADR"
TITLE_RAW="$*"

mkdir -p "$ADR_DIR"

LAST_NUM="$(find "$ADR_DIR" -maxdepth 1 -type f -name 'ADR-*.md' \
  | sed -E 's|.*/ADR-([0-9]{3}).*|\1|' \
  | sort -n \
  | tail -n 1)"

if [[ -z "${LAST_NUM:-}" ]]; then
  NEXT_NUM=1
else
  NEXT_NUM=$((10#$LAST_NUM + 1))
fi

ADR_ID="$(printf 'ADR-%03d' "$NEXT_NUM")"

SLUG="$(echo "$TITLE_RAW" \
  | tr '[:upper:]' '[:lower:]' \
  | sed -E 's/[^a-z0-9]+/-/g; s/^-+//; s/-+$//')"

if [[ -z "$SLUG" ]]; then
  SLUG="sin-titulo"
fi

FILE_NAME="$ADR_ID-$SLUG.md"
TARGET="$ADR_DIR/$FILE_NAME"
TODAY="$(date +%F)"

cat > "$TARGET" <<EOF
# $ADR_ID: $TITLE_RAW

## Estado
Propuesto

## Fecha
$TODAY

## Contexto

## Decision

## Consecuencias
- Positivas:
- Negativas:
- Riesgos:

## Alternativas consideradas
1. 
2. 

## Enlaces
- [[Indice]]
EOF

echo "Creado: $TARGET"
