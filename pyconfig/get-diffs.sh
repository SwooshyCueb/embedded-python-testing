#!/bin/bash

set -e

mkdir -p tmp

for pyver in 3.5m 3.6m 3.7m 3.8 3.9 3.10 3.11 3.12; do
	out/pyconfig_test8_${pyver} > tmp/nenv_${pyver}.out
	out/pyconfig_test9_${pyver} > tmp/venv_${pyver}.out
	diff -rup tmp/nenv_${pyver}.out tmp/venv_${pyver}.out > tmp/env_${pyver}.diff
done
