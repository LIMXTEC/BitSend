MASTERNODE BUILD NOTES
======================
Some notes on how to build a Limx Masternode in Ubuntu server. Using Windows Qt Client to configure.


System requirements
--------------------
An Ubuntu 14.10 64bit server is recommended with at least 768MB 
of memory and 15GB space available when running a Masternode.


Notes
-----
You need exactly 5000 LIMX to run a Masternode. Masternode input must have at least 15 confirmations.
Building a new wallet.dat is also recommended (a seperate .conf is needed). Masternode earnings 
going to this wallet.dat and address. Send only new earned coins away and pay attention for transaction fees.
To fix a broken accountaddress 0 (possible when sending huge amounts of coins away), do a self tx of 5000 LIMX.


Start
-----
Open your Windows Limecoinx-Qt Client and open the debug console.

	masternode genkey
	copy the generated code to a text file
	---
	getaccountaddress 0
	copy the generated address to a text file
	---
	encryptwallet "strong password"
	Send 5000 LIMX to "address 0"
	
	
	Rent a Vultr Ubuntu 14.10 64bit server
	--------------------------------------
	http://www.vultr.com/?ref=6821776
	
	Vultr accepts Bitcoin payments.
	


Build Instructions: Ubuntu & Debian
-----------------------------------

	$ adduser <NEW USER>
	$ passwd <PSW>
	$ gpasswd -a <NEW USER> sudo
	---
	$ sudo apt-get install build-essential
	$ sudo apt-get install libtool autotools-dev autoconf automake libssl-dev
	$ sudo apt-get install libboost1.54-all-dev
	$ sudo add-apt-repository ppa:bitcoin/bitcoin
	$ sudo apt-get install libdb4.8-dev libdb4.8++-dev
	$ sudo apt-get install miniupnpc*-dev
	$ sudo apt-get install git ntp make g++ gcc autoconf cpp ngrep iftop sysstat unzip
	$ sudo update-rc.d ntp enable
	$ sudo apt-get update
	$ sudo apt-get upgrade
	---
	optiopnal linux gui: $ sudo apt-get install lubuntu-desktop
	---
	optional, if problems with boost version: 
	$ sudo apt-get remove libboost*
	$ sudo apt-get purge libboost*
	$ sudo apt-get install libboost1.54-all-dev


	Swapfile:
	---------
	$ sudo dd if=/dev/zero of=/swapfile bs=1M count=1024
	$ sudo mkswap /swapfile
	$ sudo swapon /swapfile

	
	FireWall:
	---------
	$ sudo apt-get install ufw
	$ sudo ufw allow ssh/tcp
	$ sudo ufw limit ssh/tcp
	$ sudo ufw allow 8886/tcp
	$ sudo ufw logging on
	$ sudo ufw enable
	$ sudo ufw status


	Install LIMX
	------------
	mkdir .limecoinx
	cd .limecoinx
	wget http://sourceforge.net/projects/limx/files/limx-linux64.zip
	unzip limx-linux64.zip
	
	$ sudo cp limecoinxd /usr/bin
	$ sudo chmod 775 /usr/bin/limecoinxd

	
	Create a limecoinx.conf in nano
	-------------------------------
	cd .limecoinx
	nano
	---
	rpcuser=<anything>
	rpcpassword=<anything>
	rpcallowip=127.0.0.1
	maxconnections=256
	listen=1
	server=1
	daemon=1
	masternode=1
	masternodeprivkey=XXXXXX
	externalip=xxx.xxx.xxx.xxx:8886
	---
	save nano: Ctrl +  O
	exit nano: Ctrl +  X
	cd


	Create CRON
	-----------
	cd /etc/cron.d
	crontab -e
	2 (for nano)
	
	at bottom of newly created file, insert:
	---
	@reboot /usr/bin/limecoinxd -shrinkdebugfile    [to start masternode  (or wherever you keep your daemon)]
	*/20 * * * * /usr/bin/limecoinxd
	---
	save nano: Ctrl +  O
	exit nano: Ctrl +  X
	cd
	
	
	Manually start limecoinxd
	-------------------------
	cd .limecoinx
	./limecoinxd
	
	
	limcoinxd commands
	------------------
	./limecoinxd getinfo
	./limecoinxd masternode list
	./limecoinxd masternode debug
	./limecoinxd masternode stop
	./limecoinxd help
	

Windows Limecoinx-Qt Client configuration 
-----------------------------------------
(if using a seperate wallet.dat, a seperate limecoinx.conf is needed)

	---
	go to limecoinx.conf in %appdata%
	
	---
	rpcuser=<anything>
	rpcpassword=<anything>
	maxconnections=256
	masternode=1
	masternodeprivkey=XXXXXXXXXXXXXXXXXX
	masternodeaddr=xxx.xxx.xxx.xxx:8886
	---

	
	Windows Limecoinx-Qt Client console
	-----------------------------------
	masternode start <PSW>

	
	Windows Limecoinx-Qt Client console commands
	--------------------------------------------
	masternode list    	 <<find your MN IP
	masternode debug
	masternode stop
	getinfo
	help

	
