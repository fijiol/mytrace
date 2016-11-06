#!/bin/bash -x

foo() {
	sleep $1
	echo $2
}

PIDS=

foo 2 2 &
PIDS="$PIDS $!"

foo 1 1 &
PIDS="$PIDS $!"

wait $PIDS

