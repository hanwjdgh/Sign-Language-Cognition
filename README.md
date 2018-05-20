# Sign-Language-Cognition

You can find a way to install intel 5300 nic(Linux 802.11n CSI Tool) at https://dhalperi.github.io/linux-80211n-csitool/installation.html

## Additional installation(After step 5.Enable Logging and Test)
```bash
sudo ifconfig wlan0 up & sudo ifconfig eth0 up
sudo iwlist wlan0 scanning | grep ESSID
sudo iwconfig wlan0 essid "AP" //link your router name
sudo ls /sys/kernel/debug/ieee80211/phy0/netdev:wlan0/stations/
[sudo] password for wens:
Mac address

echo 0x420A | sudo tee /sys/kernel/debug/ieee80211/phy0/netdev:wlan0/stations/Mac address/rate_scale_table
sudo iwconfig wlan0 essid "AP"
sudo dhclient wlan0
```

## Open socket using log_to_server.c
```bash
cd linux-80211n-csitool-supplementary/netlink
sudo ./log_to_server <ip> <port>  (ip address can get ifconfig)
```

## Read socket using read_socket.m
```bash
cd linux-80211n-csitool-supplementary/matlab matlab
Run read_socket.m
```
