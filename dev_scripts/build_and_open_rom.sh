#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="/workspaces/Pokemon-Leyenda-Regional"
ROM_PATH="$REPO_ROOT/pokemon_leyenda_regional.gba"

cd "$REPO_ROOT"

echo "[1/2] Compilando ROM..."
make -j"$(nproc)"

echo "[2/2] Abriendo ROM en VS Code..."
if command -v code >/dev/null 2>&1; then
    code "$ROM_PATH"
else
    echo "No se encontro el comando 'code'."
    echo "Abre manualmente: $ROM_PATH"
fi

echo "Listo. En VS Code, usa la extension NDS and GBA Emulator para ejecutar la ROM abierta."
