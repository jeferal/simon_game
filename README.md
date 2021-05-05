# simon_game

## Compilation

```bash
arm-linux-gnueabihf-g++-8 test.cpp -o build/test -lpthread
```
## Copy executable to BBB
```bash
scp build/test root@192.168.1.111:/root/targets/
```
