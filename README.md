# IPK 2022 Project

## Popis projektu
Výsledkem projektu je server, komunikující prostřednictvím HTTP protokolu. Server je implementovám pomocí jazyka C (metodou *Socket programming*). Server poskytuje
různé služby (sekce [Poskytované služby](#services)). 

Po zavolání služby server oznámí, jaký zpracovává požadavek a uživateli se dostane odpovědi.

## Způsoby komunikace
Se serverem je možné komunikovat **primárně pomocí webového prohlížeče**.

Dále lze komunikovat pomocí nástrojů `curl` a `wget` (sekce [Příklady](#examples)).

## Poskytované služby <a name="services"></a>
+ `hostname` - zobrazí informaci o názvu (*GET http://servername:12345/hostname*)
+ `cpu-name` - zobrazí informaci o jménu procesoru (*GET http://servername:12345/hostname*)
+ `load`     - zobrazí informaci o aktuálním vytížení procesoru (*GET http://servername:12345/load*)

## Způsob spuštění projektu
1. Rozbalit archiv xpechm00.zip (`unzip xpechm00.zip`)
2. Spustit soubor makefile (`make`)
3. Spustit server (`./hinfosvc 12345`) 
> ***Pozor!*** Je třeba uvést port, na kterém bude server naslouchat (`12345`)

## Způsob zasílání požadavků (příklady) <a name="examples"></a>
> Příklady jsou uvádět na *localhost* s naslouchaným portem *12345*

### Zjištění jména
Použití **Curl** `curl http://localhost:12345/hostname`

Použití **Wget** `wget http://localhost:12344/hostname`

Použitím **prohlížeče** - zadat do URL `http://localhost:12344/hostname`

### Zjištění jména procesoru
Použití **Curl** `curl http://localhost:12345/cpu-name`

Použití **Wget** `wget http://localhost:12344/cpu-name`

Použitím **prohlížeče** - zadat do URL `http://localhost:12344/cpu-name`

### Zjištění vytížení procesoru
Použití **Curl** `curl http://localhost:12345/load`

Použití **Wget** `wget http://localhost:12344/load`

Použitím **prohlížeče** - zadat do URL `http://localhost:12344/load`

> Server lze ukončit stisknutím `Ctrl + C`
