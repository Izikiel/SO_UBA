#!/bin/bash
rm -f TP3.tar.gz
echo "getting data"
curl http://so.exp.dc.uba.ar/vm/TP3.tar.gz > TP3.tar.gz
echo "uncompressing"
tar -xzvf TP3.tar.gz
echo "setting for db daemon"
mongod --dbpath .