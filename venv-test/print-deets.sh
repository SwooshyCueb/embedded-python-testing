#!/bin/bash

set -e

if [ ! -z "${1}" ]; then
	. "venv-${1}/bin/activate"
fi

./print-deets.py
