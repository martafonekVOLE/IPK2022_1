# IPK 2022 Project

## Popis projektu
Výsledkem projektu je server, komunikující prostřednictvím HTTP protokolu. Server je implementovám pomocí jazyka C (metodou *Socket programming*). Server poskytuje
různé služby (sekce [Poskytované služby](#services)). 

Po zavolání služby server oznámí, jaký zpracovává požadavek a uživateli se dostane odpovědi.

Server byl vyvíjen pro operační systémy **Linux** (Ubuntu 20.04 LTS).

## Způsoby komunikace
Se serverem je možné komunikovat **primárně pomocí webového prohlížeče**.

Dále lze komunikovat pomocí nástrojů `curl` a `wget` (sekce [Příklady](#examples)).

## Poskytované služby <a name="services"></a>
+ `hostname` - zobrazí informaci o názvu (*GET http://servername:12345/hostname*)
+ `cpu-name` - zobrazí informaci o jménu procesoru (*GET http://servername:12345/hostname*)
+ `load`     - zobrazí informaci o aktuálním vytížení procesoru (*GET http://servername:12345/load*)

## Způsob spuštění projektu
1. Rozbalit archiv xpechm00.zip
2. Spustit soubor makefile 
3. Spustit server  
> ***Pozor!*** Je třeba uvést port, na kterém bude server naslouchat (12345)

```bash
$ unzip xpechm00.zip
$ make
$ ./hinfosvc 12345
```

## Způsob zasílání požadavků (příklady) <a name="examples"></a>
> Příklady jsou uvádět na *localhost* s naslouchaným portem *12345*

### Zjištění jména
Použití **Curl** nebo **Wget**
```bash
$ curl http://localhost:12345/hostname
$ wget http://localhost:12345/hostname
```

Použitím **prohlížeče** - zadat do URL `http://localhost:12344/hostname`

### Zjištění jména procesoru
Použití **Curl** nebo **Wget**

```bash
$ curl http://localhost:12345/cpu-name
$ wget http://localhost:12345/cpu-name
```

Použitím **prohlížeče** - zadat do URL `http://localhost:12344/cpu-name`

### Zjištění vytížení procesoru
Použití **Curl** nebo **Wget**
```bash
$ curl http://localhost:12345/load
$ wget http://localhost:12345/load
```

Použitím **prohlížeče** - zadat do URL `http://localhost:12344/load`

> Server lze ukončit stisknutím `Ctrl + C`
