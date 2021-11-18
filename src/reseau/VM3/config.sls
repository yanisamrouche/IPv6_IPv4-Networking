# Tuer Network Manager pour ne pas interférer dans la configuration réseau
NetworkManager:
  service:
    - dead
    - enable: False

# Supprimer la route par default 
ip route del default:
  cmd:
    - run

# Configuration  eth1
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - enable_ipv4: true
    - enable_ipv6: false
    - ipaddr: 172.16.2.183
    - netmask: 28
    # Route par défaut
    - gateway: 172.16.2.186








