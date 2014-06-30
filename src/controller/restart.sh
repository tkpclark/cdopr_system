cd $(dirname $0)
prog="python controller.py"
kill `ps ax -H|grep "$prog"|grep -v grep|awk '{print $1}'` > /dev/null 2>&1
kill -9 `ps ax -H|grep "$prog"|grep -v grep|awk '{print $1}'` > /dev/null 2>&1
setsid /bin/mn.sh $prog &
#ps ax -H|grep "$prog"|grep -v grep
