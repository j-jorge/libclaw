#!/bin/sh

xgettext --output=libclaw.pot --c++ --trigraphs --sort-output \
    --keyword=claw_gettext --keyword=claw_ngettext:1,2 \
    ../../../claw/code/*.cpp

for f in *.po
do
    msgmerge --update --previous --backup=none "$f" libclaw.pot
done
