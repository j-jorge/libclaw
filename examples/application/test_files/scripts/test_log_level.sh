#!/bin/sh

./ex-application -b --string="my string" one two three --log-level=0
./ex-application -b --string="my string" one two three --log-level=1
./ex-application -b --string="my string" one two three --log-level=15
