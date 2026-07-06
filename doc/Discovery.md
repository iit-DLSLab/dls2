# Discovery
In DLS2, by default it is used the [Discovery Server](https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery_server.html#discovery-server) method for finding the nodes on the network. When connecting to a robot local network, your pc (and the on-boards one) belongs to the same local network, so the servers needs to be launched in this local network, instead of 127.0.0.1. To do that you can 
- create your own servers.yaml file. For example it can have

  ```
    layers: 
      ip: "192.168.123.15"
      port: 11812
    command:
      ip: "192.168.123.15"
      port: 11813
    signals:
      ip: "192.168.123.15"
      port: 11814
    logging:
      ip: "192.168.123.15"
      port: 11818
  ```

    which means that all the servers are launched on the pc with ip `192.168.123.15`.
- export the environment variable DLS_SERVERS_PATH, setting the path to your servers.yaml. For example, if servers.yaml is in the home folder

      
    `export DLS_SERVERS_PATH=~/servers.yaml`
      
    Notice that to unset the variable, you can do `uset DLS_SERVERS_PATH`.