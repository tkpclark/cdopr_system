#!/bin/sh
path=$1
prog=${path##*/}
#echo $prog_name
pid=`ps -ef | grep -v grep | grep -v $0| grep $prog | sed -n  '1P' | awk '{print $2}'`  

if [ "$2" == "start" ] ; then
	if [ -z $pid ] ; then  
		nohup python $path 2&> /dev/null &
	else  
		echo $prog "is running..." $pid  
	fi
	exit
fi

if [ "$2" == "stop" ] ; then
	kill -9 $pid
	#echo 'killing...'
fi
