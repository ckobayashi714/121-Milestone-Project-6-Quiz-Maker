#!/bin/bash
WD=`pwd`
EXECFILE=$1
DRIVER=$2
IMPLEMS=$3
HEADERS=$4
ENTRIES="[\n"

ENTRIES=${ENTRIES}"  {\n   \"directory\" : \"$WD\",\n"
ENTRIES=${ENTRIES}"    \"command\" : \"g++ -std=c++17 $DRIVER -o $1\",\n"
ENTRIES=${ENTRIES}"    \"file\" : \"$WD/$DRIVER\"\n  },\n"

for implem in $IMPLEMS
do
  ENTRIES=${ENTRIES}"  {\n   \"directory\" : \"$WD\",\n"
  ENTRIES=${ENTRIES}"    \"command\" : \"g++ -std=c++17 $implem -o $1\",\n"
  ENTRIES=${ENTRIES}"    \"file\" : \"$WD/$implem\"\n  },\n"
done

for header in $HEADERS
do
  ENTRIES=${ENTRIES}"  {\n   \"directory\" : \"$WD\",\n"
  ENTRIES=${ENTRIES}"    \"command\" : \"g++ -std=c++17 $header -o $1\",\n"
  ENTRIES=${ENTRIES}"    \"file\" : \"$WD/$header\"\n  },\n"
done
ENTRIES=${ENTRIES}"]"

echo -e $ENTRIES >> $WD/test/compile_commands.json
