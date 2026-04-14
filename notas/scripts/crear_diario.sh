#!/usr/bin/env bash
set -euo pipefail

VAULT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
TODAY="$(date +%F)"
TARGET="$VAULT_DIR/Diario/$TODAY.md"

if [[ -f "$TARGET" ]]; then
  echo "Ya existe: $TARGET"
  exit 0
fi

cat > "$TARGET" <<EOF
# Registro $TODAY

## Hecho hoy
- 

## Proximos pasos
- 

## Bloqueos
- 

## Enlaces
- [[Indice]]
EOF

echo "Creado: $TARGET"
