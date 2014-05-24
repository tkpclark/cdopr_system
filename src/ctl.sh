#!/bin/sh
path=$1
prog=${path##*/}
#echo $prog_name
pid=`ps -ef | grep -v grep | grep -v $0| grep $prog | sed -n  '1P' | awk '{print $2}'`  


if [ "$2" == "start" ] ; then
	if [ -z $pid ] ; then  
		python $path &
	else  
		echo $prog "is running..." $pid  
	fi
	exit
fi

if [ "$2" == "stop" ] ; then
	kill $pid
	sleep 1
	kill -9 $pid
		
fi
