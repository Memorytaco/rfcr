#!/bin/bash

RFCDIR=$HOME/.rfc
RFCONF=$RFCDIR/conf.yml

if [ -e $RFCDIR ]
then
  echo "Got Configuration Directory: $RFCDIR"
else
  mkdir -p $RFCDIR
  cp -r src/shaders $RFCDIR/
  echo "Create Configuration Directory: $HOME"
fi

if [ -e $RFCONF ]
then
  echo "Find conf yaml file: $RFCONF"
else
  cp -f ./etc/conf.yml $RFCONF
  echo "Create conf file $RFCONF"
fi

echo "successfully install the files"
echo "please put these VARs to your .bashrc"

echo ""
echo "    export RFCONF=$RFCONF"
echo ""

flag=n

echo -n "do you want to sync the repo?[y/n]"
read flag
if [ "$flag" = "y" ]
then
  toret=$PWD
  cd $RFCDIR
  if [ -e repo ] && [ -e repo/rfc-index.txt ]
  then
    echo "  Repository exists!"
    echo "  Done!"
  else
    echo ""
    echo "Starting Sync Repository"
    rsync -avz --delete ftp.rfc-editor.org::rfcs-text-only repo
  fi
  cd $toret
else
  echo "You can execute this command to download the repo"
  echo "make sure you PWD is $RFCDIR"
  echo "    rsync -avz --delete ftp.rfc-editor.org::rfcs-text-only repo ."
fi

echo "finish installing."

