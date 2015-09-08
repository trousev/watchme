echo yes | dh_make -p watchme_0.1 -s -c bsd --createorig
# debuild -S -sd
debuild -i -us -uc -b
