#!/bin/bash
rm -f TP3.tar.gz
echo "getting data"
curl http://so.exp.dc.uba.ar/vm/TP3.tar.gz > TP3.tar.gz
echo "uncompressing"
tar -xzvf TP3.tar.gz
echo "setting for db daemon"
rm -f mongod.lock
2>/dev/null 1>/dev/null kill -15 $(pgrep -f mongod)
mongod --dbpath .