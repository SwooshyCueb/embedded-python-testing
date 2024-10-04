#!/bin/bash

set -x

for pyver in 3.5m 3.6m 3.7m 3.8 3.9 3.10 3.11 3.12; do
	python${pyver} -m venv --system-site-packages venv-${pyver}
done

