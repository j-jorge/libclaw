#!/bin/bash

set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"

find ../../../lib -name "*.[cht]pp" -print0 \
     | xargs -0 xgettext --output=libclaw.pot --c++ --trigraphs --sort-output \
             --keyword=claw_gettext --keyword=claw_ngettext:1,2

for f in *.po
do
    msgmerge --update --previous --backup=none "$f" libclaw.pot
done
