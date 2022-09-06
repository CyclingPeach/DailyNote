

- 单独编译`preloader`命令

```bash
cd LINUX/scripts
./bld -p ROO --model-code=0UWW --major-version=1 --minor-version=000 preloader
```

- 单独编译`boot`和`odmdtbo`命令

```bash
./bld -p ROO --model-code=0UWW --major-version=1 --minor-version=000 boot
./bld -p ROO --model-code=0UWW --major-version=1 --minor-version=000 odmdtbo
```


- 单独烧录 `preloader`

```bash
sudo fastboot flash preloader ROO-0-1000-00WW-preloader.BIN
```

- 单独烧录 `bootimage`

```bash
sudo fastboot flash boot ROO-0-1000-00WW-boot.img
```

- 单独烧录 `odmdtbo`

```bash
sudo fastboot flash odmdtbo ROO-0-1000-00WW-odmdtbo.img
```


