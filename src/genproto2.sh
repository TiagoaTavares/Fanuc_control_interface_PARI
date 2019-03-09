#!/bin/bash
#
# script to create a header file with prototypes of
# all the functions in sources files in current dir
# and subdirs.
#
# Enhanced version to work with various formattings
# of source code. Take advantage of indent C formatter
# preceeded by comment stripping. Should work on most
# C code layouts...
#
#  V. Santos, Dez-2014
#
# New version with extern "C" declarations for C++ compilers
# and sepration of what are C++ and C files based on file
# extension (NB. This may fail if conventions are broken)
#
#  V. Santos, Dez-2015
#
# Another improvement to include namespaces in C++ definitions
# (This one was tricky because ctags has limited ways of expressing namespaces
#
####################################################

command -v ctags  >/dev/null 2>&1 || { echo "ctags is not installed or not in path. sudo apt-get install ctags" >&2; return; }
command -v indent >/dev/null 2>&1 || { echo "indent is not installed or not in path. sudo apt-get install indent" >&2; return; }

rm -rf .vs_tmp

echo "/*File generated automatically in $HOSTNAME by $USER on `date`*/" > prototypes.h

SRC=`find . -iname \*.c`
SRCC=`find . -iname \*.cc -o -iname \*.cpp -o -iname \*.cxx `
HEADS=`find . -iname \*.h`
HEADSPP=`find . -iname \*.hpp`

mkdir -p .vs_tmp
cp -f ${SRC} ${SRCC} ${HEADS} ${HEADSPP} .vs_tmp >/dev/null 2>&1  #Add to redirect because of warning in some systems!
sed -i '/#include .*prototypes.h/d' .vs_tmp/*
cd .vs_tmp
#The best C/C++ comment stripper :-)
sed -i -r 's/\/\/.*$//; :a; s%(.*)/\*.*\*/%\1%; ta; /\/\*/ !b; N; ba' ${SRC} ${SRCC}
#use indent imposing long lines to avoid breaks... universal solution would be ctags --fields=+S
indent -l256 -npsl -gnu ${SRC} ${SRCC}

#---Generate prototypes for "C" functions
echo '#ifdef __cplusplus' >> ../prototypes.h
echo 'extern "C" {' >> ../prototypes.h
echo '#endif' >> ../prototypes.h
for f in ${SRC}
do
        echo "/* $f */" | sed 's|\./||' >> ../prototypes.h
        ctags -x --c-kinds=f ${f} | awk '{for(n=5; n<NF; n++) printf("%s ", $n) ; printf("%s;\n", $NF);}' \
        >> ../prototypes.h
done
echo '#ifdef __cplusplus' >> ../prototypes.h
echo '}' >> ../prototypes.h
echo '#endif' >> ../prototypes.h

#---Generate prototypes for "C++" functions
for f in ${SRCC}
do
        echo "/* $f */" | sed 's|\./||' >> ../prototypes.h
        ctags -f -  --c++-kinds=+pf ${f} \
        | sed 's|/\^||; s|\$/;"||; s|[ \t]\{1,\}| |g' \
        | awk '{for(n=3;n<=NF;n++) printf("%s ", $n); printf("\n")}' \
        | grep " f[ namespace]" \
        | awk '{if($NF=="f") { for(n=1;n<NF-1; n++) printf("%s ",$n); printf("%s;\n",$(NF-1))} else \
               { printf("namespace %s { ", $NF); for(n=1;n<NF-2; n++) printf("%s ",$n); printf("%s; }\n",$(NF-2)) }}' \
        | sed 's|namespace:||' \
        >> ../prototypes.h
done


cd ..
rm -rf .vs_tmp
