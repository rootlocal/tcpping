tcpping
=======

Sequentially creates tcp connections to the specified host and measures the latency.

### Build
```bash
mkdir build
cd build
cmake ../
make
```

or
```bash
make 
```

### Install
```bash
mkdir build
cd build
cmake -D CMAKE_INSTALL_PREFIX=to_install_dir/tcpping ../
make
make install
```

or
```bash
make install CMAKE_INSTALL_PREFIX=to_install_dir/tcpping
```

### Test
```bash
mkdir build
cd build
cmake -D ENABLE_UNIT_TESTS=ON ../
make
make test
./bin/unit_tests
```

or
```bash
make test
```

### Usage:
<pre>
$ tcpping -h google.com -p 443 -c 4
OK Connected to 173.194.73.101:443, seq=1, time=5.7520 ms
OK Connected to 173.194.73.101:443, seq=2, time=8.5420 ms
OK Connected to 173.194.73.101:443, seq=3, time=8.3390 ms
OK Connected to 173.194.73.101:443, seq=4, time=8.6580 ms

--- 173.194.73.101:443 tcpping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 4017.3520 ms
rtt min/avg/max/mdev = 5.7520/7.8220/8.6580/1.2050 ms
</pre>