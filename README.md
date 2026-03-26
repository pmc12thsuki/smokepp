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
smoke++          # quick smoke (~10 sec)
smoke++ -all     # full smoke (~2.5 min)
```

## Flags

| Flag | Description |
|------|-------------|
| (none) | Quick mode. Burns for ~10 seconds. |
| `-all` | Full burn mode. Burns for ~2.5 minutes. **Cannot be interrupted with Ctrl+C.** You must watch it burn. To force quit: `kill smoke++` in another terminal. |
