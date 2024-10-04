#!/bin/bash

set -e

./print-deets.sh

for pyver in 3.5m 3.6m 3.7m 3.8 3.9 3.10 3.11 3.12; do
	./print-deets.sh "${pyver}"
done

