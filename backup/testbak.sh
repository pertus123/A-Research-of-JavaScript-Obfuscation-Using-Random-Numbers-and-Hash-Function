#!/bin/bash
timestamp=`date +%Y%m%d%H%M`
test=${timestamp:10}
half="00"
secend="30"


if [ "$test" == "$half" ];then
	killall httpd
	./root/test/testdir/test/htmltest.out
	/usr/local/apache/bin/httpd -f /usr/local/apache/conf/httpd2.conf
	/usr/local/apache/bin/apachectl start
elif [ "$test" == "$secend" ];then
	killall httpd
	./root/test/testdir/test/testhtml.out
	/usr/local/apache/bin/httpd -f /usr/local/apache/conf/httpd.conf
	/usr/local/apache/bin/apachectl start
fi

echo "$timestamp"
echo ${timestamp:10}
echo "$test"

