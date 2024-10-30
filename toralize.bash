#!/bin/bash

export LD_PRELOAD=/home/uf/code/toralize/toralize.so  #export ld_preload and point it towards the toralize.so

${@}  #will run all the arguments to the script

unset LD_PRELOAD