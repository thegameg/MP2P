#!/bin/sh

#Write all the commands you want to execute in the command_list file, or alternatively,
#provide a file as first argument.
#2nd argument: whatever you want, this is just to specify that your commands should be done on every servers at the same time.

IP0="192.168.1.40"
IP1="192.168.1.41"
IP2="192.168.1.42"

PORT0="22"
PORT1="22"
PORT2="22"

read -s -p "Please enter your MP2P password:" PWD
echo
if [[ $(whoami) != "rusty" ]]; then
  IP0="rusty.myftp.org"
  IP1="rusty.myftp.org"
  IP2="rusty.myftp.org"
  PORT0="40"
  PORT1="41"
  PORT2="42"
fi

if [ "$#" -eq 2 ] && [ -f "$1" ]; then
  echo Trying to connect to Raspberry Py 0...
  sshpass -p $PWD ssh mp2p@$IP0 -p $PORT0 -T 'sh -s' < $1 &
  echo
  echo Trying to connect to Raspberry Py 1...
  sshpass -p $PWD ssh mp2p@$IP1 -p $PORT1 -T 'sh -s' < $1 &
  echo
  echo Trying to connect to Raspberry Py 2...
  sshpass -p $PWD ssh mp2p@$IP2 -p $PORT2 -T 'sh -s' < $1 &
else
  if [ "$#" -eq 1 ]; then
    if [ -f "$1" ]; then
      echo Trying to connect to Raspberry Py 0...
      sshpass -p $PWD ssh mp2p@$IP0 -p $PORT0 -T 'sh -s' < $1
      echo
      echo Trying to connect to Raspberry Py 1...
      sshpass -p $PWD ssh mp2p@$IP1 -p $PORT1 -T 'sh -s' < $1
      echo
      echo Trying to connect to Raspberry Py 2...
      sshpass -p $PWD ssh mp2p@$IP2 -p $PORT2 -T 'sh -s' < $1
    else
      echo Trying to connect to Raspberry Py 0...
      sshpass -p $PWD ssh mp2p@$IP0 -p $PORT0 -T 'sh -s' < command_list &
      echo
      echo Trying to connect to Raspberry Py 1...
      sshpass -p $PWD ssh mp2p@$IP1 -p $PORT1 -T 'sh -s' < command_list &
      echo
      echo Trying to connect to Raspberry Py 2...
      sshpass -p $PWD ssh mp2p@$IP2 -p $PORT2 -T 'sh -s' < command_list &
    fi
  else
    echo Trying to connect to Raspberry Py 0...
    sshpass -p $PWD ssh mp2p@$IP0 -p $PORT0 -T 'sh -s' < command_list
    echo
    echo Trying to connect to Raspberry Py 1...
    sshpass -p $PWD ssh mp2p@$IP1 -p $PORT1 -T 'sh -s' < command_list
    echo
    echo Trying to connect to Raspberry Py 2...
    sshpass -p $PWD ssh mp2p@$IP2 -p $PORT2 -T 'sh -s' < command_list
  fi
fi
