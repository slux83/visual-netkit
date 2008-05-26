!###
!# This file is createb by Visual Netkit <VISUAL_NETKIT_VERSION> version
!# http://www.netkit.org ~ http://code.google.com/p/visual-netkit/
!#
!
!Just an ipv4 template
!
!Startup file config code slice 
<STARTUP_FILE>
	<PATH>/sbin/ifconfig</PATH> <HI>eth0</HI> <IP>100.1.64.2</IP> <NETMASK>netmask <NMASK_ADDR>255.255.192.0</NMASK_ADDR></NETMASK> <BROADCAST>broadcast <BCAST_ADDR>200.1.127.255</BCAST_ADDR><BROADCAST> <HI_STATE>up</HI_STATE>
</STARTUP_FILE>
!
!IPv4 config code slice for RIP
<RIP>
	<NETWORK>network <NET_ADDR>100.1.0.0/<NMASK>16</NMASK></NET_ADDR></NETWORK>
</RIP>
!
!IPv4 config code slice for BGP
<BGP>
	<NETWORK>network <NET_ADDR>0.0.0.0</NET_ADDR>/<NMASK>0</NMASK></NETWORK>
	<NEIGHBOR>neighbor <NET_ADDR>11.0.0.25</NET_ADDR> <REMOTE_AS>remote-as <REMOTE_AS_NAME>30</REMOTE_AS_NAME></REMOTE_AS></NEIGHBOR>
</BGP>
!
