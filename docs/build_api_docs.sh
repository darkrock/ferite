#!/bin/sh

if ! test -d generated; then
    mkdir generated;
fi
cd generated


root="../.."
rm -rf api/
mkdir api/

feritedoc --regenerate --prefix api/

