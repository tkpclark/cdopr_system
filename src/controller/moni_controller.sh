#!/bin/bash
while true
do
	eval $* 
	chpid="$!"
	wait $chpid
	sleep 1
done
