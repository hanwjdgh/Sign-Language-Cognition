# Sign-Language-Cognition

You can find a way to install intel 5300 nic(Linux 802.11n CSI Tool) at https://dhalperi.github.io/linux-80211n-csitool/installation.html

## Additional ㅑnstallation(After step 5)
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
