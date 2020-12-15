#!/bin/bash
if [ /testhtml1/httpd/index.html -nt /testhtml2/httpd/index.html ];then
        killall httpd
        /usr/local/apache/bin/httpd -f /usr/local/apache/conf/httpd.conf
        /usr/local/apache/bin/apachectl start
        /root/Project/exetesthtml2
else
        killall httpd
        /usr/local/apache/bin/httpd -f /usr/local/apache/conf/httpd2.conf
        /usr/local/apache/bin/apachectl start
        /root/Project/exetesthtml1
fi
