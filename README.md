# smoke++

A burning cigarette for your terminal. Inspired by [sl(1)](https://github.com/mtoyoda/sl).

## Install

Requires only a C compiler (gcc or cc). No external libraries needed.

```
git clone https://github.com/pmc12thsuki/smokepp.git
cd smokepp
make
make setup
source ~/.zshrc
```

`make setup` adds the project directory to your `PATH` in `~/.zshrc`.

## Usage

```
smoke++              # quick smoke (~10 sec)
smoke++ -a           # full smoke (~2.5 min)
smoke++ -c           # smoke with a steaming coffee
smoke++ -w           # Winston Red (brand on the filter)
smoke++ -c -w        # flags can be combined
```

## Flags

Flags are independent and can be combined in any order.

| Flag | Description |
|------|-------------|
| (none) | Quick mode. Burns for ~10 seconds. |
| `-a`, `--addict` | Full burn mode. Burns for ~2.5 minutes. **Cannot be interrupted with Ctrl+C.** You must watch it burn. To force quit: `killall 'smoke++'` in another terminal. |
| `-c`, `--coffee` | Adds a steaming cup of coffee to the left of the cigarette. |
| `-w`, `--winston` | Prints the `WINSTON` brand on the filter in glowing red, instead of the plain `#######` cork. |
