#!/bin/bash

echo "🚀 Iniciando sincronización con GitHub..."
git status

echo "📦 Agregando archivos..."
git add .

echo "💾 Creando commit..."
git commit -m "Respaldo y unificación de cambios"

echo "🌐 Empujando a GitHub..."
# Empujando a la rama que tengas (se omite -u para usar la por defecto y no fallar si no hay upstream, o se usa directo origin)
BRANCH=$(git rev-parse --abbrev-ref HEAD)
git push origin $BRANCH

echo "✅ ¡Sincronización terminada!"
echo ""
echo "⚠️ RECUERDA PARA GOOGLE DRIVE:"
echo "No sincronices esta carpeta directamente con el cliente de Drive de escritorio porque se corromperá el archivo .git."
echo "La mejor forma de tener un backup en Drive es comprimir la carpeta entera en un .zip y subirlo."
