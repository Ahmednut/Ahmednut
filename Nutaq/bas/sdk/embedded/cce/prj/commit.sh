#!/bin/bash
echo commit bas pour cce
echo Commit arborescence bas pour cce.
echo Appuyez sur ENTER pour continuer ou Ctrl-C pour interrompre...
read -n1 -r
CVS_COMMAND="cvs -r -d :pserver:${CVS_USER}@repository:/lyrtech commit"
${CVS_COMMAND} "projets/bas/sdk/embedded/cce"
${CVS_COMMAND} "projets/bas/sdk/embedded/lib"
