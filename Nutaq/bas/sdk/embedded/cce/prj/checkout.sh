#!/bin/bash
echo checkout tag [if tag is not specified, get from head]
echo Recupere arborescence bas pour cce.
echo ATTENTION: cette operation efface/ecrase le repertoire "projets"!
echo Appuyez sur ENTER pour continuer ou Ctrl-C pour interrompre...
read -n1 -r
if [ -d projets ]; then rm -rf projets; fi

if [ -z "$1" ]; then
CVS_COMMAND="cvs -d :pserver:${CVS_USER}@repository:/lyrtech checkout -P"
else
CVS_COMMAND="cvs -d :pserver:${CVS_USER}@repository:/lyrtech checkout -P -r $1" 
fi

${CVS_COMMAND} "projets/bas/sdk/embedded"
${CVS_COMMAND} "projets/bas/sdk/host"
