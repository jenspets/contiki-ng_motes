# contiki-ng_motes
Some motes for testing various parts of Contiki-NG

For using docker, add a mount point for this directory:
alias contiker='docker run --privileged --sysctl net.ipv6.conf.all.disable_ipv6=0 --mount type=bind,source=<contiki-ng-path>/contiki-ng,destination=/home/user/contiki-ng --mount type=bind,source=<contiki-ng-mote-path>/contiki-ng_motes,destination=/home/user/contiki-ng_motes -e DISPLAY=:0 -v /tmp/.X11-unix:/tmp/.X11-unix -v /dev/bus/usb:/dev/bus/usb -ti contiker/contiki-ng'
