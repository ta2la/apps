# PROMPT_ASSEMBLER on Linux - Postup

## Architektura
- Stdin pres named pipe `/tmp/prompt_in`, stdout do `/tmp/prompt_out.log`
- GUI bezi paralelne a je viditelny

## 1. Spustit

```bash
killall PROMPT_ASSEMBLER 2>/dev/null
sleep 1
rm -f /tmp/prompt_in /tmp/prompt_out.log
mkfifo /tmp/prompt_in
cd /home/pt/KADLUB/cvz/APPS/PROMPT_ASSEMBLER/debug
sleep infinity > /tmp/prompt_in &
./PROMPT_ASSEMBLER < /tmp/prompt_in > /tmp/prompt_out.log 2>&1 &
sleep 2
```

**Bez `| tee`** - tee rozbije GUI viditelnost. Pouzivam `>` redirect.

**Poznamka**: Bez vyslovneho pozadavku app **nezastavuji**. Pracuji s jiz bezici instanci.

## 2. Poslat prikaz

```bash
echo "PRIKAZ" > /tmp/prompt_in
```

Vice prikazu lze poslat za sebou - jdou do fronty:
```bash
echo "prikaz1" > /tmp/prompt_in
echo "prikaz2" > /tmp/prompt_in
echo "prikaz3" > /tmp/prompt_in
```

## 3. Cist vystup

```bash
tail -n +OFFSET /tmp/prompt_out.log
```

Kde OFFSET = posledni precteny radek + 1. Ctu nove radky od posledniho cteni.

## 4. Cekat na dalsi instrukci

Cekam. Kdyz uzivatel napise, prectu log a odpovim.

**Limitace**: Nemam trigger/notifikaci. Vzdy musim aktivne precist log. Uzivatel musi napsat, abych log prectol.

## Repeat: 2-4

## Direktiva: Auto-cteni logu

Pri KAZDE zprave od uzivatele:
1. Prectu nove radky z `/tmp/prompt_out.log`
2. Pokud jsou nove radky, vypisem je jako: `PROMPT_ASSEMBLER output: <obsah>`
3. Pak reaguji na to co uzivatel psal
