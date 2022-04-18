#!/bin/sh

if [ "$1" != "" ]
then
  OUTPUT_DIR="$1"
else
  OUTPUT_DIR=.
fi

# this test creates three files:
#
# log-file.0.out should be empty.
# log-file.1.out should contain:
#
#  error:error_level:program_name == ./ex-application
#
# log-file.15.out should contain:
#
#  error:error_level:program_name == ./ex-application
#  warning:warning_level:argument 0 == one
#  warning:warning_level:argument 1 == two
#  warning:warning_level:argument 2 == three
#  verbose_level:m_app is running
#
./ex-application -b --string="my string" one two three --log-level=0 --log-file="$OUTPUT_DIR/log-file.0.out"
./ex-application -b --string="my string" one two three --log-level=1 --log-file="$OUTPUT_DIR/log-file.1.out"
./ex-application -b --string="my string" one two three --log-level=15 --log-file="$OUTPUT_DIR/log-file.15.out"
